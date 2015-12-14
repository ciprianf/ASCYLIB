/*   
 *   File: intset.c
 *   Author: Vincent Gramoli <vincent.gramoli@sydney.edu.au>, 
 *  	     Vasileios Trigonakis <vasileios.trigonakis@epfl.ch>
 *   Description: 
 *   intset.c is part of ASCYLIB
 *
 * Copyright (c) 2014 Vasileios Trigonakis <vasileios.trigonakis@epfl.ch>,
 * 	     	      Tudor David <tudor.david@epfl.ch>
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

#include "lazy.h"
#include "utils.h"

sval_t
set_contains_l(intset_l_t* set, skey_t key)
{
      memalloc_unsafe_to_reclaim();
      sval_t r = parse_find(set, key);
      memalloc_safe_to_reclaim();
      return r;
}

int
set_add_l(intset_l_t* set, skey_t key, sval_t val)
{  
      memalloc_unsafe_to_reclaim();
      int r = parse_insert(set, key, val);
      memalloc_safe_to_reclaim();
      return r;
}

sval_t
set_remove_l(intset_l_t* set, skey_t key)
{
      memalloc_unsafe_to_reclaim();
      sval_t r = parse_delete(set, key);
      memalloc_safe_to_reclaim();
      return r;
}
