cat <<EOF
/*
** Default Linker Script for dsPIC
*/

OUTPUT_ARCH("${ARCH}")
CRT0_STARTUP(crt0_standard.o)
CRT1_STARTUP(crt1_standard.o)

/*
** Memory Regions
*/
MEMORY
{
  data  (a!xr) : ORIGIN = 0x800,    LENGTH = 63488
  program (xr) : ORIGIN = 0x100,    LENGTH = ((128K * 2) - 0x100)
  reset        : ORIGIN = 0,        LENGTH = (4)
  ivt          : ORIGIN = 0x04,     LENGTH = (62 * 2)
  aivt         : ORIGIN = 0x84,     LENGTH = (62 * 2)
  __FOSC       : ORIGIN = 0xF80000, LENGTH = (2)
  __FWDT       : ORIGIN = 0xF80002, LENGTH = (2)
  __FBORPOR    : ORIGIN = 0xF80004, LENGTH = (2)
  __CONFIG4    : ORIGIN = 0xF80006, LENGTH = (2)
  __CONFIG5    : ORIGIN = 0xF80008, LENGTH = (2)
  __FGS        : ORIGIN = 0xF8000A, LENGTH = (2)
  __FICD       : ORIGIN = 0xF8000C, LENGTH = (2)
  __FUID0      : ORIGIN = 0x8005C0, LENGTH = (2)
  __FUID1      : ORIGIN = 0x8005C2, LENGTH = (2)
  __FUID2      : ORIGIN = 0x8005C4, LENGTH = (2)
  __FUID3      : ORIGIN = 0x8005C6, LENGTH = (2)
  eedata       : ORIGIN = 0x7FF000, LENGTH = (4096)
}


/*
** Config Word Addresses
*/
__FOSC        = 0xF80000;
__FWDT        = 0xF80002;
__FBORPOR     = 0xF80004;
__CONFIG4     = 0xF80006;
__CONFIG5     = 0xF80008;
__FGS         = 0xF8000A;
__FICD        = 0xF8000C;
__FUID0       = 0x8005C0;
__FUID1       = 0x8005C2;
__FUID2       = 0x8005C4;
__FUID3       = 0x8005C6;


/*
** Base Memory Addresses - Program Memory
*/
__RESET_BASE  = 0;        /* Reset Instruction                */
__IVT_BASE    = 0x04;     /* Interrupt Vector Table           */
__AIVT_BASE   = 0x84;     /* Alternate Interrupt Vector Table */
__CODE_BASE   = 0x100;    /* Handles, User Code, Library Code */


/*
** Base Memory Addresses - Data Memory
*/
__SFR_BASE    = 0;        /* Memory-mapped SFRs                 */
__DATA_BASE   = 0x800;    /* X and General Purpose Data Memory  */
__YDATA_BASE  = 0x3000;   /* Y Data Memory for DSP Instructions */


/*
** ==================== Section Map ======================
*/

SECTIONS
{

  /*
  ** ================== Program Memory =====================
  */


  /*
  ** Reset Instruction
  */
  .reset __RESET_BASE :
  {
        SHORT(ABSOLUTE(__reset));
        SHORT(0x04);
        SHORT((ABSOLUTE(__reset) >> 16) & 0x7F);
        SHORT(0);
  } >reset


  /*
  ** Interrupt Vector Tables
  **
  ** The primary and alternate tables are loaded
  ** here, between sections .reset and .text.
  ** Vector table source code appears below.
  */


  /*
  ** User Code and Library Code
  **
  ** This section must not be assigned to __CODE_BASE,
  ** because CodeGuard(tm) sections may be located there.
  **
  ** Note that input sections *(.text) are not mapped here.
  ** The best-fit allocator locates them, so that .text
  ** may flow around PSV sections as needed.
  */
  .text :
  {
        *(.init);
        *(.user_init);
        *(.handle);
        *(.libc) *(.libm) *(.libdsp);  /* keep together in this order */
        *(.lib*);
  } >program


  /*
  ** User-Defined Section in Program Memory
  **
  ** note: can specify an address using
  **       the following syntax:
  **
  **       usercode 0x1234 :
  **         {
  **           *(usercode);
  **         } >program
  */
  usercode :
  {
        *(usercode);
  } >program


  /*
  ** User-Defined Constants in Program Memory
  **
  ** For PSV type sections, the Load Memory Address (LMA)
  ** should be specified as follows:
  **
  **       userconst : AT(0x1234)
  **         {
  **           *(userconst);
  **         } >program
  **
  ** Note that mapping PSV sections in linker scripts
  ** is not generally recommended.
  **
  ** Because of page alignment restrictions, memory is
  ** often used more efficiently when PSV sections
  ** do not appear in the linker script.
  **
  ** For more information on memory allocation,
  ** please refer to chapter 10, 'Linker Processing'
  ** in the Assembler, Linker manual (DS51317).
  */


  /*
  ** ================ Configuration Memory ================
  */


  /*
  ** Configuration Fuses
  */
  __FOSC :
  { *(__FOSC.sec)    } >__FOSC
  __FWDT :
  { *(__FWDT.sec)    } >__FWDT
  __FBORPOR :
  { *(__FBORPOR.sec) } >__FBORPOR
  __CONFIG4 :
  { *(__CONFIG4.sec) } >__CONFIG4
  __CONFIG5 :
  { *(__CONFIG5.sec) } >__CONFIG5
  __FGS :
  { *(__FGS.sec)     } >__FGS
  __FICD :
  { *(__FICD.sec)    } >__FICD
  __FUID0 :
  { *(__FUID0.sec)   } >__FUID0
  __FUID1 :
  { *(__FUID1.sec)   } >__FUID1
  __FUID2 :
  { *(__FUID2.sec)   } >__FUID2
  __FUID3 :
  { *(__FUID3.sec)   } >__FUID3


  /*
  ** User-Defined Section in Data Flash Memory
  **
  ** note: can specify an address using
  **       the following syntax:
  **
  **       user_eedata 0x7FF000 :
  **         {
  **           *(user_eedata);
  **         } >eedata
  */
  user_eedata :
  {
        *(user_eedata);
  } >eedata
 

  /*
  ** ==================== Data Memory ===================
  */


  /* 
  ** ICD Debug Exec
  **
  ** This section provides optional storage for
  ** the ICD2 debugger. Define a global symbol
  ** named __ICD2RAM to enable ICD2. This section
  ** must be loaded at data address 0x800.
  */ 
  .icd __DATA_BASE (NOLOAD): 
  { 
    . += (DEFINED (__ICD2RAM) ? 0x50 : 0 ); 
  } > data 


  /*
  ** User-Defined Section in Data Memory
  **
  ** note: can specify an address using
  **       the following syntax:
  **
  **       userdata 0x1234 :
  **         {
  **           *(userdata);
  **         } >data
  */
  userdata :
  {
        *(userdata);
  } >data


  /*
  ** ===================== Debug Info ====================
  */

  .comment        0 : { *(.comment) }

  /*
  ** DWARF-2
  */
  .debug_info     0 : { *(.debug_info) *(.gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  .debug_ranges   0 : { *(.debug_ranges) }
  .debug_aranges  0 : { *(.debug_aranges) }

} /* SECTIONS */

/*
** ================= End of Section Map ================
*/


/*
** Section Map for Interrupt Vector Tables
*/
SECTIONS
{

/*
** Primary Interrupt Vector Table
*/
.ivt __IVT_BASE :
  {
    LONG(DEFINED(__ReservedTrap0)  ? ABSOLUTE(__ReservedTrap0)  :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__OscillatorFail) ? ABSOLUTE(__OscillatorFail) :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__AddressError)   ? ABSOLUTE(__AddressError)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__StackError)     ? ABSOLUTE(__StackError)     :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__MathError)      ? ABSOLUTE(__MathError)      :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__ReservedTrap5)  ? ABSOLUTE(__ReservedTrap5)  :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__ReservedTrap6)  ? ABSOLUTE(__ReservedTrap6)  :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__ReservedTrap7)  ? ABSOLUTE(__ReservedTrap7)  :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__INT0Interrupt)  ? ABSOLUTE(__INT0Interrupt)  :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__IC1Interrupt)   ? ABSOLUTE(__IC1Interrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__OC1Interrupt)   ? ABSOLUTE(__OC1Interrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__T1Interrupt)    ? ABSOLUTE(__T1Interrupt)    :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__IC2Interrupt)   ? ABSOLUTE(__IC2Interrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__OC2Interrupt)   ? ABSOLUTE(__OC2Interrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__T2Interrupt)    ? ABSOLUTE(__T2Interrupt)    :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__T3Interrupt)    ? ABSOLUTE(__T3Interrupt)    :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__SPI1Interrupt)  ? ABSOLUTE(__SPI1Interrupt)  :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__U1RXInterrupt)  ? ABSOLUTE(__U1RXInterrupt)  :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__U1TXInterrupt)  ? ABSOLUTE(__U1TXInterrupt)  :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__ADCInterrupt)   ? ABSOLUTE(__ADCInterrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__NVMInterrupt)   ? ABSOLUTE(__NVMInterrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__SI2CInterrupt)  ? ABSOLUTE(__SI2CInterrupt)  :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__MI2CInterrupt)  ? ABSOLUTE(__MI2CInterrupt)  :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__CNInterrupt)    ? ABSOLUTE(__CNInterrupt)    :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__INT1Interrupt)  ? ABSOLUTE(__INT1Interrupt)  :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__IC7Interrupt)   ? ABSOLUTE(__IC7Interrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__IC8Interrupt)   ? ABSOLUTE(__IC8Interrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__OC3Interrupt)   ? ABSOLUTE(__OC3Interrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__OC4Interrupt)   ? ABSOLUTE(__OC4Interrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__T4Interrupt)    ? ABSOLUTE(__T4Interrupt)    :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__T5Interrupt)    ? ABSOLUTE(__T5Interrupt)    :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__INT2Interrupt)  ? ABSOLUTE(__INT2Interrupt)  :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__U2RXInterrupt)  ? ABSOLUTE(__U2RXInterrupt)  :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__U2TXInterrupt)  ? ABSOLUTE(__U2TXInterrupt)  :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__SPI2Interrupt)  ? ABSOLUTE(__SPI2Interrupt)  :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__C1Interrupt)    ? ABSOLUTE(__C1Interrupt)    :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__IC3Interrupt)   ? ABSOLUTE(__IC3Interrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__IC4Interrupt)   ? ABSOLUTE(__IC4Interrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__IC5Interrupt)   ? ABSOLUTE(__IC5Interrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__IC6Interrupt)   ? ABSOLUTE(__IC6Interrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__OC5Interrupt)   ? ABSOLUTE(__OC5Interrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__OC6Interrupt)   ? ABSOLUTE(__OC6Interrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__OC7Interrupt)   ? ABSOLUTE(__OC7Interrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__OC8Interrupt)   ? ABSOLUTE(__OC8Interrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__INT3Interrupt)  ? ABSOLUTE(__INT3Interrupt)  :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__INT4Interrupt)  ? ABSOLUTE(__INT4Interrupt)  :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__C2Interrupt)    ? ABSOLUTE(__C2Interrupt)    :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__PWMInterrupt)   ? ABSOLUTE(__PWMInterrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__QEIInterrupt)   ? ABSOLUTE(__QEIInterrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__DCIInterrupt)   ? ABSOLUTE(__DCIInterrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__LVDInterrupt)   ? ABSOLUTE(__LVDInterrupt)   :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__FLTAInterrupt)  ? ABSOLUTE(__FLTAInterrupt)  :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__FLTBInterrupt)  ? ABSOLUTE(__FLTBInterrupt)  :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__Interrupt45)    ? ABSOLUTE(__Interrupt45)    :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__Interrupt46)    ? ABSOLUTE(__Interrupt46)    :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__Interrupt47)    ? ABSOLUTE(__Interrupt47)    :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__Interrupt48)    ? ABSOLUTE(__Interrupt48)    :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__Interrupt49)    ? ABSOLUTE(__Interrupt49)    :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__Interrupt50)    ? ABSOLUTE(__Interrupt50)    :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__Interrupt51)    ? ABSOLUTE(__Interrupt51)    :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__Interrupt52)    ? ABSOLUTE(__Interrupt52)    :
         ABSOLUTE(__DefaultInterrupt));
    LONG(DEFINED(__Interrupt53)    ? ABSOLUTE(__Interrupt53)    :
         ABSOLUTE(__DefaultInterrupt));
  } >ivt


/*
** Alternate Interrupt Vector Table
*/
.aivt __AIVT_BASE :
  {
    LONG(DEFINED(__AltReservedTrap0)  ? ABSOLUTE(__AltReservedTrap0)  :
        (DEFINED(__ReservedTrap0)     ? ABSOLUTE(__ReservedTrap0)     :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltOscillatorFail) ? ABSOLUTE(__AltOscillatorFail) :
        (DEFINED(__OscillatorFail)    ? ABSOLUTE(__OscillatorFail)    :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltAddressError)   ? ABSOLUTE(__AltAddressError)   :
        (DEFINED(__AddressError)      ? ABSOLUTE(__AddressError)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltStackError)     ? ABSOLUTE(__AltStackError)     :
        (DEFINED(__StackError)        ? ABSOLUTE(__StackError)        :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltMathError)      ? ABSOLUTE(__AltMathError)      :
        (DEFINED(__MathError)         ? ABSOLUTE(__MathError)         :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltReservedTrap5)  ? ABSOLUTE(__AltReservedTrap5)  :
        (DEFINED(__ReservedTrap5)     ? ABSOLUTE(__ReservedTrap5)     :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltReservedTrap6)  ? ABSOLUTE(__AltReservedTrap6)  :
        (DEFINED(__ReservedTrap6)     ? ABSOLUTE(__ReservedTrap6)     :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltReservedTrap7)  ? ABSOLUTE(__AltReservedTrap7)  :
        (DEFINED(__ReservedTrap7)     ? ABSOLUTE(__ReservedTrap7)     :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltINT0Interrupt)  ? ABSOLUTE(__AltINT0Interrupt)  :
        (DEFINED(__INT0Interrupt)     ? ABSOLUTE(__INT0Interrupt)     :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltIC1Interrupt)   ? ABSOLUTE(__AltIC1Interrupt)   :
        (DEFINED(__IC1Interrupt)      ? ABSOLUTE(__IC1Interrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltOC1Interrupt)   ? ABSOLUTE(__AltOC1Interrupt)   :
        (DEFINED(__OC1Interrupt)      ? ABSOLUTE(__OC1Interrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltT1Interrupt)    ? ABSOLUTE(__AltT1Interrupt)    :
        (DEFINED(__T1Interrupt)       ? ABSOLUTE(__T1Interrupt)       :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltIC2Interrupt)   ? ABSOLUTE(__AltIC2Interrupt)   :
        (DEFINED(__IC2Interrupt)      ? ABSOLUTE(__IC2Interrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltOC2Interrupt)   ? ABSOLUTE(__AltOC2Interrupt)   :
        (DEFINED(__OC2Interrupt)      ? ABSOLUTE(__OC2Interrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltT2Interrupt)    ? ABSOLUTE(__AltT2Interrupt)    :
        (DEFINED(__T2Interrupt)       ? ABSOLUTE(__T2Interrupt)       :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltT3Interrupt)    ? ABSOLUTE(__AltT3Interrupt)    :
        (DEFINED(__T3Interrupt)       ? ABSOLUTE(__T3Interrupt)       :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltSPI1Interrupt)  ? ABSOLUTE(__AltSPI1Interrupt)  :
        (DEFINED(__SPI1Interrupt)     ? ABSOLUTE(__SPI1Interrupt)     :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltU1RXInterrupt)  ? ABSOLUTE(__AltU1RXInterrupt)  :
        (DEFINED(__U1RXInterrupt)     ? ABSOLUTE(__U1RXInterrupt)     :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltU1TXInterrupt)  ? ABSOLUTE(__AltU1TXInterrupt)  :
        (DEFINED(__U1TXInterrupt)     ? ABSOLUTE(__U1TXInterrupt)     :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltADCInterrupt)   ? ABSOLUTE(__AltADCInterrupt)   :
        (DEFINED(__ADCInterrupt)      ? ABSOLUTE(__ADCInterrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltNVMInterrupt)   ? ABSOLUTE(__AltNVMInterrupt)   :
        (DEFINED(__NVMInterrupt)      ? ABSOLUTE(__NVMInterrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltSI2CInterrupt)  ? ABSOLUTE(__AltSI2CInterrupt)  :
        (DEFINED(__SI2CInterrupt)     ? ABSOLUTE(__SI2CInterrupt)     :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltMI2CInterrupt)  ? ABSOLUTE(__AltMI2CInterrupt)  :
        (DEFINED(__MI2CInterrupt)     ? ABSOLUTE(__MI2CInterrupt)     :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltCNInterrupt)    ? ABSOLUTE(__AltCNInterrupt)    :
        (DEFINED(__CNInterrupt)       ? ABSOLUTE(__CNInterrupt)       :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltINT1Interrupt)  ? ABSOLUTE(__AltINT1Interrupt)  :
        (DEFINED(__INT1Interrupt)     ? ABSOLUTE(__INT1Interrupt)     :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltIC7Interrupt)   ? ABSOLUTE(__AltIC7Interrupt)   :
        (DEFINED(__IC7Interrupt)      ? ABSOLUTE(__IC7Interrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltIC8Interrupt)   ? ABSOLUTE(__AltIC8Interrupt)   :
        (DEFINED(__IC8Interrupt)      ? ABSOLUTE(__IC8Interrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltOC3Interrupt)   ? ABSOLUTE(__AltOC3Interrupt)   :
        (DEFINED(__OC3Interrupt)      ? ABSOLUTE(__OC3Interrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltOC4Interrupt)   ? ABSOLUTE(__AltOC4Interrupt)   :
        (DEFINED(__OC4Interrupt)      ? ABSOLUTE(__OC4Interrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltT4Interrupt)    ? ABSOLUTE(__AltT4Interrupt)    :
        (DEFINED(__T4Interrupt)       ? ABSOLUTE(__T4Interrupt)       :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltT5Interrupt)    ? ABSOLUTE(__AltT5Interrupt)    :
        (DEFINED(__T5Interrupt)       ? ABSOLUTE(__T5Interrupt)       :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltINT2Interrupt)  ? ABSOLUTE(__AltINT2Interrupt)  :
        (DEFINED(__INT2Interrupt)     ? ABSOLUTE(__INT2Interrupt)     :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltU2RXInterrupt)  ? ABSOLUTE(__AltU2RXInterrupt)  :
        (DEFINED(__U2RXInterrupt)     ? ABSOLUTE(__U2RXInterrupt)     :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltU2TXInterrupt)  ? ABSOLUTE(__AltU2TXInterrupt)  :
        (DEFINED(__U2TXInterrupt)     ? ABSOLUTE(__U2TXInterrupt)     :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltSPI2Interrupt)  ? ABSOLUTE(__AltSPI2Interrupt)  :
        (DEFINED(__SPI2Interrupt)     ? ABSOLUTE(__SPI2Interrupt)     :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltC1Interrupt)    ? ABSOLUTE(__AltC1Interrupt)    :
        (DEFINED(__C1Interrupt)       ? ABSOLUTE(__C1Interrupt)       :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltIC3Interrupt)   ? ABSOLUTE(__AltIC3Interrupt)   :
        (DEFINED(__IC3Interrupt)      ? ABSOLUTE(__IC3Interrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltIC4Interrupt)   ? ABSOLUTE(__AltIC4Interrupt)   :
        (DEFINED(__IC4Interrupt)      ? ABSOLUTE(__IC4Interrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltIC5Interrupt)   ? ABSOLUTE(__AltIC5Interrupt)   :
        (DEFINED(__IC5Interrupt)      ? ABSOLUTE(__IC5Interrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltIC6Interrupt)   ? ABSOLUTE(__AltIC6Interrupt)   :
        (DEFINED(__IC6Interrupt)      ? ABSOLUTE(__IC6Interrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltOC5Interrupt)   ? ABSOLUTE(__AltOC5Interrupt)   :
        (DEFINED(__OC5Interrupt)      ? ABSOLUTE(__OC5Interrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltOC6Interrupt)   ? ABSOLUTE(__AltOC6Interrupt)   :
        (DEFINED(__OC6Interrupt)      ? ABSOLUTE(__OC6Interrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltOC7Interrupt)   ? ABSOLUTE(__AltOC7Interrupt)   :
        (DEFINED(__OC7Interrupt)      ? ABSOLUTE(__OC7Interrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltOC8Interrupt)   ? ABSOLUTE(__AltOC8Interrupt)   :
        (DEFINED(__OC8Interrupt)      ? ABSOLUTE(__OC8Interrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltINT3Interrupt)  ? ABSOLUTE(__AltINT3Interrupt)  :
        (DEFINED(__INT3Interrupt)     ? ABSOLUTE(__INT3Interrupt)     :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltINT4Interrupt)  ? ABSOLUTE(__AltINT4Interrupt)  :
        (DEFINED(__INT4Interrupt)     ? ABSOLUTE(__INT4Interrupt)     :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltC2Interrupt)    ? ABSOLUTE(__AltC2Interrupt)    :
        (DEFINED(__C2Interrupt)       ? ABSOLUTE(__C2Interrupt)       :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltPWMInterrupt)   ? ABSOLUTE(__AltPWMInterrupt)   :
        (DEFINED(__PWMInterrupt)      ? ABSOLUTE(__PWMInterrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltQEIInterrupt)   ? ABSOLUTE(__AltQEIInterrupt)   :
        (DEFINED(__QEIInterrupt)      ? ABSOLUTE(__QEIInterrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltDCIInterrupt)   ? ABSOLUTE(__AltDCIInterrupt)   :
        (DEFINED(__DCIInterrupt)      ? ABSOLUTE(__DCIInterrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltLVDInterrupt)   ? ABSOLUTE(__AltLVDInterrupt)   :
        (DEFINED(__LVDInterrupt)      ? ABSOLUTE(__LVDInterrupt)      :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltFLTAInterrupt)  ? ABSOLUTE(__AltFLTAInterrupt)  :
        (DEFINED(__FLTAInterrupt)     ? ABSOLUTE(__FLTAInterrupt)     :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltFLTBInterrupt)  ? ABSOLUTE(__AltFLTBInterrupt)  :
        (DEFINED(__FLTBInterrupt)     ? ABSOLUTE(__FLTBInterrupt)     :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltInterrupt45)    ? ABSOLUTE(__AltInterrupt45)    :
        (DEFINED(__Interrupt45)       ? ABSOLUTE(__Interrupt45)       :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltInterrupt46)    ? ABSOLUTE(__AltInterrupt46)    :
        (DEFINED(__Interrupt46)       ? ABSOLUTE(__Interrupt46)       :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltInterrupt47)    ? ABSOLUTE(__AltInterrupt47)    :
        (DEFINED(__Interrupt47)       ? ABSOLUTE(__Interrupt47)       :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltInterrupt48)    ? ABSOLUTE(__AltInterrupt48)    :
        (DEFINED(__Interrupt48)       ? ABSOLUTE(__Interrupt48)       :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltInterrupt49)    ? ABSOLUTE(__AltInterrupt49)    :
        (DEFINED(__Interrupt49)       ? ABSOLUTE(__Interrupt49)       :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltInterrupt50)    ? ABSOLUTE(__AltInterrupt50)    :
        (DEFINED(__Interrupt50)       ? ABSOLUTE(__Interrupt50)       :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltInterrupt51)    ? ABSOLUTE(__AltInterrupt51)    :
        (DEFINED(__Interrupt51)       ? ABSOLUTE(__Interrupt51)       :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltInterrupt52)    ? ABSOLUTE(__AltInterrupt52)    :
        (DEFINED(__Interrupt52)       ? ABSOLUTE(__Interrupt52)       :
         ABSOLUTE(__DefaultInterrupt)));
    LONG(DEFINED(__AltInterrupt53)    ? ABSOLUTE(__AltInterrupt53)    :
        (DEFINED(__Interrupt53)       ? ABSOLUTE(__Interrupt53)       :
         ABSOLUTE(__DefaultInterrupt)));
  } >aivt

} /* SECTIONS */


/*==========================================================================
**
**       dsPIC Core Register Definitions
**
**=========================================================================*/
 WREG0 = 0x0000;
_WREG0 = 0x0000;
 WREG1 = 0x0002;
_WREG1 = 0x0002;
 WREG2 = 0x0004;
_WREG2 = 0x0004;
 WREG3 = 0x0006;
_WREG3 = 0x0006;
 WREG4 = 0x0008;
_WREG4 = 0x0008;
 WREG5 = 0x000A;
_WREG5 = 0x000A;
 WREG6 = 0x000C;
_WREG6 = 0x000C;
 WREG7 = 0x000E;
_WREG7 = 0x000E;
 WREG8 = 0x0010;
_WREG8 = 0x0010;
 WREG9 = 0x0012;
_WREG9 = 0x0012;
 WREG10 = 0x0014;
_WREG10 = 0x0014;
 WREG11 = 0x0016;
_WREG11 = 0x0016;
 WREG12 = 0x0018;
_WREG12 = 0x0018;
 WREG13 = 0x001A;
_WREG13 = 0x001A;
 WREG14 = 0x001C;
_WREG14 = 0x001C;
 WREG15 = 0x001E;
_WREG15 = 0x001E;
 SPLIM = 0x0020;
_SPLIM = 0x0020;
 ACCAL = 0x0022;
_ACCAL = 0x0022;
 ACCAH = 0x0024;
_ACCAH = 0x0024;
 ACCAU = 0x0026;
_ACCAU = 0x0026;
 ACCBL = 0x0028;
_ACCBL = 0x0028;
 ACCBH = 0x002A;
_ACCBH = 0x002A;
 ACCBU = 0x002C;
_ACCBU = 0x002C;
 PCL = 0x002E;
_PCL = 0x002E;
 PCH = 0x0030;
_PCH = 0x0030;
 TBLPAG = 0x0032;
_TBLPAG = 0x0032;
 PSVPAG = 0x0034;
_PSVPAG = 0x0034;
 RCOUNT = 0x0036;
_RCOUNT = 0x0036;
 DCOUNT = 0x0038;
_DCOUNT = 0x0038;
 DOSTARTL = 0x003A;
_DOSTARTL = 0x003A;
 DOSTARTH = 0x003C;
_DOSTARTH = 0x003C;
 DOENDL = 0x003E;
_DOENDL = 0x003E;
 DOENDH = 0x0040;
_DOENDH = 0x0040;
 SR = 0x0042;
_SR = 0x0042;
 CORCON = 0x0044;
_CORCON = 0x0044;
 MODCON = 0x0046;
_MODCON = 0x0046;
 XMODSRT = 0x0048;
_XMODSRT = 0x0048;
 XMODEND = 0x004A;
_XMODEND = 0x004A;
 YMODSRT = 0x004C;
_YMODSRT = 0x004C;
 YMODEND = 0x004E;
_YMODEND = 0x004E;
 XBREV = 0x0050;
_XBREV = 0x0050;
 DISICNT = 0x0052;
_DISICNT = 0x0052;
/*
**reserved 0x0054 to 0x007E
*/


/*=========================================================================
**
**      Additional SFR Definitions required for C header files.
**
**========================================================================*/
 ACCA = 0x0022;
_ACCA = 0x0022;
 ACCB = 0x0028;
_ACCB = 0x0028;
 SRbits = 0x0042;
_SRbits = 0x0042;
 CORCONbits = 0x0044;
_CORCONbits = 0x0044;
 MODCONbits = 0x0046;
_MODCONbits = 0x0046;
 XBREVbits = 0x0050;
_XBREVbits = 0x0050;
 DISICNTbits = 0x0052;
_DISICNTbits = 0x0052;
_TMR2 = 0x0106;
_T2CON = 0x0110;
_U1MODE = 0x020c;
_U1MODEbits = 0x020c;
_U1STA = 0x20e;
_U1STAbits = 0x20e;
_U1TXREG = 0x0210;
_U1RXREG = 0x0212;
_U1BRG = 0x0214;
_U2MODE = 0x0216;
_U2MODEbits = 0x0216;
_U2STA = 0x0218;
_U2STAbits = 0x0218;
_U2TXREG = 0x021a;
_U2RXREG = 0x021c;
_U2BRG = 0x21e;


EOF
