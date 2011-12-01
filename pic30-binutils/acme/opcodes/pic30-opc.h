#ifndef __PIC30_OPC_H__
#define __PIC30_OPC_H__

/* pic30-opc.h -- This file contains defines and externs for the
 * opcodes/operands for Microchip dsPIC30
 * Copyright (C) 2000 Free Software Foundation, Inc.
 * Contributed by Microchip Corporation.
 * Written by Tracy A. Kuhrt
 * Modified by John Elliott (john.elliott@microchip.com)
 *
 * This file is part of GAS, the GNU Assembler.
 *
 * GAS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * GAS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with GAS; see the file COPYING.  If not, write to the Free Software
 * Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/******************************************************************************
 *
 * The following are mode flags for use with operands
 *
 ******************************************************************************/

#define NO_MODE  (0)
#define P_OR_Q   (1 << 0)
#define G_OR_H   (1 << 2)

#define MODE_MASK (0xF)

/******************************************************************************
 *
 * The following are mode values for insertion into and extraction from
 * instructions.
 *
 ******************************************************************************/

#define P_OR_Q_REGISTER_DIRECT            (0x0)
#define P_OR_Q_REGISTER_INDIRECT          (0x1)
#define P_OR_Q_REGISTER_POST_DECREMENT    (0x2)
#define P_OR_Q_REGISTER_POST_INCREMENT    (0x3)
#define P_OR_Q_REGISTER_PRE_DECREMENT     (0x4)
#define P_OR_Q_REGISTER_PRE_INCREMENT     (0x5)

#define G_OR_H_REGISTER_DIRECT            (0x0)
#define G_OR_H_REGISTER_INDIRECT          (0x1)
#define G_OR_H_REGISTER_POST_DECREMENT    (0x2)
#define G_OR_H_REGISTER_POST_INCREMENT    (0x3)
#define G_OR_H_REGISTER_PRE_DECREMENT     (0x4)
#define G_OR_H_REGISTER_PRE_INCREMENT     (0x5)
#define G_OR_H_REGISTER_WITH_DISPLACEMENT (0x6)

/******************************************************************************
 *
 * The following are flags for the different type of operands that exist
 *
 ******************************************************************************/

#define OPND_UNKNOWN                             (1 << 4)
#define OPND_REGISTER_DIRECT                     (1 << 5)
#define OPND_REGISTER_INDIRECT                   (1 << 6)
#define OPND_REGISTER_POST_DECREMENT             (1 << 7)
#define OPND_REGISTER_POST_INCREMENT             (1 << 8)
#define OPND_REGISTER_PRE_DECREMENT              (1 << 9)
#define OPND_REGISTER_PRE_INCREMENT              (1 << 10)
#define OPND_REGISTER_WITH_DISPLACEMENT          (1 << 11)
#define OPND_REGISTER_PLUS_LITERAL               (1 << 12)
#define OPND_REGISTER_MINUS_LITERAL              (1 << 13)
#define OPND_REGISTER_POST_INCREMENT_BY_N        (1 << 14)
#define OPND_REGISTER_POST_DECREMENT_BY_N        (1 << 15)
#define OPND_EXPRESSION                          (1 << 16)
#define OPND_SYMBOL                              (1 << 17)
#define OPND_CONSTANT                            (1 << 18)
#define OPND_W_REG                               (1 << 19)
#define OPND_BRANCH_ON_DSP_CONDITION             (1 << 20)
#define OPND_BRANCH_ON_CONDITION                 (1 << 21)
#define OPND_DSP_ACCUMULATOR_SELECT              (1 << 22)
#define OPND_DSP_MULTIPLICAND                    (1 << 23)
#define OPND_DSP_SQUARE                          (1 << 24)
#define OPND_PAGE_SYMBOL                         (1 << 25)
#define OPND_W_REGISTER_DIRECT                   (1 << 26)
#define OPND_ANY                                ((1 << 27) - 1)

#define OPND_VALUE (OPND_EXPRESSION | OPND_SYMBOL | OPND_CONSTANT)

/******************************************************************************
 *
 * The following are combination flags for modes P or Q.
 *
 ******************************************************************************/

#define OPND_P_OR_Q_REGISTER_DIRECT            (P_OR_Q | \
                                                OPND_REGISTER_DIRECT)
#define OPND_P_OR_Q_REGISTER_INDIRECT          (P_OR_Q | \
                                                OPND_REGISTER_INDIRECT)
#define OPND_P_OR_Q_REGISTER_POST_DECREMENT    (P_OR_Q | \
                                                OPND_REGISTER_POST_DECREMENT)
#define OPND_P_OR_Q_REGISTER_POST_INCREMENT    (P_OR_Q | \
                                                OPND_REGISTER_POST_INCREMENT)
#define OPND_P_OR_Q_REGISTER_PRE_DECREMENT     (P_OR_Q | \
                                                OPND_REGISTER_PRE_DECREMENT)
#define OPND_P_OR_Q_REGISTER_PRE_INCREMENT     (P_OR_Q | \
                                                OPND_REGISTER_PRE_INCREMENT)

#define OPND_P_OR_Q_IND ( \
                     OPND_P_OR_Q_REGISTER_INDIRECT       | \
                     OPND_P_OR_Q_REGISTER_POST_DECREMENT | \
                     OPND_P_OR_Q_REGISTER_POST_INCREMENT | \
                     OPND_P_OR_Q_REGISTER_PRE_DECREMENT  | \
                     OPND_P_OR_Q_REGISTER_PRE_INCREMENT)

#define OPND_P_OR_Q (OPND_P_OR_Q_REGISTER_DIRECT         | \
                     OPND_P_OR_Q_IND)

/******************************************************************************
 *
 * The following are combination flags for modes G or H.
 *
 ******************************************************************************/

#define OPND_G_OR_H_REGISTER_DIRECT            (G_OR_H | \
                                                OPND_REGISTER_DIRECT)
#define OPND_G_OR_H_REGISTER_INDIRECT          (G_OR_H | \
                                                OPND_REGISTER_INDIRECT)
#define OPND_G_OR_H_REGISTER_POST_DECREMENT    (G_OR_H | \
                                                OPND_REGISTER_POST_DECREMENT)
#define OPND_G_OR_H_REGISTER_POST_INCREMENT    (G_OR_H | \
                                                OPND_REGISTER_POST_INCREMENT)
#define OPND_G_OR_H_REGISTER_PRE_DECREMENT     (G_OR_H | \
                                                OPND_REGISTER_PRE_DECREMENT)
#define OPND_G_OR_H_REGISTER_PRE_INCREMENT     (G_OR_H | \
                                                OPND_REGISTER_PRE_INCREMENT)
#define OPND_G_OR_H_REGISTER_WITH_DISPLACEMENT (G_OR_H | \
                                                OPND_REGISTER_WITH_DISPLACEMENT)

#define OPND_G_OR_H (OPND_G_OR_H_REGISTER_DIRECT             | \
                     OPND_G_OR_H_REGISTER_INDIRECT           | \
                     OPND_G_OR_H_REGISTER_POST_DECREMENT     | \
                     OPND_G_OR_H_REGISTER_POST_INCREMENT     | \
                     OPND_G_OR_H_REGISTER_PRE_DECREMENT      | \
                     OPND_G_OR_H_REGISTER_PRE_INCREMENT      | \
                     OPND_G_OR_H_REGISTER_WITH_DISPLACEMENT)

/******************************************************************************
 *
 * The following combination flag is for the literal offset MOV instructions.
 *
 ******************************************************************************/
#define OPND_LO_REGISTER (OPND_REGISTER_PLUS_LITERAL   | \
                          OPND_REGISTER_MINUS_LITERAL)

/******************************************************************************
 *
 * The following are combination flags for DSP instructions.
 *
 ******************************************************************************/
#define OPND_DSP_PRESHIFT            (OPND_VALUE)
#define OPND_DSP_PREFETCH_OPERATION  (OPND_REGISTER_INDIRECT            | \
                                      OPND_REGISTER_POST_INCREMENT_BY_N | \
                                      OPND_REGISTER_POST_DECREMENT_BY_N | \
                                      OPND_REGISTER_WITH_DISPLACEMENT)
#define OPND_DSP_WRITEBACK           (OPND_REGISTER_DIRECT              | \
                                      OPND_REGISTER_POST_INCREMENT_BY_N)

/******************************************************************************
 *
 * Opcode flags
 *
 ******************************************************************************/
#define F_NONE                       (0)
#define F_WORD                       (1 << 0)
#define F_HAS_REQUIRED_WREG          (1 << 1)
#define F_HAS_IMPLIED_WREG           (1 << 2)
#define F_HAS_WREG                   (F_HAS_REQUIRED_WREG | F_HAS_IMPLIED_WREG)
#define F_HAS_BRANCH_FLAG            (1 << 3)
#define F_IS_DSP_INSN                (1 << 4)
#define F_CANNOT_FOLLOW_REPEAT       (1 << 5)
#define F_IS_2_WORD_INSN             (1 << 6)
#define F_ECORE                      (1 << 7)
#define F_FCORE                      (1 << 8)

/******************************************************************************
 *
 * The following defines give clear names to the machine dependent operatorT's.
 *
 ******************************************************************************/

#define O_REGISTER_INDIRECT                  (operatorT)O_md1
#define O_REGISTER_POST_DECREMENT            (operatorT)O_md2
#define O_REGISTER_POST_INCREMENT            (operatorT)O_md3
#define O_REGISTER_PRE_DECREMENT             (operatorT)O_md4
#define O_REGISTER_PRE_INCREMENT             (operatorT)O_md5
#define O_REGISTER_WITH_DISPLACEMENT         (operatorT)O_md6
#define O_REGISTER_PLUS_LITERAL              (operatorT)O_md7
#define O_REGISTER_MINUS_LITERAL             (operatorT)O_md8
#define O_REGISTER_POST_INCREMENT_BY_N       (operatorT)O_md9
#define O_REGISTER_POST_DECREMENT_BY_N       (operatorT)O_md10
#define O_W_REG                              (operatorT)O_md11
#define O_BRANCH_ON_DSP_CONDITION            (operatorT)O_md12
#define O_BRANCH_ON_CONDITION                (operatorT)O_md13
#define O_DSP_ACCUMULATOR_SELECT             (operatorT)O_md14
#define O_DSP_MULTIPLICAND                   (operatorT)O_md15
#define O_DSP_SQUARE                         (operatorT)O_md16

/******************************************************************************
 *
 * The following defines allow us to set whether the destination of a file
 * register instruction is the working register or the file register.
 *
 ******************************************************************************/

#define PIC30_DESTINATION_WORKING_REGISTER 0

/******************************************************************************
 *
 * The following defines allow us to set the condition code portion of the
 * branch instruction.
 *
 ******************************************************************************/

#define PIC30_BRANCH_FLAG_OPERAND_POSITION 1

#define PIC30_BRANCH_ON_OVERFLOW                  (0x0)
#define PIC30_BRANCH_ON_CARRY                     (0x1)
#define PIC30_BRANCH_ON_GREATER_OR_EQUAL_UNSIGNED (0x1)
#define PIC30_BRANCH_ON_ZERO                      (0x2)
#define PIC30_BRANCH_ON_NEGATIVE                  (0x3)
#define PIC30_BRANCH_ON_LESS_OR_EQUAL             (0x4)
#define PIC30_BRANCH_ON_LESS_THAN                 (0x5)
#define PIC30_BRANCH_ON_LESS_OR_EQUAL_UNSIGNED    (0x6)
#define PIC30_BRANCH_ON_NOT_OVERFLOW              (0x8)
#define PIC30_BRANCH_ON_NOT_CARRY                 (0x9)
#define PIC30_BRANCH_ON_LESS_THAN_UNSIGNED        (0x9)
#define PIC30_BRANCH_ON_NOT_ZERO                  (0xA)
#define PIC30_BRANCH_ON_NOT_NEGATIVE              (0xB)
#define PIC30_BRANCH_ON_GREATER_THAN              (0xC)
#define PIC30_BRANCH_ON_GREATER_OR_EQUAL          (0xD)
#define PIC30_BRANCH_ON_GREATER_THAN_UNSIGNED     (0xE)

#define PIC30_BRANCH_ON_ACCUMULATOR_A_OVERFLOW    (0x0)
#define PIC30_BRANCH_ON_ACCUMULATOR_B_OVERFLOW    (0x1)
#define PIC30_BRANCH_ON_ACCUMULATOR_A_SATURATION  (0x2)
#define PIC30_BRANCH_ON_ACCUMULATOR_B_SATURATION  (0x3)

#define PIC30_MAX_CONDITION_CODE_LENGTH 3

/******************************************************************************
 *
 * The following defines allow us to set the accumulator select bit of the
 * DSP instructions.
 *
 ******************************************************************************/

#define PIC30_DSP_ACCUMULATOR_A 0
#define PIC30_DSP_ACCUMULATOR_B 1

#define PIC30_DSP_ACCUMULATOR_A_STRING "A"
#define PIC30_DSP_ACCUMULATOR_B_STRING "B"

#define PIC30_MAX_DSP_ACCUMULATOR_LENGTH 1

/******************************************************************************
 *
 * The following defines allow us to set the writeback bits of the
 * DSP instructions.
 *
 ******************************************************************************/

#define PIC30_DSP_WRITEBACK_DIRECT              0
#define PIC30_DSP_WRITEBACK_POST_INCREMENT_BY_N 1

#define PIC30_DSP_WRITEBACK_REGISTER_NUMBER (13)
#define PIC30_MAX_DSP_WRITEBACK_LENGTH      (8)

/******************************************************************************
 *
 * The following defines allow us to set the prefetch bits of the DSP
 * instructions.
 *
 ******************************************************************************/
#define PIC30_MAX_REGISTER_LENGTH 3

#define PIC30_MIN_DSP_PREFETCH_DST_REGISTER_VALUE 4
#define PIC30_MAX_DSP_PREFETCH_DST_REGISTER_VALUE 7

#define PIC30_MAX_DSP_X_PREFETCH_OPERATION_LENGTH 9
#define PIC30_MAX_DSP_Y_PREFETCH_OPERATION_LENGTH 9

#define PIC30_X_PREFETCH_MAC1_REGISTER_VALUE 8
#define PIC30_X_PREFETCH_MAC2_REGISTER_VALUE 9

#define PIC30_X_PREFETCH_MAC1_REGISTER "w8"
#define PIC30_X_PREFETCH_MAC2_REGISTER "w9"

#define PIC30_Y_PREFETCH_MAC1_REGISTER_VALUE 10
#define PIC30_Y_PREFETCH_MAC2_REGISTER_VALUE 11

#define PIC30_Y_PREFETCH_MAC1_REGISTER "w10"
#define PIC30_Y_PREFETCH_MAC2_REGISTER "w11"

#define PIC30_DSP_PREFETCH_DISPLACEMENT_REGISTER_VALUE 12
#define PIC30_DSP_PREFETCH_DISPLACEMENT_REGISTER "w12"

#define PIC30_DSP_X_PREFETCH_OPERATION_MAC1_INDIRECT 0x0
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC1_PLUS2    0x1
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC1_PLUS4    0x2
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC1_PLUS6    0x3
#define PIC30_DSP_X_PREFETCH_OPERATION_NO_PREFETCH   0x4
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC1_MINUS2   0x7
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC1_MINUS4   0x6
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC1_MINUS6   0x5
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC2_INDIRECT 0x8
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC2_PLUS2    0x9
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC2_PLUS4    0xA
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC2_PLUS6    0xB
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC2_PLUS_REG 0xC
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC2_MINUS6   0xD
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC2_MINUS4   0xE
#define PIC30_DSP_X_PREFETCH_OPERATION_MAC2_MINUS2   0xF

#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_INDIRECT 0x0
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_PLUS2    0x1
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_PLUS4    0x2
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_PLUS6    0x3
#define PIC30_DSP_Y_PREFETCH_OPERATION_NO_PREFETCH   0x4
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_MINUS2   0x7
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_MINUS4   0x6
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC1_MINUS6   0x5
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_INDIRECT 0x8
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_PLUS2    0x9
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_PLUS4    0xA
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_PLUS6    0xB
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_PLUS_REG 0xC
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_MINUS6   0xD
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_MINUS4   0xE
#define PIC30_DSP_Y_PREFETCH_OPERATION_MAC2_MINUS2   0xF

/******************************************************************************
 *
 * The following defines allow us to set the multiplicand bits of the DSP
 * instructions.
 *
 ******************************************************************************/
#define PIC30_MAX_DSP_MULTIPLICAND_LENGTH 7

#define PIC30_DSP_W4_W5_MULTIPLICAND 0x0
#define PIC30_DSP_W4_W6_MULTIPLICAND 0x1
#define PIC30_DSP_W4_W7_MULTIPLICAND 0x2
#define PIC30_DSP_W5_W6_MULTIPLICAND 0x4
#define PIC30_DSP_W5_W7_MULTIPLICAND 0x5
#define PIC30_DSP_W6_W7_MULTIPLICAND 0x6

#define PIC30_DSP_W4_W5_MULTIPLICAND_STRING "w4 * w5"
#define PIC30_DSP_W4_W6_MULTIPLICAND_STRING "w4 * w6"
#define PIC30_DSP_W4_W7_MULTIPLICAND_STRING "w4 * w7"
#define PIC30_DSP_W5_W6_MULTIPLICAND_STRING "w5 * w6"
#define PIC30_DSP_W5_W7_MULTIPLICAND_STRING "w5 * w7"
#define PIC30_DSP_W6_W7_MULTIPLICAND_STRING "w6 * w7"

/******************************************************************************/

#define PIC30_SIZE_OF_PROGRAM_WORD 4
#define PIC30_SIZE_OF_DATA_WORD 2

#define PIC30_CALL_SECOND_PROGRAM_WORD 0x000000

#define PIC30_ADD_OPERAND(insn, op_val, bits, shift) \
   ((insn) | ((op_val) & ((1 << (bits)) - 1)) << (shift))
/*    (insn) = (insn) | ((op_val) & ((1 << (bits)) - 1)) << (shift) */

#define PIC30_EXTRACT_OPERAND(insn, bits, shift) \
  (((insn) >> (shift)) & ((1 << (bits)) - 1))



#define PIC30_BASE_OPERAND (0)

#define BASE_10_STRING_LENGTH 10

#define PIC30_ADDRESSING_MODE_BITS 3
#define PIC30_SRC_MODE_SHIFT 4
#define PIC30_DST_MODE_SHIFT 11

#define PIC30_W_REGISTER_STRING "WREG"

extern const struct pic30_operand pic30_operands[];
extern const int pic30_num_operands;
extern const struct pic30_opcode pic30_opcodes[];
extern const int pic30_num_opcodes;
extern const unsigned long pic30_operator_mask[];
extern const struct relocation_info pic30_relocation_info[];
extern const char * pic30_registers[];
extern const int pic30_num_registers;
#if 0
extern const struct pic30_processor pic30_processors[];
#endif
extern const int pic30_num_processors;

struct pic30_predefined_symbol
{
   const char * name;
   long value;
};

extern const struct pic30_predefined_symbol pic30_sfr_symbols[];
extern const int pic30_num_sfr_symbols;

/******************************************************************************/

extern const struct relocation_info pic30_get_pc_relative_relocation_info(void);

/******************************************************************************/

#define CALL_INSN     0x020000
#define RCALL_INSN    0x070000
#define GOTO_INSN     0x040000
#define BRA_DSP_INSN  0x0C0000
#define BRA_CC_INSN   0x300000
#define BRA_INSN      0x370000
#define DO_INSN       0x080000
#define DOW_INSN      0x088000
#define REPEAT_INSN   0x090000
#define REPEATW_INSN  0x098000
#define CPWBEQ_W_INSN 0xE78000
#define CPWBEQ_B_INSN 0xE78400
#define CPWBGT_W_INSN 0xE60000
#define CPWBGT_B_INSN 0xE60400
#define CPWBLT_W_INSN 0xE68000
#define CPWBLT_B_INSN 0xE68400
#define CPWBNE_W_INSN 0xE70000
#define CPWBNE_B_INSN 0xE70400

/******************************************************************************/

/*
**                      baseop
**                       |              number_of_words
**                       |              |  opcode
**                       |              |  |             mask
**                       |              |  |             |
**                       v              v  v             v
*/
#define DUMMY            OP_DUMMY,      0, { 0xFFFFFF }, 0xFFFFFF

#define ADDWFF_B         OP_ADDWFF_B,   1, { 0xB46000 }, 0xFFE000
#define ADDWFW_B         OP_ADDWFW_B,   1, { 0xB44000 }, 0xFFE000
#define ADDLW_B          OP_ADDLW_B,    1, { 0xB04000 }, 0xFFC000
#define ADDLS_B          OP_ADDLS_B,    1, { 0x404060 }, 0xF84060
#define ADD_B            OP_ADD_B,      1, { 0x404000 }, 0xF84000

#define ADDWFF_W         OP_ADDWFF_W,   1, { 0xB42000 }, 0xFFE000
#define ADDWFW_W         OP_ADDWFW_W,   1, { 0xB40000 }, 0xFFE000
#define ADDLW_W          OP_ADDLW_W,    1, { 0xB00000 }, 0xFFC000
#define ADDLS_W          OP_ADDLS_W,    1, { 0x400060 }, 0xF84060
#define ADD_W            OP_ADD_W,      1, { 0x400000 }, 0xF84000
#define ADDAB            OP_ADDAB,      1, { 0xCB0000 }, 0xFF7000
#define ADDAC_PS         OP_ADDAC_PS,   1, { 0xC90000 }, 0xFF0000
#define ADDAC            OP_ADDAC,      1, { 0xC90000 }, 0xFF0780

#define ADDCWFF_B        OP_ADDCWFF_B,  1, { 0xB4E000 }, 0xFFE000
#define ADDCWFW_B        OP_ADDCWFW_B,  1, { 0xB4C000 }, 0xFFE000
#define ADDCLW_B         OP_ADDCLW_B,   1, { 0xB0C000 }, 0xFFC000
#define ADDCLS_B         OP_ADDCLS_B,   1, { 0x484060 }, 0xF84060
#define ADDC_B           OP_ADDC_B,     1, { 0x484000 }, 0xF84000

#define ADDCWFF_W        OP_ADDCWFF_W,  1, { 0xB4A000 }, 0xFFE000
#define ADDCWFW_W        OP_ADDCWFW_W,  1, { 0xB48000 }, 0xFFE000
#define ADDCLW_W         OP_ADDCLW_W,   1, { 0xB08000 }, 0xFFC000
#define ADDCLS_W         OP_ADDCLS_W,   1, { 0x480060 }, 0xF84060
#define ADDC_W           OP_ADDC_W,     1, { 0x480000 }, 0xF84000

#define ANDWFF_B         OP_ANDWFF_B,   1, { 0xB66000 }, 0xFFE000
#define ANDWFW_B         OP_ANDWFW_B,   1, { 0xB64000 }, 0xFFE000
#define ANDLW_B          OP_ANDLW_B,    1, { 0xB24000 }, 0xFFC000
#define ANDLS_B          OP_ANDLS_B,    1, { 0x604060 }, 0xF84060
#define AND_B            OP_AND_B,      1, { 0x604000 }, 0xF84000

#define ANDWFF_W         OP_ANDWFF_W,   1, { 0xB62000 }, 0xFFE000
#define ANDWFW_W         OP_ANDWFW_W,   1, { 0xB60000 }, 0xFFE000
#define ANDLW_W          OP_ANDLW_W,    1, { 0xB20000 }, 0xFFC000
#define ANDLS_W          OP_ANDLS_W,    1, { 0x600060 }, 0xF84060
#define AND_W            OP_AND_W,      1, { 0x600000 }, 0xF84000

#define ASRFF_B          OP_ASRFF_B,    1, { 0xD5E000 }, 0xFFE000
#define ASRFW_B          OP_ASRFW_B,    1, { 0xD5C000 }, 0xFFE000
#define ASR_B            OP_ASR_B,      1, { 0xD1C000 }, 0xFFC000
 
#define ASRFF_W          OP_ASRFF_W,    1, { 0xD5A000 }, 0xFFE000
#define ASRFW_W          OP_ASRFW_W,    1, { 0xD58000 }, 0xFFE000
#define ASR_W            OP_ASR_W,      1, { 0xD18000 }, 0xFFC000
#define ASRW_W           OP_ASRW_W,     1, { 0xDE8000 }, 0xFF8070
#define ASRK_W           OP_ASRK_W,     1, { 0xDE8040 }, 0xFF8070

#define BCLR_W           OP_BCLR_W,     1, { 0xA10000 }, 0xFF0F80
#define BCLRF_W          OP_BCLRF_B,    1, { 0xA90000 }, 0x000000
#define BCLRF_B          OP_BCLRF_B,    1, { 0xA90000 }, 0xFF0000
#define BCLR_B           OP_BCLR_B,     1, { 0xA10400 }, 0xFF0F80

#define BRA_DSP          OP_BRA_DSP,    1, { BRA_DSP_INSN }, 0xFC0000
#define BRA              OP_BRA,        1, { BRA_INSN },     0xFF0000
#define BRA_CC           OP_BRA_CC,     1, { BRA_CC_INSN },  0xF00000
#define BRAW             OP_BRAW,       1, { 0x016000 },     0xFFFFF0
#define BRAWE            OP_BRAWE,      1, { 0x010600 },     0xFF0600

#define BSET_W           OP_BSET_W,     1, { 0xA00000 }, 0xFF0F80
#define BSETF_W          OP_BSETF_B,    1, { 0xA80000 }, 0x000000
#define BSETF_B          OP_BSETF_B,    1, { 0xA80000 }, 0xFF0000
#define BSET_B           OP_BSET_B,     1, { 0xA00400 }, 0xFF8F80

#define BSW_Z            OP_BSW_Z,      1, { 0xAD8000 }, 0xFF8780
#define BSW_C            OP_BSW_C,      1, { 0xAD0000 }, 0xFF8780

#define BTG_W            OP_BTG_W,      1, { 0xA20000 }, 0xFF0400
#define BTGF_W           OP_BTGF_B,     1, { 0xAA0000 }, 0x000000
#define BTGF_B           OP_BTGF_B,     1, { 0xAA0000 }, 0xFF0000
#define BTG_B            OP_BTG_B,      1, { 0xA20400 }, 0xFF8400

#define BTSC_W           OP_BTSC_W,     1, { 0xA70000 }, 0xFF0F80
#define BTSCF_W          OP_BTSCF_B,    1, { 0xAF0000 }, 0x000000
#define BTSCF_B          OP_BTSCF_B,    1, { 0xAF0000 }, 0xFF0000
#define BTSC_B           OP_BTSC_B,     1, { 0xA70000 }, 0xFF8F80

#define BTSS_W           OP_BTSS_W,     1, { 0xA60000 }, 0xFF0F80
#define BTSSF_W          OP_BTSSF_B,    1, { 0xAE0000 }, 0x000000
#define BTSSF_B          OP_BTSSF_B,    1, { 0xAE0000 }, 0xFF0000
#define BTSS_B           OP_BTSS_B,     1, { 0xA60000 }, 0xFF8F80

#define BTST_Z           OP_BTST_Z,     1, { 0xA30800 }, 0xFF0F80
#define BTSTW_Z          OP_BTSTW_Z,    1, { 0xA58000 }, 0xFF8780
#define BTST_C           OP_BTST_C,     1, { 0xA30000 }, 0xFF0F80
#define BTSTW_C          OP_BTSTW_C,    1, { 0xA50000 }, 0xFF8780
#define BTSTF_W          OP_BTSTF_B,    1, { 0xAB0000 }, 0x000000
#define BTSTF_B          OP_BTSTF_B,    1, { 0xAB0000 }, 0xFF0000
#define BTST_B           OP_BTST_B,     1, { 0xA30800 }, 0xFF8F80

#define BTSTS_Z          OP_BTSTS_Z,    1, { 0xA40800 }, 0xFF0F80
#define BTSTS_C          OP_BTSTS_C,    1, { 0xA40000 }, 0xFF0F80
#define BTSTSF_W         OP_BTSTSF_B,   1, { 0xAC0000 }, 0x000000
#define BTSTSF_B         OP_BTSTSF_B,   1, { 0xAC0000 }, 0xFF0000
#define BTSTS_B          OP_BTSTS_B,    1, { 0xA40800 }, 0xFF8F80


#define CALLW            OP_CALLW,       1, { 0x010000 },           0xFF8600
#define CALL             OP_CALL,       2, { CALL_INSN,  0x000000 },0xFF0000
#define CALL_L           OP_CALLW,      1, { 0x018000 },            0xFF8600

#define CLRW_B           OP_CLRW_B,     1, { 0xEF4000 }, 0xFFFFFF
#define CLR_B            OP_CLR_B,      1, { 0xEB4000 }, 0xFFC07F
#define CLRF_B           OP_CLRF_B,     1, { 0xEF6000 }, 0xFFE000

#define CLRW_W           OP_CLRW_W,     1, { 0xEF0000 }, 0xFFFFFF
#define CLR_W            OP_CLR_W,      1, { 0xEB0000 }, 0xFFC07F
#define CLRF_W           OP_CLRF_W,     1, { 0xEF2000 }, 0xFFE000

#define CLRAC_A          OP_CLRAC_A,    1, { 0xC30112 }, 0xFF7FFF
#define CLRAC_WB         OP_CLRAC_WB,   1, { 0xC30110 }, 0xFF7FFC
#define CLRAC_X          OP_CLRAC_X,    1, { 0xC30012 }, 0xFF4C3F
#define CLRAC_Y          OP_CLRAC_Y,    1, { 0xC30102 }, 0xFF43C3
#define CLRAC_XWB        OP_CLRAC_XWB,  1, { 0xC30010 }, 0xFF4C3C
#define CLRAC_YWB        OP_CLRAC_YWB,  1, { 0xC30100 }, 0xFF43C0
#define CLRAC_XY         OP_CLRAC_XY,   1, { 0xC30002 }, 0xFF4003
#define CLRAC            OP_CLRAC,      1, { 0xC30000 }, 0xFF4000

#define CLRWDT           OP_CLRWDT,     1, { 0xFE6000 }, 0xFFFFFF

#define COMFF_B          OP_COMFF_B,    1, { 0xEEE000 }, 0xFFE000
#define COMFW_B          OP_COMFW_B,    1, { 0xEEC000 }, 0xFFE000
#define COM_B            OP_COM_B,      1, { 0xEAC000 }, 0xFFC000

#define COMFF_W          OP_COMFF_W,    1, { 0xEEA000 }, 0xFFE000
#define COMFW_W          OP_COMFW_W,    1, { 0xEE8000 }, 0xFFE000
#define COM_W            OP_COM_W,      1, { 0xEA8000 }, 0xFFC000

#define CPLS_B           OP_CPLS_B,     1, { 0xE10460 }, 0xFF87E0
#define CPLS8_B          OP_CPLS8_B,    1, { 0xE10460 }, 0xFF8460
#define CPF_B            OP_CPF_B,      1, { 0xE34000 }, 0xFFE000
#define CP_B             OP_CP_B,       1, { 0xE10400 }, 0xFF8780

#define CPLS_W           OP_CPLS_W,     1, { 0xE10060 }, 0xFF87E0
#define CPLS8_W          OP_CPLS8_W,    1, { 0xE10060 }, 0xFF8460
#define CPF_W            OP_CPF_W,      1, { 0xE30000 }, 0xFFE000
#define CP_W             OP_CP_W,       1, { 0xE10000 }, 0xFF8780

#define CPBLS_B          OP_CPBLS_B,    1, { 0xE18460 }, 0xFF87E0
#define CPBLS8_B         OP_CPBLS8_B,   1, { 0xE18460 }, 0xFF8460
#define CPBF_B           OP_CPBF_B,     1, { 0xE3C000 }, 0xFFE000
#define CPB_B            OP_CPB_B,      1, { 0xE18400 }, 0xFF8780

#define CPBLS_W          OP_CPBLS_W,    1, { 0xE18060 }, 0xFF87E0
#define CPBLS8_W         OP_CPBLS8_W,   1, { 0xE18060 }, 0xFF8460
#define CPBF_W           OP_CPBF_W,     1, { 0xE38000 }, 0xFFE000
#define CPB_W            OP_CPB_W,      1, { 0xE18000 }, 0xFF8780

#define CP0F_B           OP_CP0F_B,     1, { 0xE24000 }, 0xFFE000
#define CP0_B            OP_CP0_B,      1, { 0xE00400 }, 0xFFFF80

#define CP0F_W           OP_CP0F_W,     1, { 0xE20000 }, 0xFFE000
#define CP0_W            OP_CP0_W,      1, { 0xE00000 }, 0xFFFF80

#define CP1_B            OP_CP1_B,      1, { 0xE0C000 }, 0xFFFF80
#define CP1_W            OP_CP1_W,      1, { 0xE08000 }, 0xFFFF80

#define CPWSEQ_B         OP_CPWSEQ_B,   1, { CPWBEQ_B_INSN }, 0xFF87F0
#define CPWBEQ_B         OP_CPWBEQ_B,   1, { CPWBEQ_B_INSN }, 0xFF8400
#define CPWSEQ_W         OP_CPWSEQ_W,   1, { CPWBEQ_W_INSN }, 0xFF87F0
#define CPWBEQ_W         OP_CPWBEQ_W,   1, { CPWBEQ_W_INSN }, 0xFF8400
#define CPWSEQ_E_B       OP_CPWSEQ_E_B, 1, { 0xE78410 }, 0xFF87F0
#define CPWSEQ_E_W       OP_CPWSEQ_E_W, 1, { 0xE78010 }, 0xFF87F0


#define CPWSGT_B         OP_CPWSGT_B,   1, { CPWBGT_B_INSN }, 0xFF87F0
#define CPWBGT_B         OP_CPWBGT_B,   1, { CPWBGT_B_INSN }, 0xFF8400
#define CPWSGT_W         OP_CPWSGT_W,   1, { CPWBGT_W_INSN }, 0xFF87F0
#define CPWBGT_W         OP_CPWBGT_W,   1, { CPWBGT_W_INSN }, 0xFF8400
#define CPWSGT_E_B       OP_CPWSGT_E_B, 1, { 0xE60410 }, 0xFF87F0
#define CPWSGT_E_W       OP_CPWSGT_E_W, 1, { 0xE60010 }, 0xFF87F0


#define CPWSLT_B         OP_CPWSLT_B,   1, { CPWBLT_B_INSN }, 0xFF87F0
#define CPWBLT_B         OP_CPWBLT_B,   1, { CPWBLT_B_INSN }, 0xFF8400
#define CPWSLT_W         OP_CPWSLT_W,   1, { CPWBLT_W_INSN }, 0xFF87F0
#define CPWBLT_W         OP_CPWBLT_W,   1, { CPWBLT_W_INSN }, 0xFF8400
#define CPWSLT_E_B       OP_CPWSLT_E_B, 1, { 0xE68410 }, 0xFF87F0
#define CPWSLT_E_W       OP_CPWSLT_E_W, 1, { 0xE68010 }, 0xFF87F0


#define CPWSNE_B         OP_CPWSNE_B,   1, { CPWBNE_B_INSN }, 0xFF87F0
#define CPWBNE_B         OP_CPWBNE_B,   1, { CPWBNE_B_INSN }, 0xFF8400
#define CPWSNE_W         OP_CPWSNE_W,   1, { CPWBNE_W_INSN }, 0xFF87F0
#define CPWBNE_W         OP_CPWBNE_W,   1, { CPWBNE_W_INSN }, 0xFF8400
#define CPWSNE_E_B       OP_CPWSNE_E_B, 1, { 0xE70410 }, 0xFF87F0
#define CPWSNE_E_W       OP_CPWSNE_E_W, 1, { 0xE70010 }, 0xFF87F0

#define DAW_B            OP_DAW_B,      1, { 0xFD4000 }, 0xFFFFF0

#define DECFF_B          OP_DECFF_B,    1, { 0xED6000 }, 0xFFE000
#define DECFW_B          OP_DECFW_B,    1, { 0xED4000 }, 0xFFE000
#define DEC_B            OP_DEC_B,      1, { 0xE94000 }, 0xFFC000

#define DECFF_W          OP_DECFF_W,    1, { 0xED2000 }, 0xFFE000
#define DECFW_W          OP_DECFW_W,    1, { 0xED0000 }, 0xFFE000
#define DEC_W            OP_DEC_W,      1, { 0xE90000 }, 0xFFC000

#define DEC2FF_B         OP_DEC2FF_B,   1, { 0xEDE000 }, 0xFFE000
#define DEC2FW_B         OP_DEC2FW_B,   1, { 0xEDC000 }, 0xFFE000
#define DEC2_B           OP_DEC2_B,     1, { 0xE9C000 }, 0xFFC000
#define DEC2FF_W         OP_DEC2FF_W,   1, { 0xEDA000 }, 0xFFE000
#define DEC2FW_W         OP_DEC2FW_W,   1, { 0xED8000 }, 0xFFE000
#define DEC2_W           OP_DEC2_W,     1, { 0xE98000 }, 0xFFC000

#define DISI             OP_DISI,       1, { 0xFC0000 }, 0xFFC000

#define DIVF             OP_DIVF,       1, { 0xD90000 }, 0xFF87F0

#define DIVS_W           OP_DIVS_W,     1, { 0xD80000 }, 0xFFF870
#define DIVS_D           OP_DIVS_D,     1, { 0xD80040 }, 0xFF8070

#define DIVU_W           OP_DIVU_W,     1, { 0xD88000 }, 0xFFF870
#define DIVU_D           OP_DIVU_D,     1, { 0xD88040 }, 0xFF8070

#define DOW              OP_DOW,        2, { DOW_INSN, 0x000000 }, 0xFF80F0
#define DO               OP_DO,         2, { DO_INSN,  0x000000 }, 0xFFC000
#define DOE              OP_DO,         2, { DO_INSN,  0x000000 }, 0xFF8000

#define ED               OP_ED,         1, { 0xF04003 }, 0xFC4003

#define EDAC             OP_EDAC,       1, { 0xF04002 }, 0xFC4C03

#define EXCH             OP_EXCH,       1, { 0xFD0000 }, 0xFFF870

#define FBCL             OP_FBCL,       1, { 0xDF0000 }, 0xFFF800

#define FF1L             OP_FF1L,       1, { 0xCF8000 }, 0xFFF800

#define FF1R             OP_FF1R,       1, { 0xCF0000 }, 0xFFF800

#define GOTOW            OP_GOTOW,      1, { 0x014000 },           0xFFFFF0
#define GOTO             OP_GOTO,       2, { GOTO_INSN, 0x000000 }, 0xFF0000
#define GOTOWE           OP_GOTOW,      1, { 0x010400 },           0xFF8600
#define GOTOW_L          OP_GOTOW,     1, { 0x018400 },           0xFF8600

#define INCFF_B          OP_INCFF_B,    1, { 0xEC6000 }, 0xFFE000
#define INCFW_B          OP_INCFW_B,    1, { 0xEC4000 }, 0xFFE000
#define INC_B            OP_INC_B,      1, { 0xE84000 }, 0xFFC000

#define INCFF_W          OP_INCFF_W,    1, { 0xEC2000 }, 0xFFE000
#define INCFW_W          OP_INCFW_W,    1, { 0xEC0000 }, 0xFFE000
#define INC_W            OP_INC_W,      1, { 0xE80000 }, 0xFFC000

#define INC2FF_B         OP_INC2FF_B,   1, { 0xECE000 }, 0xFFE000
#define INC2FW_B         OP_INC2FW_B,   1, { 0xECC000 }, 0xFFE000
#define INC2_B           OP_INC2_B,     1, { 0xE8C000 }, 0xFFC000
#define INC2FF_W         OP_INC2FF_W,   1, { 0xECA000 }, 0xFFE000
#define INC2FW_W         OP_INC2FW_W,   1, { 0xEC8000 }, 0xFFE000
#define INC2_W           OP_INC2_W,     1, { 0xE88000 }, 0xFFC000

#define IORWFF_B         OP_IORWFF_B,   1, { 0xB76000 }, 0xFFE000
#define IORWFW_B         OP_IORWFW_B,   1, { 0xB74000 }, 0xFFE000
#define IORLW_B          OP_IORLW_B,    1, { 0xB34000 }, 0xFFC000
#define IORLS_B          OP_IORLS_B,    1, { 0x704060 }, 0xF84060
#define IOR_B            OP_IOR_B,      1, { 0x704000 }, 0xF84000

#define IORWFF_W         OP_IORWFF_W,   1, { 0xB72000 }, 0xFFE000
#define IORWFW_W         OP_IORWFW_W,   1, { 0xB70000 }, 0xFFE000
#define IORLW_W          OP_IORLW_W,    1, { 0xB30000 }, 0xFFC000
#define IORLS_W          OP_IORLS_W,    1, { 0x700060 }, 0xF84060
#define IOR_W            OP_IOR_W,      1, { 0x700000 }, 0xF84000

#define LAC_PS           OP_LAC_PS,     1, { 0xCA0000 }, 0xFF0000
#define LAC              OP_LAC,        1, { 0xCA0000 }, 0xFF0780

#define LNK              OP_LNK,        1, { 0xFA0000 }, 0xFFC000

#define LSRFF_B          OP_LSRFF_B,    1, { 0xD56000 }, 0xFFE000
#define LSRFW_B          OP_LSRFW_B,    1, { 0xD54000 }, 0xFFE000
#define LSR_B            OP_LSR_B,      1, { 0xD14000 }, 0xFFC000

#define LSRFF_W          OP_LSRFF_W,    1, { 0xD52000 }, 0xFFE000
#define LSRFW_W          OP_LSRFW_W,    1, { 0xD50000 }, 0xFFE000
#define LSR_W            OP_LSR_W,      1, { 0xD10000 }, 0xFFC000
#define LSRW_W           OP_LSRW_W,     1, { 0xDE0000 }, 0xFF8070
#define LSRK_W           OP_LSRK_W,     1, { 0xDE0040 }, 0xFF8070

#define MPY_A            OP_MPY_A,      1, { 0xC00113 }, 0xF87FFF
#define MPY_X            OP_MPY_X,      1, { 0xC00013 }, 0xF84C3F
#define MPY_Y            OP_MPY_Y,      1, { 0xC00103 }, 0xF843C3
#define MPY              OP_MPY,        1, { 0xC00003 }, 0xF84003
#define SQR_A            OP_SQR_A,      1, { 0xF00111 }, 0xFC7FFF
#define SQR_X            OP_SQR_X,      1, { 0xF00011 }, 0xFC4C3F
#define SQR_Y            OP_SQR_Y,      1, { 0xF00101 }, 0xFC73C3
#define SQR              OP_SQR,        1, { 0xF00001 }, 0xFC4003

#define MPYN_A           OP_MPYN_A,     1, { 0xC04113 }, 0xF87FFF
#define MPYN_X           OP_MPYN_X,     1, { 0xC04013 }, 0xF84C3F
#define MPYN_Y           OP_MPYN_Y,     1, { 0xC04103 }, 0xF843C3
#define MPYN             OP_MPYN,       1, { 0xC04003 }, 0xF84003

#define MPY_A            OP_MPY_A,      1, { 0xC00113 }, 0xF87FFF
#define MPY_X            OP_MPY_X,      1, { 0xC00013 }, 0xF84C3F
#define MPY_Y            OP_MPY_Y,      1, { 0xC00103 }, 0xF843C3
#define MPY              OP_MPY,        1, { 0xC00003 }, 0xF84003
#define SQR_A            OP_SQR_A,      1, { 0xF00111 }, 0xFC7FFF
#define SQR_X            OP_SQR_X,      1, { 0xF00011 }, 0xFC4C3F
#define SQR_Y            OP_SQR_Y,      1, { 0xF00101 }, 0xFC73C3
#define SQR              OP_SQR,        1, { 0xF00001 }, 0xFC4003

#define MPYN_A           OP_MPYN_A,     1, { 0xC04113 }, 0xF87FFF
#define MPYN_X           OP_MPYN_X,     1, { 0xC04013 }, 0xF84C3F
#define MPYN_Y           OP_MPYN_Y,     1, { 0xC04103 }, 0xF843C3
#define MPYN             OP_MPYN,       1, { 0xC04003 }, 0xF84003

#define MOVSAC_A         OP_MOVSAC_A,   1, { 0xC70112 }, 0xFF7FFF
#define MOVSAC_WB        OP_MOVSAC_WB,  1, { 0xC70110 }, 0xFF7FFC
#define MOVSAC_X         OP_MOVSAC_X,   1, { 0xC70012 }, 0xFF4C3F
#define MOVSAC_Y         OP_MOVSAC_Y,   1, { 0xC70102 }, 0xFF43C3
#define MOVSAC_XWB       OP_MOVSAC_XWB, 1, { 0xC70010 }, 0xFF4C3C
#define MOVSAC_YWB       OP_MOVSAC_YWB, 1, { 0xC70100 }, 0xFF43C0
#define MOVSAC_XY        OP_MOVSAC_XY,  1, { 0xC70002 }, 0xFF4003
#define MOVSAC           OP_MOVSAC,     1, { 0xC70000 }, 0xFF4000

#define MAC_A            OP_MAC_A,      1, { 0xC00112 }, 0xF87FFF
#define MAC_WB           OP_MAC_WB,     1, { 0xC00110 }, 0xF87FFC
#define MAC_X            OP_MAC_X,      1, { 0xC00012 }, 0xF84C3F
#define MAC_Y            OP_MAC_Y,      1, { 0xC00102 }, 0xF843C3
#define MAC_XWB          OP_MAC_XWB,    1, { 0xC00010 }, 0xF84C3C
#define MAC_YWB          OP_MAC_YWB,    1, { 0xC00100 }, 0xF843C0
#define MAC_XY           OP_MAC_XY,     1, { 0xC00002 }, 0xF84003
#define MAC              OP_MAC,        1, { 0xC00000 }, 0xF84000
#define SQRAC_A          OP_SQRAC_A,    1, { 0xF00110 }, 0xFC7FFF
#define SQRAC_X          OP_SQRAC_X,    1, { 0xF00010 }, 0xFC4C3F
#define SQRAC_Y          OP_SQRAC_Y,    1, { 0xF00100 }, 0xFC73C3
#define SQRAC            OP_SQRAC,      1, { 0xF00000 }, 0xFC4003

#define MOVWF_B          OP_MOVWF_B,    1, { 0xB7E000 }, 0xFFE000
#define MOVLW_B          OP_MOVLW_B,    1, { 0xB3C000 }, 0xFFF000
#define MOVLW_W          OP_MOVLW_W,    1, { 0xB38000 }, 0xFFC000
#define MOVFF_B          OP_MOVFF_B,    1, { 0xBFE000 }, 0xFFE000
#define MOVFW_B          OP_MOVFW_B,    1, { 0xBFC000 }, 0xFFE000
#define MOV_B            OP_MOV_B,      1, { 0x784000 }, 0xF84000
#define LDWLO_B          OP_LDWLO_B,    1, { 0x904000 }, 0xF84000
#define STWLO_B          OP_STWLO_B,    1, { 0x984000 }, 0xF84000

#define LDDW             OP_LDDW,       1, { 0xBE0000 }, 0xFFF800
#define STDW             OP_STDW,       1, { 0xBE8000 }, 0xFFC070

#define MOVWF_W          OP_MOVWF_W,    1, { 0xB7A000 }, 0xFFE000
#define MOVL_W           OP_MOVL_W,     1, { 0x200000 }, 0xF00000
#define MOVFF_W          OP_MOVFF_W,    1, { 0xBFA000 }, 0xFFE000
#define MOVFW_W          OP_MOVFW_W,    1, { 0xBF8000 }, 0xFFE000
#define MOV_W            OP_MOV_W,      1, { 0x780000 }, 0xF84000
#define MOVPAG           OP_MOVPAG,     1, { 0xFEC000 }, 0xFFE000
#define MOVPAG_W         OP_MOVPAG_W,   1, { 0xFED000 }, 0xFFF000
#define LDWLO_W          OP_LDWLO_W,    1, { 0x900000 }, 0xF84000
#define STWLO_W          OP_STWLO_W,    1, { 0x980000 }, 0xF84000
#define STW              OP_STW,        1, { 0x880000 }, 0xF80000
#define LDW              OP_LDW,        1, { 0x800000 }, 0xF80000

#define MSC_A            OP_MSC_A,      1, { 0xC04112 }, 0xF87FFF
#define MSC_WB           OP_MSC_WB,     1, { 0xC04110 }, 0xF87FFC
#define MSC_X            OP_MSC_X,      1, { 0xC04012 }, 0xF84C3F
#define MSC_Y            OP_MSC_Y,      1, { 0xC04102 }, 0xF843C3
#define MSC_XWB          OP_MSC_XWB,    1, { 0xC04010 }, 0xF84C3C
#define MSC_YWB          OP_MSC_YWB,    1, { 0xC04100 }, 0xF843C0
#define MSC_XY           OP_MSC_XY,     1, { 0xC04002 }, 0xF84003
#define MSC              OP_MSC,        1, { 0xC04000 }, 0xF84000

#define MULWF_B          OP_MULWF_B,    1, { 0xBC4000 }, 0xFFE000
#define MULWF_W          OP_MULWF_W,    1, { 0xBC0000 }, 0xFFE000

#define MUL_SS           OP_MUL_SS,     1, { 0xB98000 }, 0xFF8080
#define MUL_SS_ACC       OP_MUL_SS_ACC, 1, { 0xB98700 }, 0xFF8700
#define MULW_SS          OP_MULW_SS,    1, { 0xB98080 }, 0xFF8080
#define MULLS_SU         OP_MULLS_SU,   1, { 0xB90060 }, 0xFF80E0
#define MULLSW_SU        OP_MULLSW_SU,  1, { 0xB900E0 }, 0xFF80E0
#define MULLS_SU_ACC     OP_MULLS_SU_ACC,1,{ 0xB90760 }, 0xFF8760
#define MUL_SU           OP_MUL_SU,     1, { 0xB90000 }, 0xFF8080
#define MUL_SU_ACC       OP_MUL_SU_ACC, 1, { 0xB90700 }, 0xFF8700
#define MULW_SU          OP_MULW_SU,    1, { 0xB90080 }, 0xFF8080
#define MULLS_UU         OP_MULLS_UU,   1, { 0xB80060 }, 0xFF80E0
#define MULLSW_UU        OP_MULLSW_UU,  1, { 0xB800E0 }, 0xFF80E0
#define MULLS_UU_ACC     OP_MULLS_UU_ACC,   1, { 0xB80760 }, 0xFF8760
#define MUL_UU           OP_MUL_UU,     1, { 0xB80000 }, 0xFF8080
#define MULW_UU          OP_MULW_UU,   1, { 0xB80080 }, 0xFF8080
#define MUL_UU_ACC       OP_MUL_UU_ACC, 1, { 0xB80700 }, 0xFF8700
#define MUL_US           OP_MUL_US,     1, { 0xB88000 }, 0xFF8080
#define MUL_US_ACC       OP_MUL_US_ACC, 1, { 0xB88700 }, 0xFF8700
#define MULW_US          OP_MULW_US,   1, { 0xB88080 }, 0xFF8080

#define NEGFF_B          OP_NEGFF_B,    1, { 0xEE6000 }, 0xFFE000
#define NEGFW_B          OP_NEGFW_B,    1, { 0xEE4000 }, 0xFFE000
#define NEG_B            OP_NEG_B,      1, { 0xEA4000 }, 0xFFC000

#define NEGFF_W          OP_NEGFF_W,    1, { 0xEE2000 }, 0xFFE000
#define NEGFW_W          OP_NEGFW_W,    1, { 0xEE0000 }, 0xFFE000
#define NEG_W            OP_NEG_W,      1, { 0xEA0000 }, 0xFFC000
#define NEGAB            OP_NEGAB,      1, { 0xCB1000 }, 0xFF7000

#define NOP              OP_NOP,        1, { 0x000000 }, 0xFF0000
#define NOPR             OP_NOPR,       1, { 0xFF0000 }, 0xFF0000

#define POP_W            OP_POP_W,      1, { 0x78004F }, 0xFFC07F
#define POPF             OP_POPF,       1, { 0xF90000 }, 0xFF0000

#define POP_D            OP_POP_D,      1, { 0xBE004F }, 0xFFF87F

#define ITCH             OP_ITCH,       1, { 0xFE8000 }, 0xFFFFFF

#define PWRSAV           OP_PWRSAV,     1, { 0xFE4000 }, 0xFFFFFE

#define PUSH_W           OP_PUSH_W,     1, { 0x781F80 }, 0xFFFF80
#define PUSHF            OP_PUSHF,      1, { 0xF80000 }, 0xFF0000

#define PUSH_D           OP_PUSH_D,     1, { 0xBE9F80 }, 0xFFFFF0

#define SCRATCH          OP_SCRATCH,    1, { 0xFEA000 }, 0xFFFFFF

#define RCALLW           OP_RCALLW,     1, { 0x012000 },   0xFFFFF0
#define RCALL            OP_RCALL,      1, { RCALL_INSN }, 0xFF0000
#define RCALLWE          OP_RCALLW,     1, { 0x010200 },   0xFF0600


#define REPEATW          OP_REPEATW,    1, { REPEATW_INSN }, 0xFFFFF0
#define REPEAT           OP_REPEAT,     1, { REPEAT_INSN },  0xFFC000
#define REPEATE          OP_REPEAT,     1, { REPEAT_INSN },  0xFF8000

#define RESET            OP_RESET,      1, { 0xFE0000 }, 0xFFFFFF

#define RETFIE           OP_RETFIE,     1, { 0x064000 }, 0xFFFFFF

#define RETLW_B          OP_RETLW_B,    1, { 0x054000 }, 0xFFC000
#define RETLW_W          OP_RETLW_W,    1, { 0x050000 }, 0xFFC000

#define RETURN           OP_RETURN,     1, { 0x060000 }, 0xFFFFFF

#define RLCFF_B          OP_RLCFF_B,    1, { 0xD6E000 }, 0xFFE000
#define RLCFW_B          OP_RLCFW_B,    1, { 0xD6C000 }, 0xFFE000
#define RLC_B            OP_RLC_B,      1, { 0xD2C000 }, 0xFFC000

#define RLCFF_W          OP_RLCFF_W,    1, { 0xD6A000 }, 0xFFE000
#define RLCFW_W          OP_RLCFW_W,    1, { 0xD68000 }, 0xFFE000
#define RLC_W            OP_RLC_W,      1, { 0xD28000 }, 0xFFC000

#define RLNCFF_B         OP_RLNCFF_B,   1, { 0xD66000 }, 0xFFE000
#define RLNCFW_B         OP_RLNCFW_B,   1, { 0xD64000 }, 0xFFE000
#define RLNC_B           OP_RLNC_B,     1, { 0xD24000 }, 0xFFC000

#define RLNCFF_W         OP_RLNCFF_W,   1, { 0xD62000 }, 0xFFE000
#define RLNCFW_W         OP_RLNCFW_W,   1, { 0xD60000 }, 0xFFE000
#define RLNC_W           OP_RLNC_W,     1, { 0xD20000 }, 0xFFC000

#define RRCFF_B          OP_RRCFF_B,    1, { 0xD7E000 }, 0xFFE000
#define RRCFW_B          OP_RRCFW_B,    1, { 0xD7C000 }, 0xFFE000
#define RRC_B            OP_RRC_B,      1, { 0xD3C000 }, 0xFFC000

#define RRCFF_W          OP_RRCFF_W,    1, { 0xD7A000 }, 0xFFE000
#define RRCFW_W          OP_RRCFW_W,    1, { 0xD78000 }, 0xFFE000
#define RRC_W            OP_RRC_W,      1, { 0xD38000 }, 0xFFC000

#define RRNCFF_B         OP_RRNCFF_B,   1, { 0xD76000 }, 0xFFE000
#define RRNCFW_B         OP_RRNCFW_B,   1, { 0xD74000 }, 0xFFE000
#define RRNC_B           OP_RRNC_B,     1, { 0xD34000 }, 0xFFC000

#define RRNCFF_W         OP_RRNCFF_W,   1, { 0xD72000 }, 0xFFE000
#define RRNCFW_W         OP_RRNCFW_W,   1, { 0xD70000 }, 0xFFE000
#define RRNC_W           OP_RRNC_W,     1, { 0xD30000 }, 0xFFC000

#define SAC_PS          OP_SAC_PS,     1, { 0xCC0000 }, 0xFF0000
#define SAC             OP_SAC,        1, { 0xCC0000 }, 0xFF0780

#define SAC_R_PS        OP_SAC_R_PS,   1, { 0xCD0000 }, 0xFF0000
#define SAC_R           OP_SAC_R,      1, { 0xCD0000 }, 0xFF0780

#define SE_W            OP_SE_W,       1, { 0xFB0000 }, 0xFFF800

#define SETW_B          OP_SETW_B,     1, { 0xEFC000 }, 0xFFFFFF
#define SET_B           OP_SET_B,      1, { 0xEBC000 }, 0xFFC07F
#define SETF_B          OP_SETF_B,     1, { 0xEFE000 }, 0xFFE000

#define SETW_W          OP_SETW_W,     1, { 0xEF8000 }, 0xFFFFFF
#define SET_W           OP_SET_W,      1, { 0xEB8000 }, 0xFFC07F
#define SETF_W          OP_SETF_W,     1, { 0xEFA000 }, 0xFFE000

#define SFTACW          OP_SFTACW,     1, { 0xC80000 }, 0xFF7FF0
#define SFTACK          OP_SFTACK,     1, { 0xC80040 }, 0xFF7FC0

#define SLFF_B          OP_SLFF_B,     1, { 0xD46000 }, 0xFFE000
#define SLFW_B          OP_SLFW_B,     1, { 0xD44000 }, 0xFFE000
#define SL_B            OP_SL_B,       1, { 0xD04000 }, 0xFFC000

#define SLFF_W          OP_SLFF_W,     1, { 0xD42000 }, 0xFFE000
#define SLFW_W          OP_SLFW_W,     1, { 0xD40000 }, 0xFFE000
#define SL_W            OP_SL_W,       1, { 0xD00000 }, 0xFFC000
#define SLW_W           OP_SLW_W,      1, { 0xDD0000 }, 0xFF8070
#define SLK_W           OP_SLK_W,      1, { 0xDD0040 }, 0xFF8070

#define SUBWFF_B        OP_SUBWFF_B,   1, { 0xB56000 }, 0xFFE000
#define SUBWFW_B        OP_SUBWFW_B,   1, { 0xB54000 }, 0xFFE000
#define SUBLW_B         OP_SUBLW_B,    1, { 0xB14000 }, 0xFFC000
#define SUBLS_B         OP_SUBLS_B,    1, { 0x504060 }, 0xF84060
#define SUB_B           OP_SUB_B,      1, { 0x504000 }, 0xF84000

#define SUBWFF_W        OP_SUBWFF_W,   1, { 0xB52000 }, 0xFFE000
#define SUBWFW_W        OP_SUBWFW_W,   1, { 0xB50000 }, 0xFFE000
#define SUBLW_W         OP_SUBLW_W,    1, { 0xB10000 }, 0xFFC000
#define SUBLS_W         OP_SUBLS_W,    1, { 0x500060 }, 0xF84060
#define SUB_W           OP_SUB_W,      1, { 0x500000 }, 0xF84000
#define SUBAB           OP_SUBAB,      1, { 0xCB3000 }, 0xFF7000

#define SUBBWFF_B       OP_SUBBWFF_B,  1, { 0xB5E000 }, 0xFFE000
#define SUBBWFW_B       OP_SUBBWFW_B,  1, { 0xB5C000 }, 0xFFE000
#define SUBBLW_B        OP_SUBBLW_B,   1, { 0xB1C000 }, 0xFFC000
#define SUBBLS_B        OP_SUBBLS_B,   1, { 0x584060 }, 0xF84060
#define SUBB_B          OP_SUBB_B,     1, { 0x584000 }, 0xF84000

#define SUBBWFF_W       OP_SUBBWFF_W,  1, { 0xB5A000 }, 0xFFE000
#define SUBBWFW_W       OP_SUBBWFW_W,  1, { 0xB58000 }, 0xFFE000
#define SUBBLW_W        OP_SUBBLW_W,   1, { 0xB18000 }, 0xFFC000
#define SUBBLS_W        OP_SUBBLS_W,   1, { 0x580060 }, 0xF84060
#define SUBB_W          OP_SUBB_W,     1, { 0x580000 }, 0xF84000

#define SUBRWFF_B       OP_SUBRWFF_B,  1, { 0xBD6000 }, 0xFFE000
#define SUBRWFW_B       OP_SUBRWFW_B,  1, { 0xBD4000 }, 0xFFE000
#define SUBRLS_B        OP_SUBRLS_B,   1, { 0x104060 }, 0xF84060
#define SUBR_B          OP_SUBR_B,     1, { 0x104000 }, 0xF84000

#define SUBRWFF_W       OP_SUBRWFF_W,  1, { 0xBD2000 }, 0xFFE000
#define SUBRWFW_W       OP_SUBRWFW_W,  1, { 0xBD0000 }, 0xFFE000
#define SUBRLS_W        OP_SUBRLS_W,   1, { 0x100060 }, 0xF84060
#define SUBR_W          OP_SUBR_W,     1, { 0x100000 }, 0xF84000

#define SUBBRWFF_B      OP_SUBBRWFF_B, 1, { 0xBDE000 }, 0xFFE000
#define SUBBRWFW_B      OP_SUBBRWFW_B, 1, { 0xBDC000 }, 0xFFE000
#define SUBBRLS_B       OP_SUBBRLS_B,  1, { 0x184060 }, 0xF84060
#define SUBBR_B         OP_SUBBR_B,    1, { 0x184000 }, 0xF84000

#define SUBBRWFF_W      OP_SUBBRWFF_W, 1, { 0xBDA000 }, 0xFFE000
#define SUBBRWFW_W      OP_SUBBRWFW_W, 1, { 0xBD8000 }, 0xFFE000
#define SUBBRLS_W       OP_SUBBRLS_W,  1, { 0x180060 }, 0xF84060
#define SUBBR_W         OP_SUBBR_W,    1, { 0x180000 }, 0xF84000

#define SWAP_B          OP_SWAP_B,     1, { 0xFDC000 }, 0xFFFFF0
#define SWAP_W          OP_SWAP_W,     1, { 0xFD8000 }, 0xFFFFF0

#define TBLRDH_B        OP_TBLRDH_B,   1, { 0xBAC000 }, 0xFFC000
#define TBLRDH_W        OP_TBLRDH_W,   1, { 0xBA8000 }, 0xFFC000

#define TBLRDL_B        OP_TBLRDL_B,   1, { 0xBA4000 }, 0xFFC000
#define TBLRDL_W        OP_TBLRDL_W,   1, { 0xBA0000 }, 0xFFC000

#define TBLWTH_B        OP_TBLWTH_B,   1, { 0xBBC000 }, 0xFFC000
#define TBLWTH_W        OP_TBLWTH_W,   1, { 0xBB8000 }, 0xFFC000

#define TBLWTL_B        OP_TBLWTL_B,   1, { 0xBB4000 }, 0xFFC000
#define TBLWTL_W        OP_TBLWTL_W,   1, { 0xBB0000 }, 0xFFC000

#define ULNK            OP_ULNK,       1, { 0xFA8000 }, 0xFFFFFF

#define XORWFF_B        OP_XORWFF_B,   1, { 0xB6E000 }, 0xFFE000
#define XORWFW_B        OP_XORWFW_B,   1, { 0xB6C000 }, 0xFFE000
#define XORLW_B         OP_XORLW_B,    1, { 0xB2C000 }, 0xFFC000
#define XORLS_B         OP_XORLS_B,    1, { 0x684060 }, 0xF84060
#define XOR_B           OP_XOR_B,      1, { 0x684000 }, 0xF84000

#define XORWFF_W        OP_XORWFF_W,   1, { 0xB6A000 }, 0xFFE000
#define XORWFW_W        OP_XORWFW_W,   1, { 0xB68000 }, 0xFFE000
#define XORLW_W         OP_XORLW_W,    1, { 0xB28000 }, 0xFFC000
#define XORLS_W         OP_XORLS_W,    1, { 0x680060 }, 0xF84060
#define XOR_W           OP_XOR_W,      1, { 0x680000 }, 0xF84000

#define ZE_W            OP_ZE_W,       1, { 0xFB8000 }, 0xFFF800

/******************************************************************************/

#endif /* #ifndef __PIC30_OPC_H__ */
