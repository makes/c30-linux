/* Default target hook functions.
   Copyright (C) 2003, 2004, 2005 Free Software Foundation, Inc.

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

extern void default_external_libcall (rtx);

extern enum machine_mode default_cc_modes_compatible (enum machine_mode,
						      enum machine_mode);

extern bool default_return_in_memory (tree, tree);

extern rtx default_expand_builtin_saveregs (void);
extern void default_setup_incoming_varargs (CUMULATIVE_ARGS *, enum machine_mode, tree, int *, int);
extern rtx default_builtin_setjmp_frame_value (void);
extern bool default_pretend_outgoing_varargs_named (CUMULATIVE_ARGS *);

extern enum machine_mode default_eh_return_filter_mode (void);
extern unsigned HOST_WIDE_INT default_shift_truncation_mask
  (enum machine_mode);

extern tree default_cxx_guard_type (void);
extern tree default_cxx_get_cookie_size (tree);

extern bool hook_pass_by_reference_must_pass_in_stack
  (CUMULATIVE_ARGS *, enum machine_mode mode, tree, bool);
extern bool hook_callee_copies_named
  (CUMULATIVE_ARGS *ca, enum machine_mode, tree, bool);

extern void default_unwind_emit (FILE *, rtx);

extern bool default_scalar_mode_supported_p (enum machine_mode);

/* These are here, and not in hooks.[ch], because not all users of
   hooks.h include tm.h, and thus we don't have CUMULATIVE_ARGS.  */

extern bool hook_bool_CUMULATIVE_ARGS_false (CUMULATIVE_ARGS *);
extern bool hook_bool_CUMULATIVE_ARGS_true (CUMULATIVE_ARGS *);

extern bool hook_bool_CUMULATIVE_ARGS_mode_tree_bool_false
  (CUMULATIVE_ARGS *, enum machine_mode, tree, bool);
extern bool hook_bool_CUMULATIVE_ARGS_mode_tree_bool_true
  (CUMULATIVE_ARGS *, enum machine_mode, tree, bool);
extern int hook_int_CUMULATIVE_ARGS_mode_tree_bool_0
  (CUMULATIVE_ARGS *, enum machine_mode, tree, bool);
