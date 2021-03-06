/* Set operations on pointers
   Copyright (C) 2004 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#ifndef POINTER_SET_H
#define POINTER_SET_H

struct pointer_set_t;

struct pointer_set_t *pointer_set_create (void);
void pointer_set_destroy (struct pointer_set_t *pset);

int pointer_set_contains (struct pointer_set_t *pset, void *p);
int pointer_set_insert (struct pointer_set_t *pset, void *p);

#endif  /* POINTER_SET_H  */
