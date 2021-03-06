/*   
 *   File: optik.c
 *   Author: Vasileios Trigonakis <vasileios.trigonakis@epfl.ch>
 *   Description:  
 *   optik.c is part of ASCYLIB
 *
 * Copyright (c) 2015 Vasileios Trigonakis <vasileios.trigonakis@epfl.ch>,
 *	      	      Distributed Programming Lab (LPD), EPFL
 *
 * ASCYLIB is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "skiplist-optik.h"
#include "utils.h"

RETRY_STATS_VARS;

#include "latency.h"
#if LATENCY_PARSING == 1
__thread size_t lat_parsing_get = 0;
__thread size_t lat_parsing_put = 0;
__thread size_t lat_parsing_rem = 0;
#endif	/* LATENCY_PARSING == 1 */

extern ALIGNED(CACHE_LINE_SIZE) unsigned int levelmax;

#define MAX_BACKOFF 131071
#define OPTIK_MAX_MAX_LEVEL 64 /* covers up to 2^64 elements */

/*
 * finds the predecessors and the successors of a key, 
 */
static sl_node_t*
sl_optik_search(sl_intset_t* set, skey_t key, sl_node_t** preds, sl_node_t** succs, optik_t* predsv)
{
 restart:
  PARSE_TRY();
	
  sl_node_t* node_found = NULL;
  sl_node_t* pred = set->head;
  optik_t predv = set->head->lock;
	
  int i;
  for (i = (pred->toplevel - 1); i >= 0; i--)
    {
      sl_node_t* curr = pred->next[i];
      optik_t currv = curr->lock; 

      while (key > curr->key)
	{
	  predv = currv;
	  pred = curr;

	  curr = pred->next[i];
	  currv = curr->lock;  
	}

      if (unlikely(node_is_unlinking(pred)))
      	{
      	  goto restart;
      	}
      preds[i] = pred;
      succs[i] = curr;
      predsv[i] = predv;
      if (key == curr->key)
	{
	  node_found = curr;
	}
    }
  return node_found;
}

inline sl_node_t*
sl_optik_left_search(sl_intset_t* set, skey_t key)
{
  PARSE_TRY();
  int i;
  sl_node_t* pred, *curr, *nd = NULL;
	
  pred = set->head;
	
  for (i = (pred->toplevel - 1); i >= 0; i--)
    {
      curr = pred->next[i];
      while (key > curr->key)
	{
	  pred = curr;
	  curr = pred->next[i];
	}

      if (key == curr->key)
	{
	  nd = curr;
	  break;
	}
    }

  return nd;
}

sval_t
sl_optik_find(sl_intset_t* set, skey_t key)
{ 
  PARSE_START_TS(0);
  sl_node_t* nd = sl_optik_left_search(set, key);
  PARSE_END_TS(0, lat_parsing_get++);

  sval_t result = 0;
  if (nd != NULL)
    {
      result = nd->val;
    }
  return result;
}

static inline void
unlock_levels_down(sl_node_t** nodes, int low, int high)
{
  sl_node_t* old = NULL;
  int i;
  for (i = high; i >= low; i--)
    {
      if (old != nodes[i])
	{
	  optik_unlock(&nodes[i]->lock);
	}
      old = nodes[i];
    }
}

static inline void
unlock_levels_up(sl_node_t** nodes, int low, int high)
{
  sl_node_t* old = NULL;
  int i;
  for (i = low; i < high; i++)
    {
      if (old != nodes[i])
	{
	  optik_unlock(&nodes[i]->lock);
	}
      old = nodes[i];
    }
}


#define PAUSE_AND_RETRY() DO_PAUSE(); goto restart;

int
sl_optik_insert(sl_intset_t* set, skey_t key, sval_t val)
{
  sl_node_t* preds[OPTIK_MAX_MAX_LEVEL], *succs[OPTIK_MAX_MAX_LEVEL];
  optik_t predsv[OPTIK_MAX_MAX_LEVEL];
  sl_node_t* node_new = NULL;

  int toplevel = get_rand_level();
  int inserted_upto = 0;
  /* printf("++> inserting %zu\n", key); */

  NUM_RETRIES();
 restart:
  UPDATE_TRY();
  sl_node_t* node_found = sl_optik_search(set, key, preds, succs, predsv);
  if (node_found != NULL && !inserted_upto)
    {
      if (unlikely(node_new != NULL))
	{
#if GC == 1
	  ssmem_free(alloc, (void*) node_new);
#else
	  ssfree(node_new);
#endif
	}
      return 0;
    }

  if (node_new == NULL)
    {
      node_new = sl_new_simple_node(key, val, toplevel, 0);
    }

  sl_node_t* pred_prev = NULL;
  int i;
  for (i = inserted_upto; i < toplevel; i++)
    {
      sl_node_t* pred = preds[i];
      if (pred_prev != pred && (!optik_lock_version(&pred->lock, predsv[i])))
	{
	  sl_node_t* succ = succs[i];
	  if (node_is_unlinking(pred) || node_is_unlinking(succ) || pred->next[i] != succ)
	    {
	      unlock_levels_down(preds, inserted_upto, i);
	      inserted_upto = i;
	      PAUSE_AND_RETRY();
	    }
	}
      node_new->next[i] = pred->next[i];
      pred->next[i] = node_new;
      pred_prev = pred;
    }

  node_set_valid(node_new);
  unlock_levels_down(preds, inserted_upto, toplevel - 1);

  return 1;
}


sval_t
sl_optik_delete(sl_intset_t* set, skey_t key)
{
  sl_node_t* preds[OPTIK_MAX_MAX_LEVEL], *succs[OPTIK_MAX_MAX_LEVEL];
  optik_t predsv[OPTIK_MAX_MAX_LEVEL];
  int my_delete = 0;

  NUM_RETRIES();
 restart:
  UPDATE_TRY();
  sl_node_t* node_found = sl_optik_search(set, key, preds, succs, predsv);
  if (node_found == NULL)
    {
      return 0;
    }

  if (!my_delete)
    {
      if (node_is_unlinking(node_found))
	{
	  return 0;
	}
      else if (node_is_linking(node_found))
	{
	  PAUSE_AND_RETRY();
	}


      optik_lock(&node_found->lock);
      if (node_is_unlinking(node_found))
	{
	  optik_unlock(&node_found->lock);
	  return 0;
	}
      node_set_unlinking(node_found);
    }

  my_delete = 1;

  const int toplevel = node_found->toplevel;
  sl_node_t* pred_prev = NULL;
  int i;
  for (i = 0; i < toplevel; i++)
    {
      sl_node_t* pred = preds[i];
      if (pred_prev != pred && (!optik_lock_version(&pred->lock, predsv[i])))
	{
	  if (node_is_unlinking(pred) || pred->next[i] != succs[i])
	    {
	      unlock_levels_down(preds, 0, i);
	      PAUSE_AND_RETRY();
	    }
	}
      pred_prev = pred;
    }

  for (i = (node_found->toplevel - 1); i >= 0; i--)
    {
      preds[i]->next[i] = node_found->next[i];
    }

  optik_unlock(&node_found->lock);
  unlock_levels_down(preds, 0, toplevel - 1);

  sval_t val = node_found->val;
#if GC == 1
  ssmem_free(alloc, (void*) node_found);
#endif

  return val;
}
