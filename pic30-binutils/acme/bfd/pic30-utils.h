/*
** pic30-utils.h
**
** Copyright (c) 2004 Microchip Technology, Inc.
**
** This file contains utility macros and structures
** that are common to mulitple OMF formats.
*/

#ifndef _PIC30_UTILS_H
#define _PIC30_UTILS_H

/*****************************************************************************/

/*
** Macro to clear all section attributes
**
** We also clear parameters to address()
** and merge(), but not align(), reverse().
*/
#define PIC30_CLEAR_ALL_ATTR(sec) \
  { (sec)->flags = 0;             \
  (sec)->near = 0;                \
  (sec)->persistent = 0;          \
  (sec)->xmemory = 0;             \
  (sec)->ymemory = 0;             \
  (sec)->psv = 0;                 \
  (sec)->eedata = 0;              \
  (sec)->absolute = 0;            \
  (sec)->reverse = 0;             \
  (sec)->dma = 0;                 \
  (sec)->boot = 0;                \
  (sec)->secure = 0;              \
  (sec)->memory = 0;              \
  (sec)->heap = 0;                \
  (sec)->eds = 0;                 \
  (sec)->page = 0;                \
  (sec)->stack = 0;               \
  (sec)->vma = 0;                 \
  (sec)->lma = 0;                 \
  (sec)->entsize = 0;             \
  (sec)->auxflash = 0;}

/*
** Macros used to set section attributes
*/
#define PIC30_SET_CODE_ATTR(sec) \
  (sec)->flags |= (SEC_HAS_CONTENTS | SEC_LOAD | SEC_CODE | SEC_ALLOC);
#define PIC30_SET_DATA_ATTR(sec) \
  (sec)->flags |= (SEC_HAS_CONTENTS | SEC_LOAD | SEC_DATA | SEC_ALLOC);
#define PIC30_SET_BSS_ATTR(sec)   \
  { (sec)->flags |= SEC_ALLOC;      \
  (sec)->flags &= ~(SEC_LOAD | SEC_DATA | SEC_HAS_CONTENTS); }
#define PIC30_SET_PERSIST_ATTR(sec)  \
  { (sec)->persistent = 1;           \
  (sec)->flags |= SEC_ALLOC;         \
  (sec)->flags &= ~(SEC_LOAD | SEC_DATA); }
#define PIC30_SET_PSV_ATTR(sec) \
  { (sec)->psv = 1;             \
  (sec)->flags |= (SEC_HAS_CONTENTS | SEC_ALLOC | SEC_LOAD | SEC_READONLY); }
#define PIC30_SET_EEDATA_ATTR(sec) \
  { (sec)->eedata = 1;             \
  (sec)->flags |= (SEC_HAS_CONTENTS | SEC_ALLOC | SEC_LOAD); }
#define PIC30_SET_MEMORY_ATTR(sec) \
  { (sec)->memory = 1;             \
  (sec)->flags |= (SEC_HAS_CONTENTS | SEC_ALLOC); }
#define PIC30_SET_HEAP_ATTR(sec) \
  { (sec)->heap = 1;             \
  (sec)->flags |= SEC_ALLOC;     \
  (sec)->flags &= ~(SEC_LOAD | SEC_DATA | SEC_HAS_CONTENTS); }
#define PIC30_SET_STACK_ATTR(sec) \
  { (sec)->stack = 1;            \
  (sec)->flags |= SEC_ALLOC;     \
  (sec)->flags &= ~(SEC_LOAD | SEC_DATA | SEC_HAS_CONTENTS); }
#define PIC30_SET_AUXFLASH_ATTR(sec) \
  {(sec)->auxflash = 1;              \
   (sec)->flags |= (SEC_HAS_CONTENTS | SEC_ALLOC | SEC_LOAD);}

#define PIC30_SET_ABSOLUTE_ATTR(sec) \
  (sec)->absolute = 1;
#define PIC30_SET_NEAR_ATTR(sec) \
  (sec)->near = 1;
#define PIC30_SET_XMEMORY_ATTR(sec) \
  (sec)->xmemory = 1;
#define PIC30_SET_YMEMORY_ATTR(sec) \
  (sec)->ymemory = 1;
#define PIC30_SET_REVERSE_ATTR(sec) \
  (sec)->reverse = 1;
#define PIC30_SET_DMA_ATTR(sec) \
  (sec)->dma = 1;
#define PIC30_SET_BOOT_ATTR(sec) \
  (sec)->boot = 1;
#define PIC30_SET_SECURE_ATTR(sec) \
  (sec)->secure = 1;
#define PIC30_SET_NOLOAD_ATTR(sec)  \
  { (sec)->flags |= SEC_NEVER_LOAD; \
    (sec)->flags &= ~ SEC_LOAD; }
#define PIC30_SET_MERGE_ATTR(sec) \
  (sec)->flags |= SEC_MERGE;
#define PIC30_SET_INFO_ATTR(sec) \
  { (sec)->flags |= SEC_DEBUGGING; \
    (sec)->flags &= ~SEC_ALLOC; }
#define PIC30_SET_EDS_ATTR(sec) \
  (sec)->eds = 1;
#define PIC30_SET_PAGE_ATTR(sec) \
  (sec)->page = 1;

/* UNORDERED is used internally by the assembler
   and is not encoded in the object file */
#define PIC30_SET_UNORDERED_ATTR(sec) \
  (sec)->unordered = 1;

/*
** Macros used to query section attributes
*/
#define PIC30_IS_CODE_ATTR(sec) \
  (((sec)->flags & (SEC_CODE | SEC_ALLOC)) == (SEC_CODE | SEC_ALLOC))
#define PIC30_IS_DATA_ATTR(sec) \
  ((((sec)->flags & (SEC_DATA | SEC_ALLOC)) == (SEC_DATA | SEC_ALLOC)) && \
   ((sec)->memory !=1))
#define PIC30_IS_BSS_ATTR(sec) \
  ((((sec)->flags & (SEC_ALLOC|SEC_LOAD|SEC_CODE|SEC_DATA|SEC_HAS_CONTENTS)) == SEC_ALLOC) && \
   ((sec)->persistent != 1) && ((sec)->memory !=1) && \
   ((sec)->heap != 1) && ((sec)->stack !=1))
#define PIC30_IS_PERSIST_ATTR(sec) \
  ((((sec)->flags & (SEC_ALLOC|SEC_LOAD|SEC_CODE|SEC_DATA|SEC_HAS_CONTENTS)) == SEC_ALLOC) && \
   ((sec)->persistent == 1))
#define PIC30_IS_PSV_ATTR(sec) \
  (((sec)->psv == 1) || (((sec)->flags & SEC_READONLY) == SEC_READONLY))
#define PIC30_IS_EEDATA_ATTR(sec) \
  ((sec)->eedata == 1)
#define PIC30_IS_MEMORY_ATTR(sec) \
  ((((sec)->flags & (SEC_ALLOC|SEC_LOAD|SEC_CODE|SEC_DATA)) == SEC_ALLOC) && \
  ((sec)->memory == 1))
#define PIC30_IS_HEAP_ATTR(sec) \
  ((((sec)->flags & (SEC_ALLOC|SEC_LOAD|SEC_CODE|SEC_DATA|SEC_HAS_CONTENTS)) == SEC_ALLOC) && \
  ((sec)->heap == 1))
#define PIC30_IS_STACK_ATTR(sec) \
  ((((sec)->flags & (SEC_ALLOC|SEC_LOAD|SEC_CODE|SEC_DATA|SEC_HAS_CONTENTS)) == SEC_ALLOC) && \
  ((sec)->stack == 1))
#define PIC30_IS_AUXFLASH_ATTR(sec) \
  (((sec)->auxflash == 1) && (((sec)->flags & SEC_ALLOC) == SEC_ALLOC))

#define PIC30_IS_ABSOLUTE_ATTR(sec) \
  ((sec)->absolute == 1)
#define PIC30_IS_NEAR_ATTR(sec) \
  ((sec)->near == 1)
#define PIC30_IS_XMEMORY_ATTR(sec) \
  ((sec)->xmemory == 1)
#define PIC30_IS_YMEMORY_ATTR(sec) \
  ((sec)->ymemory == 1)
#define PIC30_IS_REVERSE_ATTR(sec) \
  ((sec)->reverse == 1)
#define PIC30_IS_DMA_ATTR(sec) \
  ((sec)->dma == 1)
#define PIC30_IS_BOOT_ATTR(sec) \
  ((sec)->boot == 1)
#define PIC30_IS_SECURE_ATTR(sec) \
  ((sec)->secure == 1)
#define PIC30_IS_NOLOAD_ATTR(sec) \
  (((sec)->flags & SEC_NEVER_LOAD) == SEC_NEVER_LOAD)
#define PIC30_IS_MERGE_ATTR(sec) \
  (((sec)->flags & SEC_MERGE) == SEC_MERGE)
#define PIC30_IS_INFO_ATTR(sec) \
  (((sec)->flags & SEC_DEBUGGING) == SEC_DEBUGGING)
#define PIC30_IS_EDS_ATTR(sec) \
  ((sec)->eds == 1)
#define PIC30_IS_PAGE_ATTR(sec) \
  ((sec)->page == 1)

/* UNORDERED is used internally by the assembler
   and is not encoded in the object file */
#define PIC30_IS_UNORDERED_ATTR(sec) \
  ((sec)->unordered == 1)
#define PIC30_IS_EXTERNAL_ATTR(sec) \
  (0)

#define PIC30_IS_LOCAL_DATA(sec)       \
  ((!PIC30_IS_EDS_ATTR(sec) &&         \
      (PIC30_IS_BSS_ATTR(sec) ||       \
       PIC30_IS_DATA_ATTR(sec) ||      \
       PIC30_IS_PERSIST_ATTR(sec))))

/*
** Macros used for access CodeGuard data structures
*/
#define NUM_BOOT_ACCESS_SLOTS 32
#define NUM_SECURE_ACCESS_SLOTS 32

/*
** enums for array indexing
**
** wrong:    array[BOOT][RAM]
** correct:  array[BOOTx][RAMx]
*/
enum { BOOTx = 0, SECUREx, GENERALx, SEGMENTS };
enum { RAMx  = 0, FLASHx,  EEPROMx, MEMORIES };

#define BOOT_IS_ACTIVE(mem_index)  \
  (( end_address[BOOTx][mem_index] != (bfd_vma) ~1) && \
   (base_address[BOOTx][mem_index] <= \
     end_address[BOOTx][mem_index]))

#define SECURE_IS_ACTIVE(mem_index)  \
  (( end_address[SECUREx][mem_index] != (bfd_vma) ~1) && \
   (base_address[SECUREx][mem_index] <= \
     end_address[SECUREx][mem_index]))

#define CRT0_KEY "__resetPRI"
#define CRT1_KEY "__resetALT"

/*****************************************************************************/

/*
** Define some structures for the Handle hash table.
**
** This is used by the linker to collect and process
** handle relocations.
*/
struct handle_hash_entry
{
  struct bfd_hash_entry root;
  int index;     /* jump table index    */
  char *sym;     /* name of jump target */
  flagword flags;
};

struct handle_hash_table
{
  struct bfd_hash_table table;
  int num;  /* # of entries */
};

#define handle_hash_lookup(t, string, create, copy) \
  ((struct handle_hash_entry *) \
   bfd_hash_lookup (&(t)->table, (string), (create), (copy)))

/*
** Define some structures for the Undefined Symbol hash table.
**
** This is used by the linker to collect object signatures
** for undefined symbols. Signatures are used to help
** choose between multiple definitions of a symbol
** in archive files (aka libraries).
**
** As multiple objects refer to the same symbol,
** a composite signature is created. The most recent file
** to reference is saved, to facilitate error reporting.
*/
#define PIC30_UNDEFSYM_INIT 20
struct pic30_undefsym_entry {
  struct bfd_hash_entry root;
  bfd *most_recent_reference;
  int external_options_mask;
  int options_set;
};

struct pic30_undefsym_table {
  struct bfd_hash_table table;
  int num;  /* # of entries */
};

#define pic30_undefsym_lookup(t, string, create, copy) \
  ((struct pic30_undefsym_entry *) \
   bfd_hash_lookup (&(t)->table, (string), (create), (copy)))

extern struct pic30_undefsym_table *undefsyms;

extern struct bfd_hash_entry *pic30_undefsym_newfunc
  PARAMS ((struct bfd_hash_entry *, struct bfd_hash_table *, const char *));

extern void pic30_undefsym_traverse
  PARAMS ((struct pic30_undefsym_table *,
           bfd_boolean (*func) PARAMS ((struct bfd_hash_entry *, PTR)), PTR));

extern struct pic30_undefsym_table *pic30_undefsym_init
  PARAMS ((void));


/* Section Lists */
struct pic30_section
{
  struct pic30_section *next;
  unsigned int attributes;
  PTR *file;
  asection *sec;
};

/*****************************************************************************/

#endif
