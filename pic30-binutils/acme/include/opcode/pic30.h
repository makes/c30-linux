#ifndef __OPCODE_PIC30_H__
#define __OPCODE_PIC30_H__

/*
 * pic30.h -- Opcode/operand structure header for Microchip dsPIC30
 * Copyright (C) 2000 Free Software Foundation, Inc.
 * Contributed by Microchip Corporation.
 * Written by Tracy A. Kuhrt
 * Modified by John Elliott (john.elliott@microchip.com)
 *
 * This file is part of GDB, GAS, and the GNU binutils.
 *
 * GDB, GAS, and the GNU binutils are free software; you can redistribute
 * them and/or modify them under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either version
 * 1, or (at your option) any later version.
 *
 * GDB, GAS, and the GNU binutils are distributed in the hope that they
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this file; see the file COPYING.  If not, write to the Free
 * Software Foundation, 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#include "ansidecl.h"

/******************************************************************************
 * EXPRESSION MODIFIERS
 ******************************************************************************/
enum pic30_modifier
{
  PIC30_NO_MODIFIER_FOUND,
  PIC30_TBLOFFSET_FOUND,
  PIC30_PSVOFFSET_FOUND,
  PIC30_TBLPAGE_FOUND,
  PIC30_PSVPAGE_FOUND,
  PIC30_HANDLE_FOUND,
  PIC30_PADDR_FOUND,
  PIC30_DMAOFFSET_FOUND,
  PIC30_DMAPAGE_FOUND,
  PIC30_PSVPTR_FOUND,
  PIC30_BOOT_FOUND,
  PIC30_SECURE_FOUND,
  PIC30_EDSPAGE_FOUND,
  PIC30_EDSOFFSET_FOUND
};

/******************************************************************************/

struct pic30_operand_value
{
  long value;
  unsigned long type;           /* holds operatorT */
  long optional_value;
  enum pic30_modifier modifier;
};

/******************************************************************************/

struct relocation_info
{
  int value;
  unsigned char pc_relative;
  int fix_size;

  /*
   * If this function pointer is non-NULL, the function will be called to
   * check to see if the pic30_operand_value structure passed is a valid value.
   */
  unsigned char (*is_match) PARAMS
    ((const struct pic30_operand_value * check));
};

/******************************************************************************/

struct pic30_operand
{
  /* Number of bits in the operand */
  unsigned short bits;

  /* Number of bits to shift operand in the instruction */
  unsigned short shift;

  /* The type of the operand */
  unsigned long type;

  /* Whether or not the operand is an immediate value */
  unsigned char immediate;

  /* The default relocation type */
  unsigned long relocation;

  /*
   * If this function pointer is non-NULL, the function will be called to
   * check to see if the pic30_operand_value structure passed is a valid
   * value for this operand; otherwise, just check the types to ensure
   * that they are the same.
   */
  unsigned char (*is_match) PARAMS
    ((const struct pic30_operand_value * check));

  /* info string for the is_match function, may be NULL */
  char *info_string;

  unsigned long (*insert) PARAMS
    ((unsigned long insn, unsigned long flags,
      const struct pic30_operand * operand,
      const struct pic30_operand_value * operand_value,
      char **error_msg));

  char *(*extract) PARAMS ((unsigned long insn,
                            struct disassemble_info *info,
                            unsigned long flags,
                            const struct pic30_operand * operand,
                            unsigned char *err));
};

/******************************************************************************/

/* REV_A enables support for dsPIC Rev. A silicon */
#define REV_A 1
enum pic30_baseop
{
  OP_UNKNOWN = 0,
  OP_ADDAB,
  OP_ADDAC,
  OP_ADDAC_PS,
  OP_ADDCLS_B,
  OP_ADDCLS_W,
  OP_ADDCLW_B,
  OP_ADDCLW_W,
  OP_ADDCWFF_B,
  OP_ADDCWFF_W,
  OP_ADDCWFW_B,
  OP_ADDCWFW_W,
  OP_ADDC_B,
  OP_ADDC_W,
  OP_ADDLS_B,
  OP_ADDLS_W,
  OP_ADDLW_B,
  OP_ADDLW_W,
  OP_ADDWFF_B,
  OP_ADDWFF_W,
  OP_ADDWFW_B,
  OP_ADDWFW_W,
  OP_ADD_B,
  OP_ADD_W,
  OP_ANDLS_B,
  OP_ANDLS_W,
  OP_ANDLW_B,
  OP_ANDLW_W,
  OP_ANDWFF_B,
  OP_ANDWFF_W,
  OP_ANDWFW_B,
  OP_ANDWFW_W,
  OP_AND_B,
  OP_AND_W,
  OP_ASRFF_B,
  OP_ASRFF_W,
  OP_ASRFW_B,
  OP_ASRFW_W,
  OP_ASRK_W,
  OP_ASRW_W,
  OP_ASR_B,
  OP_ASR_W,
  OP_BCLRF_B,
  OP_BCLR_B,
  OP_BCLR_W,
  OP_BRA,
  OP_BRAW,
  OP_BRAWE,
  OP_BRA_CC,
  OP_BRA_DSP,
  OP_BSETF_B,
  OP_BSET_B,
  OP_BSET_W,
  OP_BSW_C,
  OP_BSW_Z,
  OP_BTGF_B,
  OP_BTG_B,
  OP_BTG_W,
  OP_BTSCF_B,
  OP_BTSC_B,
  OP_BTSC_W,
  OP_BTSSF_B,
  OP_BTSS_B,
  OP_BTSS_W,
  OP_BTSTF_B,
  OP_BTSTSF_B,
  OP_BTSTS_B,
  OP_BTSTS_C,
  OP_BTSTS_Z,
  OP_BTSTW_C,
  OP_BTSTW_Z,
  OP_BTST_B,
  OP_BTST_C,
  OP_BTST_Z,
  OP_CALL,
  OP_CALLW,
  OP_CLRAC,
  OP_CLRAC_A,
  OP_CLRAC_WB,
  OP_CLRAC_X,
  OP_CLRAC_XWB,
  OP_CLRAC_XY,
  OP_CLRAC_Y,
  OP_CLRAC_YWB,
  OP_CLRF_B,
  OP_CLRF_W,
  OP_CLRWDT,
  OP_CLRW_B,
  OP_CLRW_W,
  OP_CLR_B,
  OP_CLR_W,
  OP_COMFF_B,
  OP_COMFF_W,
  OP_COMFW_B,
  OP_COMFW_W,
  OP_COM_B,
  OP_COM_W,
  OP_CP0F_B,
  OP_CP0F_W,
  OP_CP0_B,
  OP_CP0_W,
  OP_CPBF_B,
  OP_CPBF_W,
  OP_CPBLS_B,
  OP_CPBLS8_B,
  OP_CPBLS_W,
  OP_CPBLS8_W,
  OP_CPB_B,
  OP_CPB_W,
#if REV_A
  OP_CPFSEQ_B,
  OP_CPFSEQ_W,
  OP_CPFSGT_B,
  OP_CPFSGT_W,
  OP_CPFSLT_B,
  OP_CPFSLT_W,
  OP_CPFSNE_B,
  OP_CPFSNE_W,
#endif
  OP_CPWSEQ_B,
  OP_CPWBEQ_B,
  OP_CPWSEQ_E_B,
  OP_CPWSEQ_W,
  OP_CPWBEQ_W,
  OP_CPWSEQ_E_W,
  OP_CPWSGT_B,
  OP_CPWBGT_B,
  OP_CPWSGT_E_B,
  OP_CPWSGT_W,
  OP_CPWBGT_W,
  OP_CPWSGT_E_W,
  OP_CPWSLT_B,
  OP_CPWBLT_B,
  OP_CPWSLT_E_B,
  OP_CPWSLT_W,
  OP_CPWBLT_W,
  OP_CPWSLT_E_W,
  OP_CPWSNE_B,
  OP_CPWBNE_B,
  OP_CPWSNE_E_B,
  OP_CPWSNE_W,
  OP_CPWBNE_W,
  OP_CPWSNE_E_W,
  OP_CPF_B,
  OP_CPF_W,
  OP_CPLS_B,
  OP_CPLS_W,
  OP_CPLS8_B,
  OP_CPLS8_W,
  OP_CP_B,
  OP_CP_W,
  OP_DAW_B,
  OP_DEC2FF_B,
  OP_DEC2FF_W,
  OP_DEC2FW_B,
  OP_DEC2FW_W,
  OP_DEC2_B,
  OP_DEC2_W,
  OP_DECFF_B,
  OP_DECFF_W,
  OP_DECFW_B,
  OP_DECFW_W,
#if REV_A
  OP_DECSNZF_B,
  OP_DECSNZF_W,
  OP_DECSNZW_B,
  OP_DECSNZW_W,
  OP_DECSZF_B,
  OP_DECSZF_W,
  OP_DECSZW_B,
  OP_DECSZW_W,
#endif
  OP_DEC_B,
  OP_DEC_W,
  OP_DISI,
  OP_DIVF,
  OP_DIVS_D,
  OP_DIVS_W,
  OP_DIVU_D,
  OP_DIVU_W,
  OP_DO,
  OP_DOW,
  OP_DUMMY,
  OP_ED,
  OP_EDAC,
  OP_EXCH,
  OP_FBCL,
  OP_FF1L,
  OP_FF1R,
  OP_GOTO,
  OP_GOTOW,
  OP_INC2FF_B,
  OP_INC2FF_W,
  OP_INC2FW_B,
  OP_INC2FW_W,
  OP_INC2_B,
  OP_INC2_W,
  OP_INCFF_B,
  OP_INCFF_W,
  OP_INCFW_B,
  OP_INCFW_W,
#if REV_A
  OP_INCSNZF_B,
  OP_INCSNZF_W,
  OP_INCSNZW_B,
  OP_INCSNZW_W,
  OP_INCSZF_B,
  OP_INCSZF_W,
  OP_INCSZW_B,
  OP_INCSZW_W,
#endif
  OP_INC_B,
  OP_INC_W,
  OP_IORLS_B,
  OP_IORLS_W,
  OP_IORLW_B,
  OP_IORLW_W,
  OP_IORWFF_B,
  OP_IORWFF_W,
  OP_IORWFW_B,
  OP_IORWFW_W,
  OP_IOR_B,
  OP_IOR_W,
  OP_ITCH,
  OP_LAC,
  OP_LAC_PS,
  OP_LDDW,
  OP_LDW,
  OP_LDWLO_B,
  OP_LDWLO_W,
  OP_LNK,
  OP_LSRFF_B,
  OP_LSRFF_W,
  OP_LSRFW_B,
  OP_LSRFW_W,
  OP_LSRK_W,
  OP_LSRW_W,
  OP_LSR_B,
  OP_LSR_W,
  OP_MAC,
  OP_MAC_A,
  OP_MAC_WB,
  OP_MAC_X,
  OP_MAC_XWB,
  OP_MAC_XY,
  OP_MAC_Y,
  OP_MAC_YWB,
  OP_MOVFF_B,
  OP_MOVFF_W,
  OP_MOVFW_B,
  OP_MOVFW_W,
  OP_MOVLW_B,
  OP_MOVLW_W,
  OP_MOVL_W,
  OP_MOVSAC,
  OP_MOVSAC_A,
  OP_MOVSAC_WB,
  OP_MOVSAC_X,
  OP_MOVSAC_XWB,
  OP_MOVSAC_XY,
  OP_MOVSAC_Y,
  OP_MOVSAC_YWB,
  OP_MOVWF_B,
  OP_MOVWF_W,
  OP_MOVPAG,
  OP_MOVPAG_W,
  OP_MOV_B,
  OP_MOV_W,
  OP_MPY,
  OP_MPYN,
  OP_MPYN_A,
  OP_MPYN_X,
  OP_MPYN_Y,
  OP_MPY_A,
  OP_MPY_X,
  OP_MPY_Y,
  OP_MSC,
  OP_MSC_A,
  OP_MSC_WB,
  OP_MSC_X,
  OP_MSC_XWB,
  OP_MSC_XY,
  OP_MSC_Y,
  OP_MSC_YWB,
  OP_MULLS_SU,
  OP_MULLSW_SU,
  OP_MULLS_SU_ACC,
  OP_MULLS_UU,
  OP_MULLSW_UU,
  OP_MULLS_UU_ACC,
  OP_MULWF_B,
  OP_MULWF_W,
  OP_MUL_SS,
  OP_MUL_SS_ACC,
  OP_MULW_SS,
  OP_MUL_SU,
  OP_MUL_SU_ACC,
  OP_MULW_SU,
  OP_MUL_US,
  OP_MUL_US_ACC,
  OP_MULW_US,
  OP_MUL_UU,
  OP_MULW_UU,
  OP_MUL_UU_ACC,
  OP_NEGAB,
  OP_NEGFF_B,
  OP_NEGFF_W,
  OP_NEGFW_B,
  OP_NEGFW_W,
  OP_NEG_B,
  OP_NEG_W,
  OP_NOP,
  OP_NOPR,
  OP_POPF,
  OP_POP_D,
  OP_POP_W,
  OP_PUSHF,
  OP_PUSH_D,
  OP_PUSH_W,
  OP_PWRSAV,
  OP_RCALL,
  OP_RCALLW,
  OP_REPEAT,
  OP_REPEATW,
  OP_RESET,
  OP_RETFIE,
  OP_RETLW_B,
  OP_RETLW_W,
  OP_RETURN,
  OP_RLCFF_B,
  OP_RLCFF_W,
  OP_RLCFW_B,
  OP_RLCFW_W,
  OP_RLC_B,
  OP_RLC_W,
  OP_RLNCFF_B,
  OP_RLNCFF_W,
  OP_RLNCFW_B,
  OP_RLNCFW_W,
  OP_RLNC_B,
  OP_RLNC_W,
  OP_RRCFF_B,
  OP_RRCFF_W,
  OP_RRCFW_B,
  OP_RRCFW_W,
  OP_RRC_B,
  OP_RRC_W,
  OP_RRNCFF_B,
  OP_RRNCFF_W,
  OP_RRNCFW_B,
  OP_RRNCFW_W,
  OP_RRNC_B,
  OP_RRNC_W,
  OP_SAC,
  OP_SAC_PS,
  OP_SAC_R,
  OP_SAC_R_PS,
  OP_SCRATCH,
  OP_SETF_B,
  OP_SETF_W,
  OP_SETW_B,
  OP_SETW_W,
  OP_SET_B,
  OP_SET_W,
  OP_SE_W,
  OP_SFTACK,
  OP_SFTACW,
  OP_SLFF_B,
  OP_SLFF_W,
  OP_SLFW_B,
  OP_SLFW_W,
  OP_SLK_W,
  OP_SLW_W,
  OP_SL_B,
  OP_SL_W,
  OP_SQR,
  OP_SQRAC,
  OP_SQRAC_A,
  OP_SQRAC_X,
  OP_SQRAC_Y,
  OP_SQR_A,
  OP_SQR_X,
  OP_SQR_Y,
  OP_STDW,
  OP_STW,
  OP_STWLO_B,
  OP_STWLO_W,
  OP_SUBAB,
  OP_SUBBLS_B,
  OP_SUBBLS_W,
  OP_SUBBLW_B,
  OP_SUBBLW_W,
  OP_SUBBRLS_B,
  OP_SUBBRLS_W,
  OP_SUBBRWFF_B,
  OP_SUBBRWFF_W,
  OP_SUBBRWFW_B,
  OP_SUBBRWFW_W,
  OP_SUBBR_B,
  OP_SUBBR_W,
  OP_SUBBWFF_B,
  OP_SUBBWFF_W,
  OP_SUBBWFW_B,
  OP_SUBBWFW_W,
  OP_SUBB_B,
  OP_SUBB_W,
  OP_SUBLS_B,
  OP_SUBLS_W,
  OP_SUBLW_B,
  OP_SUBLW_W,
  OP_SUBRLS_B,
  OP_SUBRLS_W,
  OP_SUBRWFF_B,
  OP_SUBRWFF_W,
  OP_SUBRWFW_B,
  OP_SUBRWFW_W,
  OP_SUBR_B,
  OP_SUBR_W,
  OP_SUBWFF_B,
  OP_SUBWFF_W,
  OP_SUBWFW_B,
  OP_SUBWFW_W,
  OP_SUB_B,
  OP_SUB_W,
  OP_SWAP_B,
  OP_SWAP_W,
  OP_TBLRDH_B,
  OP_TBLRDH_W,
  OP_TBLRDL_B,
  OP_TBLRDL_W,
  OP_TBLWTH_B,
  OP_TBLWTH_W,
  OP_TBLWTL_B,
  OP_TBLWTL_W,
  OP_TRAP,
  OP_ULNK,
  OP_XORLS_B,
  OP_XORLS_W,
  OP_XORLW_B,
  OP_XORLW_W,
  OP_XORWFF_B,
  OP_XORWFF_W,
  OP_XORWFW_B,
  OP_XORWFW_W,
  OP_XOR_B,
  OP_XOR_W,
  OP_ZE_W,
};

#define MAX_OPERANDS 8
#define MAX_WORDS_PER_INSN 2

/* A structure defining the opcodes for PIC30 */
struct pic30_opcode
{
  /* The opcode name */
  const char *name;
  enum pic30_baseop baseop;
  /*
   * The number of words in this instruction.
   */
  unsigned short number_of_words;
  /*
   * The opcode itself.  Those bits which will be filled in with operands
   * are zeroes.
   */
  unsigned long opcode[MAX_WORDS_PER_INSN];
  /*
   * The opcode mask.  This is used by the disassembler.  This is a mask
   * containing ones indicating those bits which must match the opcode
   * field, and zeroes indicating those bits which need not match (and are
   * presumably filled in by operands).
   */
  unsigned long mask;
  /* The number of operands for this opcode */
  unsigned short number_of_operands;
  /* The operands for this opcode.  Points to one of the operands. */
  unsigned long operands[MAX_OPERANDS];
  /* Different flags needed for this opcode */
  unsigned long flags;
};
/******************************************************************************/
extern const struct pic30_opcode pic30_opcodes[];
extern const int pic30_num_opcodes;
#endif /* __OPCODE_PIC30_H__ */
