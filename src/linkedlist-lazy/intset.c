/*
 * File:
 *   intset.c
 * Author(s):
 * Description:
 *   Linked list integer set operations
 *
 * Copyright (c) 2009-2010.
 *
 * intset.c is part of HIDDEN
 * 
 * HIDDEN is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "lazy.h"
#include "utils.h"

sval_t
set_contains_l(intset_l_t* set, skey_t key)
{
      return parse_find(set, key);
}

int
set_add_l(intset_l_t* set, skey_t key, sval_t val)
{  
      return parse_insert(set, key, val);
}

sval_t
set_remove_l(intset_l_t* set, skey_t key)
{
      return parse_delete(set, key);
}
