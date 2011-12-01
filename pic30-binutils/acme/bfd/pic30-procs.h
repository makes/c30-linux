/*
** Note: this file is no longer used -- 10/31/2008 GM
*/
#if !defined(PROC_FLAGS)

/* processor attribute flags */
#define F_NONE                      (0)
#define F_DSP                       (1 << 0)
#define F_EEDATA                    (1 << 1)
#define F_SMPS                      (1 << 2)
#define F_DMA                       (1 << 3)
#define F___                        (1 << 4)
#define F____                       (1 << 5)
#define F_30F                       (1 << 6)
#define F_33F                       (1 << 7)
#define F_24F                       (1 << 8)
#define F_24H                       (1 << 9)

#define FAMILY_MASK                 (F_30F | F_33F | F_24F | F_24H)

#define PROC_FLAGS
#endif

#if !defined(ARCH)
#define ARCH(machine, printable_name, default_machine, next, flags)
#endif

#if !defined(EOL)
#define EOL
#endif

ARCH ( bfd_mach_P30F2010,  "30F2010",  FALSE, &arch_info_struct[1],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F2011,  "30F2011",  FALSE, &arch_info_struct[2],
       F_30F | F_DSP ) EOL

ARCH ( bfd_mach_P30F2012,  "30F2012",  FALSE, &arch_info_struct[3],
       F_30F | F_DSP ) EOL

ARCH ( bfd_mach_P30F3010,  "30F3010",  FALSE, &arch_info_struct[4],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F3011,  "30F3011",  FALSE, &arch_info_struct[5],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F3012,  "30F3012",  FALSE, &arch_info_struct[6],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F3013,  "30F3013",  FALSE, &arch_info_struct[7],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F3014,  "30F3014",  FALSE, &arch_info_struct[8],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F5015,  "30F5015",  FALSE, &arch_info_struct[9],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F4011,  "30F4011",  FALSE, &arch_info_struct[10],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F4012,  "30F4012",  FALSE, &arch_info_struct[11],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F4013,  "30F4013",  FALSE, &arch_info_struct[12],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F4014,  "30F4014",  FALSE, &arch_info_struct[13],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F4015,  "30F4015",  FALSE, &arch_info_struct[14],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F5010,  "30F5010",  FALSE, &arch_info_struct[15],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F5011,  "30F5011",  FALSE, &arch_info_struct[16],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F5012,  "30F5012",  FALSE, &arch_info_struct[17],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F5013,  "30F5013",  FALSE, &arch_info_struct[18],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F5014,  "30F5014",  FALSE, &arch_info_struct[19],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F5016,  "30F5016",  FALSE, &arch_info_struct[20],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F6010,  "30F6010",  FALSE, &arch_info_struct[21],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F6010A, "30F6010A", FALSE, &arch_info_struct[22],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F6011,  "30F6011",  FALSE, &arch_info_struct[23],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F6011A, "30F6011A", FALSE, &arch_info_struct[24],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F6012,  "30F6012",  FALSE, &arch_info_struct[25],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F6012A, "30F6012A", FALSE, &arch_info_struct[26],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F6013,  "30F6013",  FALSE, &arch_info_struct[27],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F6013A, "30F6013A", FALSE, &arch_info_struct[28],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F6014,  "30F6014",  FALSE, &arch_info_struct[29],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F6014A, "30F6014A", FALSE, &arch_info_struct[30],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P30F6015,  "30F6015",  FALSE, &arch_info_struct[31],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_P24FJ64GA006,  "24FJ64GA006",  FALSE, &arch_info_struct[32],
       F_24F ) EOL

ARCH ( bfd_mach_P24FJ64GA008,  "24FJ64GA008",  FALSE, &arch_info_struct[33],
       F_24F ) EOL

ARCH ( bfd_mach_P24FJ64GA010,  "24FJ64GA010",  FALSE, &arch_info_struct[34],
       F_24F ) EOL

ARCH ( bfd_mach_P24FJ96GA006,  "24FJ96GA006",  FALSE, &arch_info_struct[35],
       F_24F ) EOL

ARCH ( bfd_mach_P24FJ96GA008,  "24FJ96GA008",  FALSE, &arch_info_struct[36],
       F_24F ) EOL

ARCH ( bfd_mach_P24FJ96GA010,  "24FJ96GA010",  FALSE, &arch_info_struct[37],
       F_24F ) EOL

ARCH ( bfd_mach_P24FJ128GA006,  "24FJ128GA006",  FALSE, &arch_info_struct[38],
       F_24F ) EOL

ARCH ( bfd_mach_P24FJ128GA008,  "24FJ128GA008",  FALSE, &arch_info_struct[39],
       F_24F ) EOL

ARCH ( bfd_mach_P24FJ128GA010,  "24FJ128GA010",  FALSE, &arch_info_struct[40],
       F_24F ) EOL

ARCH ( bfd_mach_P33FJ64GP306,  "33FJ64GP306",  FALSE, &arch_info_struct[41],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ64GP308,  "33FJ64GP308",  FALSE, &arch_info_struct[42],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ64GP310,  "33FJ64GP310",  FALSE, &arch_info_struct[43],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ64GP506,  "33FJ64GP506",  FALSE, &arch_info_struct[44],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ64GP508,  "33FJ64GP508",  FALSE, &arch_info_struct[45],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ64GP510,  "33FJ64GP510",  FALSE, &arch_info_struct[46],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ128GP306,  "33FJ128GP306",  FALSE, &arch_info_struct[47],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ128GP308,  "33FJ128GP308",  FALSE, &arch_info_struct[48],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ128GP310,  "33FJ128GP310",  FALSE, &arch_info_struct[49],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ128GP506,  "33FJ128GP506",  FALSE, &arch_info_struct[50],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ128GP508,  "33FJ128GP508",  FALSE, &arch_info_struct[51],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ128GP510,  "33FJ128GP510",  FALSE, &arch_info_struct[52],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ256GP506,  "33FJ256GP506",  FALSE, &arch_info_struct[53],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ256GP508,  "33FJ256GP508",  FALSE, &arch_info_struct[54],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ256GP510,  "33FJ256GP510",  FALSE, &arch_info_struct[55],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ64MC306,  "33FJ64MC306",  FALSE, &arch_info_struct[56],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ64MC308,  "33FJ64MC308",  FALSE, &arch_info_struct[57],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ64MC310,  "33FJ64MC310",  FALSE, &arch_info_struct[58],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ64MC506,  "33FJ64MC506",  FALSE, &arch_info_struct[59],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ64MC508,  "33FJ64MC508",  FALSE, &arch_info_struct[60],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ64MC510,  "33FJ64MC510",  FALSE, &arch_info_struct[61],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ128MC306,  "33FJ128MC306",  FALSE, &arch_info_struct[62],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ128MC308,  "33FJ128MC308",  FALSE, &arch_info_struct[63],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ128MC310,  "33FJ128MC310",  FALSE, &arch_info_struct[64],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ128MC506,  "33FJ128MC506",  FALSE, &arch_info_struct[65],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ128MC508,  "33FJ128MC508",  FALSE, &arch_info_struct[66],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ128MC510,  "33FJ128MC510",  FALSE, &arch_info_struct[67],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ256MC506,  "33FJ256MC506",  FALSE, &arch_info_struct[68],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ256MC508,  "33FJ256MC508",  FALSE, &arch_info_struct[69],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ256MC510,  "33FJ256MC510",  FALSE, &arch_info_struct[70],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P30F2020,  "30F2020",  FALSE, &arch_info_struct[71],
       F_30F | F_SMPS | F_DSP ) EOL

ARCH ( bfd_mach_P30F2021,  "30F2021",  FALSE, &arch_info_struct[72],
       F_30F | F_SMPS | F_DSP ) EOL

ARCH ( bfd_mach_P30F2022,  "30F2022",  FALSE, &arch_info_struct[73],
       F_30F | F_SMPS | F_DSP ) EOL

ARCH ( bfd_mach_P30F2023,  "30F2023",  FALSE, &arch_info_struct[74],
       F_30F | F_SMPS | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ64GP206,  "33FJ64GP206",  FALSE, &arch_info_struct[75],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ64GP706,  "33FJ64GP706",  FALSE, &arch_info_struct[76],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ64GP708,  "33FJ64GP708",  FALSE, &arch_info_struct[77],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ64GP710,  "33FJ64GP710",  FALSE, &arch_info_struct[78],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ128GP706,  "33FJ128GP706",  FALSE, &arch_info_struct[79],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ128GP708,  "33FJ128GP708",  FALSE, &arch_info_struct[80],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ128GP710,  "33FJ128GP710",  FALSE, &arch_info_struct[81],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ256GP710,  "33FJ256GP710",  FALSE, &arch_info_struct[82],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ64MC706,  "33FJ64MC706",  FALSE, &arch_info_struct[83],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ64MC710,  "33FJ64MC710",  FALSE, &arch_info_struct[84],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ128MC706,  "33FJ128MC706",  FALSE, &arch_info_struct[85],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ128MC708,  "33FJ128MC708",  FALSE, &arch_info_struct[86],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ128MC710,  "33FJ128MC710",  FALSE, &arch_info_struct[87],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_P33FJ256MC710,  "33FJ256MC710",  FALSE, &arch_info_struct[88],
       F_33F | F_DMA  | F_DSP ) EOL

ARCH ( bfd_mach_GENERIC_16BIT,  "GENERIC-16BIT",  FALSE, &arch_info_struct[89],
       F_NONE ) EOL

ARCH ( bfd_mach_P24HJ64GP206,  "24HJ64GP206",  FALSE, &arch_info_struct[90],
       F_24H | F_DMA ) EOL

ARCH ( bfd_mach_P24HJ64GP210,  "24HJ64GP210",  FALSE, &arch_info_struct[91],
       F_24H | F_DMA ) EOL

ARCH ( bfd_mach_P24HJ256GP206,  "24HJ256GP206",  FALSE, &arch_info_struct[92],
       F_24H | F_DMA ) EOL

ARCH ( bfd_mach_P24HJ256GP610,  "24HJ256GP610",  FALSE, &arch_info_struct[93],
       F_24H | F_DMA ) EOL

ARCH ( bfd_mach_P24HJ64GP506,  "24HJ64GP506",  FALSE, &arch_info_struct[94],
       F_24H | F_DMA ) EOL

ARCH ( bfd_mach_P24HJ64GP510,  "24HJ64GP510",  FALSE, &arch_info_struct[95],
       F_24H | F_DMA ) EOL

ARCH ( bfd_mach_P24HJ128GP206,  "24HJ128GP206",  FALSE, &arch_info_struct[96],
       F_24H | F_DMA ) EOL

ARCH ( bfd_mach_P24HJ128GP306,  "24HJ128GP306",  FALSE, &arch_info_struct[97],
       F_24H | F_DMA ) EOL

ARCH ( bfd_mach_P24HJ128GP310,  "24HJ128GP310",  FALSE, &arch_info_struct[98],
       F_24H | F_DMA ) EOL

ARCH ( bfd_mach_P24HJ128GP506,  "24HJ128GP506",  FALSE, &arch_info_struct[99],
       F_24H | F_DMA ) EOL

ARCH ( bfd_mach_P24HJ128GP510,  "24HJ128GP510",  FALSE, &arch_info_struct[100],
       F_24H | F_DMA ) EOL

ARCH ( bfd_mach_P24HJ256GP210,  "24HJ256GP210",  FALSE, &arch_info_struct[101],
       F_24H | F_DMA ) EOL

ARCH ( bfd_mach_P30F2005,  "30F2005",  FALSE, &arch_info_struct[102],
       F_30F | F_DSP ) EOL

ARCH ( bfd_mach_P30F2010A,  "30F2010A",  FALSE, &arch_info_struct[103],
       F_30F | F_DSP | F_EEDATA ) EOL

ARCH ( bfd_mach_GENERIC_16DSP,  "GENERIC-16DSP",  FALSE, &arch_info_struct[104],
       F_DSP ) EOL

ARCH ( bfd_mach_last, "none", FALSE, 0, F_NONE) EOL
  
