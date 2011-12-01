#if !defined(ATTR)
#define ATTR(a,b,c)
#endif
#if !defined(ATTR_IS)
#define ATTR_IS(a,b)
#endif
#if !defined(MASK1)
#define MASK1(a,b,c,d,e,f,g,h,i,j)
#endif
#if !defined(MASK2)
#define MASK2(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o)
#endif
#if !defined(MASK3)
#define MASK3(a,b,c,d,e,f,g,h,i,j,k,l,m,n)
#endif
#if !defined(MASK4)
#define MASK4(a,b,c,d)
#endif

/*
 *
 * ATTR: Setting Section Attributes
 *
 *  ATTR(id, has_arg, set_it)
 *
 *  id:        attribute name
 *  has_arg:   (0 = no, 1 = yes)
 *  set_it:    C statement to set the attribute
 *             (refers to variables sec,arg)
 *
 */

#if NO_ARGS
#define PIC30_SET_ABSOLUTE_ATTR_ARG(a) {}
#define PIC30_SET_ALIGN_ATTR_ARG(a) {}
#define PIC30_SET_MERGE_ATTR_ARG(a) {}
#define PIC30_SET_BOOT_ATTR_ARG(a) {}
#define PIC30_SET_SECURE_ATTR_ARG(a) {}
#define PIC30_SET_MEMORY_ATTR_ARG(a) {}
#endif

/*
 * This is the list of "standard" attributes.
 * They are represented in COFF and ELF object
 * files using the customary section flags field.
 *
 * Do not modify this list. New attributes
 * should be added to the extended attribute
 * list which appears below.
 */
ATTR( code,    0, PIC30_SET_CODE_ATTR(sec) )
ATTR( data,    0, PIC30_SET_DATA_ATTR(sec) )
ATTR( bss,     0, PIC30_SET_BSS_ATTR(sec)  )
ATTR( persist, 0, PIC30_SET_PERSIST_ATTR(sec) )
ATTR( psv,     0, PIC30_SET_PSV_ATTR(sec) )
ATTR( eedata,  0, PIC30_SET_EEDATA_ATTR(sec) )
ATTR( memory,  1, PIC30_SET_MEMORY_ATTR(sec)
                  PIC30_SET_MEMORY_ATTR_ARG(arg) )

ATTR( address, 1, PIC30_SET_ABSOLUTE_ATTR(sec)
                  PIC30_SET_ABSOLUTE_ATTR_ARG(arg) )
ATTR( near,    0, PIC30_SET_NEAR_ATTR(sec) )
ATTR( xmemory, 0, PIC30_SET_XMEMORY_ATTR(sec) )
ATTR( ymemory, 0, PIC30_SET_YMEMORY_ATTR(sec) )
ATTR( reverse, 1, PIC30_SET_REVERSE_ATTR(sec)
                  PIC30_SET_ALIGN_ATTR_ARG(arg) )
ATTR( align,   1, PIC30_SET_ALIGN_ATTR_ARG(arg) )
ATTR( noload,  0, PIC30_SET_NOLOAD_ATTR(sec) )
ATTR( merge,   1, PIC30_SET_MERGE_ATTR(sec)
                  PIC30_SET_MERGE_ATTR_ARG(arg) )
ATTR( info,    0, PIC30_SET_INFO_ATTR(sec) )
ATTR( dma,     0, PIC30_SET_DMA_ATTR(sec) )
ATTR( boot,    0, PIC30_SET_BOOT_ATTR(sec)
                  PIC30_SET_BOOT_ATTR_ARG(arg) )
ATTR( secure,  0, PIC30_SET_SECURE_ATTR(sec)
                  PIC30_SET_SECURE_ATTR_ARG(arg) )

/*
 * This is the list of "extended" attributes.
 * They are represented in COFF and ELF object
 * files as a symbol named __ext_flags_secname
 *
 * Do not re-order this list! New attributes
 * must be added to the end. Up to 32 extended
 * attributes are supported.
 *
 * "heap" is ALWAYS the first extended attribute.
 */

ATTR( heap,    0, PIC30_SET_HEAP_ATTR(sec) )
ATTR( stack,   0, PIC30_SET_STACK_ATTR(sec) )
ATTR( eds,     0, PIC30_SET_EDS_ATTR(sec) )
ATTR( page,    0, PIC30_SET_PAGE_ATTR(sec) )
ATTR( auxflash,0, PIC30_SET_AUXFLASH_ATTR(sec) )


/*
 * ATTR_IS: Testing Section Attributes
 *
 *  ATTR_IS(id, test_it)
 *
 *  id:        attribute name
 *  test_it:   C statement to test the attribute
 *
 */

ATTR_IS( code,    PIC30_IS_CODE_ATTR(sec) )
ATTR_IS( data,    PIC30_IS_DATA_ATTR(sec) )
ATTR_IS( bss,     PIC30_IS_BSS_ATTR(sec)  )
ATTR_IS( persist, PIC30_IS_PERSIST_ATTR(sec) )
ATTR_IS( psv,     PIC30_IS_PSV_ATTR(sec) )
ATTR_IS( eedata,  PIC30_IS_EEDATA_ATTR(sec) )
ATTR_IS( memory,  PIC30_IS_MEMORY_ATTR(sec) )

ATTR_IS( address, PIC30_IS_ABSOLUTE_ATTR(sec) )
ATTR_IS( near,    PIC30_IS_NEAR_ATTR(sec) )
ATTR_IS( xmemory, PIC30_IS_XMEMORY_ATTR(sec) )
ATTR_IS( ymemory, PIC30_IS_YMEMORY_ATTR(sec) )
ATTR_IS( reverse, PIC30_IS_REVERSE_ATTR(sec) )
/* can't test for align */
ATTR_IS( noload,  PIC30_IS_NOLOAD_ATTR(sec) )
ATTR_IS( merge,   PIC30_IS_MERGE_ATTR(sec) )
ATTR_IS( info,    PIC30_IS_INFO_ATTR(sec) )
ATTR_IS( dma,     PIC30_IS_DMA_ATTR(sec) )
ATTR_IS( boot,    PIC30_IS_BOOT_ATTR(sec) )
ATTR_IS( secure,  PIC30_IS_SECURE_ATTR(sec) )

ATTR_IS( heap,    PIC30_IS_HEAP_ATTR(sec) )
ATTR_IS( stack,   PIC30_IS_STACK_ATTR(sec) )
ATTR_IS( eds,     PIC30_IS_EDS_ATTR(sec) )
ATTR_IS( page,    PIC30_IS_PAGE_ATTR(sec) )
ATTR_IS( auxflash,PIC30_IS_AUXFLASH_ATTR(sec) )




/*
 * MASK1: Attributes that represent section types
 */
MASK1( code, data, bss, persist, psv, eedata, memory, heap, stack, auxflash)


/*
 * MASK2: Attributes that modify section types
 *
 * MASK2( type, modifier1, ... modifier14 )
 */
MASK2( code, address, 0, 0, 0, 0, align, noload, merge, info, 0, boot, secure, 0, page )
MASK2( data, address, near, xmemory, ymemory, reverse, align, noload, merge, info, dma, 0, 0, eds, page )
MASK2( bss, address, near, xmemory, ymemory, reverse, align, noload, 0, info, dma, boot, secure, eds, page )
MASK2( persist, address, near, xmemory, ymemory, reverse, align, noload, 0, 0, dma, 0, 0, eds, page )
MASK2( psv, address, 0, 0, 0, reverse, align, noload, merge, 0, 0, boot, secure, 0, page )
MASK2( eedata, address, 0, 0, 0, reverse, align, noload, merge, 0, 0, boot, secure, 0, 0 )
MASK2( memory, address, 0, 0, 0, reverse, align, noload, 0, 0, 0, 0, 0, 0, 0 )
MASK2( heap, address, xmemory, ymemory, 0, 0, align, 0, 0, 0, 0, 0, 0, 0, 0 )
MASK2( stack, address, 0, 0, 0, 0, align, 0, 0, 0, 0, 0, 0, 0, 0 )
MASK2( auxflash, address, 0, 0, 0, 0, align, noload, merge, info, 0, 0, 0, 0, page )
/*
 * MASK3: Attributes that may be used in combination
 *
 * MASK3( key, attr1, ... attr13)
 */
MASK3( address, near, xmemory, ymemory, 0, 0, noload, 0, 0, dma, boot, secure, eds, page )
MASK3( near, address, xmemory, ymemory, reverse, align, noload, merge, 0, 0, 0, 0, 0, 0 )
MASK3( xmemory, address, near, 0, reverse, align, noload, merge, 0, 0, 0, 0, eds, page )
MASK3( ymemory, address, near, 0, reverse, align, noload, merge, 0, 0, 0, 0, eds, page )
MASK3( reverse, 0, near, xmemory, ymemory, 0, noload, merge, 0, dma, boot, secure, eds, page )
MASK3( align, address, near, xmemory, ymemory, 0, noload, merge, 0, dma, boot, secure, eds, page )
MASK3( noload, address, near, xmemory, ymemory, reverse, align, 0, 0, dma, boot, secure, eds, page )
MASK3( merge, 0, near, xmemory, ymemory, reverse, align, 0, 0, 0, 0, 0, eds, page )
MASK3( dma, address, 0, 0, 0,  reverse, align, noload, 0, 0, 0, 0, eds, page )
MASK3( boot, address, 0, 0, reverse, align, noload, 0, 0, 0, 0, 0, eds, page )
MASK3( secure, address, 0, 0, reverse, align, noload, 0, 0, 0, 0, 0, eds, page )
MASK3( eds, address, near, xmemory, ymemory, reverse, align, noload, merge, 0, dma, boot, secure, page )
MASK3( page, address, near, xmemory, ymemory, reverse, align, noload, merge, 0, dma, boot, secure, eds )
MASK3( info, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )

/*
 * MASK4: Reserved section names with implied attributes
 *
 *  MASK4( name, quiet, type, modifier)
 *
 *  name:     reserved section name
 *  quiet:    set implied attribute(s) quietly?
 *            (0 = no, 1 = yes)
 *  type:     attribute that describes type
 *  modifier: attribute that modifies type
 *   
 */
MASK4( .text,    1, code, 0 )
MASK4( .data,    1, data, 0 )
MASK4( .bss,     1, bss, 0 )
MASK4( .xbss,    0, bss, xmemory )
MASK4( .xdata,   0, data, xmemory )
MASK4( .nbss,    0, bss, near )
MASK4( .ndata,   0, data, near )
MASK4( .ndconst, 0, data, near )
MASK4( .pbss,    0, persist, 0 )
MASK4( .dconst,  0, data, 0 )
MASK4( .ybss,    0, bss, ymemory )
MASK4( .ydata,   0, data, ymemory )
MASK4( .const,   0, psv, 0 )
MASK4( .eedata,  0, eedata, 0 )

