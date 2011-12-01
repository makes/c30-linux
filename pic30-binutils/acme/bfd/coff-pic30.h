#ifndef __COFF_PIC30_H__
#define __COFF_PIC30_H__

extern int pic30_adjustable_against_section (unsigned short);

#define PIC30_BITS_PER_PROGRAM_ADDRESS 24

#define PIC30_TBLOFFSET(value) ((value) & 0xFFFF)
#define PIC30_PSVOFFSET(value) (((value) | 0x8000) & 0xFFFF)
#define PIC30_EDSOFFSET(value) (((value) | 0x8000) & 0xFFFF)
#define PIC30_TBLPAGE(value)  ((value) >> 16)
#define PIC30_PSVPAGE(value)  ((value) >> 15)
#define PIC30_HANDLE(value) ((value) & 0xFFFF)
#define PIC30_PSVPTR(value) ((((value) & 0xFFFF8000) << 1) | \
        (((value) & 0x7FFF) << 1) | 0x1 )

#define PIC30_DATA_WINDOW_BASE 0x8000

#define PIC30_IS_EVEN(value) (((value) & 0x1) == 0)
#define PIC30_IS_VALID_DO_OFFSET(lit) \
        (((lit) >= -32768) && ((lit) < 32768) && \
         ((lit) != -1) && ((lit) != 0) && ((lit) != 1))
#define PIC30_IS_1_BIT_LITERAL(lit) (((lit) >= 0) && ((lit) < 2))
#define PIC30_IS_3_BIT_LITERAL(lit) (((lit) >= 0) && ((lit) < 8))
#define PIC30_IS_4_BIT_LITERAL(lit) (((lit) >= -8) && ((lit) < 8))
#define PIC30_IS_4_BIT_UNSIGNED_LITERAL(lit) (((lit) >= 0) && ((lit) < 16))
#define PIC30_IS_5_BIT_LITERAL(lit) (((lit) >= -16) && ((lit) < 16))
#define PIC30_IS_5_BIT_UNSIGNED_LITERAL(lit) (((lit) >= 0) && ((lit) < 32))
#define PIC30_IS_DSP6_BIT_LITERAL(lit) (((lit) >= -16) && ((lit) <= 16))
#define PIC30_IS_BYTE_VALUE(lit) (((lit) >= -128) && ((lit) < 256))
#define PIC30_IS_10_BIT_UNSIGNED_LITERAL(lit) ((0 <= (lit)) && ((lit) <= 1023))
#define PIC30_IS_14_BIT_UNSIGNED_LITERAL(lit) (((lit) >= 0) && ((lit) < 16384))
#define PIC30_IS_15_BIT_UNSIGNED_LITERAL(lit) (((lit) >= 0) && ((lit) < 32768))
#define PIC30_IS_16_BIT_LITERAL(lit) (((lit) >= -32768) && ((lit) < 65536))
#define PIC30_IS_16_BIT_UNSIGNED_LITERAL(lit) (((lit) >= 0) && ((lit) < 65536))
#define PIC30_IS_13_BIT_FILE_REG(lit) (((lit) >= 0) && ((lit) < 8192))
#define PIC30_IS_BYTE_VALUE_UNSIGNED(lit) (((lit) >= 0) && ((lit) < 256))

#define FILE_REG_LIMIT 8192


#define PIC30_SECTION_IN_PROGRAM_MEMORY(sec) \
  ((sec->flags & SEC_CODE) || (sec->auxflash == 1))
#define PIC30_SECTION_IN_PSV_MEMORY(sec) \
  ((sec->psv || ((sec->flags & SEC_READONLY) == SEC_READONLY)))
#define PIC30_SECTION_IN_DATA_MEMORY(sec) \
  (PIC30_IS_BSS_ATTR(sec) ||              \
   PIC30_IS_DATA_ATTR(sec) ||             \
   PIC30_IS_PERSIST_ATTR(sec) ||          \
   PIC30_IS_STACK_ATTR(sec) ||            \
   PIC30_IS_HEAP_ATTR(sec))

#define PIC30_SECTION_IS_BSS_TYPE(sec) \
  (((sec->flags & (SEC_ALLOC | SEC_DATA | SEC_READONLY | SEC_CODE)) \
     == SEC_ALLOC) && (sec->psv == 0) && (sec->eedata == 0) \
     && (sec->auxflash ==0))
#define PIC30_SECTION_IS_DATA_TYPE(sec) \
  ((sec->flags & SEC_DATA))

/*
** The following macros are used by
** objdump and the disassembler.
*/

int pic30_display_as_program_memory_p(asection *sec);
int pic30_display_as_data_memory_p(asection *sec);
int pic30_display_as_readonly_memory_p(asection *sec);

/* CODE section */
#define PIC30_DISPLAY_AS_PROGRAM_MEMORY(sec) \
   pic30_display_as_program_memory_p(sec)

/* linked READONLY section */
#define PIC30_DISPLAY_AS_READONLY_MEMORY(sec) \
  pic30_display_as_readonly_memory_p(sec)

/* BSS, DATA, or unlinked READONLY section */ 
#define PIC30_DISPLAY_AS_DATA_MEMORY(sec) \
  pic30_display_as_data_memory_p(sec)

#define PIC30_CAN_PRINT_IN_PROGRAM_MEMORY(addr) \
  ((addr % 4) != 3)

#define PIC30_CAN_PRINT_IN_READONLY_MEMORY(addr) \
  ((addr % 4) < 2)

#define PIC30_CAN_PRINT_IN_DATA_MEMORY(addr) \
  ((addr % 2) != 1)

#define TARGET_BFD_INIT \
  { extern void pic30_update_resource(const char *resource); \
    extern char *program_name;                               \
                                                             \
    pic30_update_resource(program_name); }

#endif

