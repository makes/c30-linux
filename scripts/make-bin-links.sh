#!/bin/bash

if [ ! -f install_prefix ];
then
    echo "error: './configure' must be run first."
    echo ""
    echo "Usage: ./configure [installdir]"
    echo "       make"
    echo "       sudo make install"
    echo ""
    exit 1
fi

prefix=`cat install_prefix`

# gcc
rm -f /usr/local/bin/pic30-gcc
ln -s $prefix/bin/pic30-coff-gcc-4.0.3 /usr/local/bin/pic30-gcc
rm -f /usr/local/bin/pic30-coff-gcc
ln -s $prefix/bin/pic30-coff-gcc-4.0.3 /usr/local/bin/pic30-coff-gcc

rm -f /usr/local/bin/pic30-elf-gcc
ln -s $prefix/bin/pic30-elf-gcc-4.0.3 /usr/local/bin/pic30-elf-gcc

# cc1
rm -f /usr/local/bin/pic30-cc1
ln -s $prefix/libexec/gcc/pic30-coff/4.0.3/pic30-coff-cc1 /usr/local/bin/pic30-cc1
rm -f /usr/local/bin/pic30-coff-cc1
ln -s $prefix/libexec/gcc/pic30-coff/4.0.3/pic30-coff-cc1 /usr/local/bin/pic30-coff-cc1

rm -f /usr/local/bin/pic30-elf-cc1
ln -s $prefix/libexec/gcc/pic30-elf/4.0.3/pic30-elf-cc1 /usr/local/bin/pic30-elf-cc1

# cpp
rm -f /usr/local/bin/pic30-cpp
ln -s $prefix/bin/pic30-coff-pic30-coff-cpp /usr/local/bin/pic30-cpp
rm -f /usr/local/bin/pic30-coff-cpp
ln -s $prefix/bin/pic30-coff-pic30-coff-cpp /usr/local/bin/pic30-coff-cpp

rm -f /usr/local/bin/pic30-elf-cpp
ln -s $prefix/bin/pic30-elf-pic30-elf-cpp /usr/local/bin/pic30-elf-cpp

# gcov
rm -f /usr/local/bin/pic30-gcov
ln -s $prefix/bin/pic30-coff-pic30-coff-gcov /usr/local/bin/pic30-gcov
rm -f /usr/local/bin/pic30-coff-gcov
ln -s $prefix/bin/pic30-coff-pic30-coff-gcov /usr/local/bin/pic30-coff-gcov

rm -f /usr/local/bin/pic30-elf-gcov
ln -s $prefix/bin/pic30-elf-pic30-elf-gcov /usr/local/bin/pic30-elf-gcov

# ld
rm -f /usr/local/bin/pic30-ld
ln -s $prefix/bin/pic30-coff-ld /usr/local/bin/pic30-ld
rm -f /usr/local/bin/pic30-coff-ld
ln -s $prefix/bin/pic30-coff-ld /usr/local/bin/pic30-coff-ld

rm -f /usr/local/bin/pic30-elf-ld
ln -s $prefix/bin/pic30-elf-ld /usr/local/bin/pic30-elf-ld

# ar
rm -f /usr/local/bin/pic30-ar
ln -s $prefix/bin/pic30-coff-ar /usr/local/bin/pic30-ar
rm -f /usr/local/bin/pic30-coff-ar
ln -s $prefix/bin/pic30-coff-ar /usr/local/bin/pic30-coff-ar

rm -f /usr/local/bin/pic30-elf-ar
ln -s $prefix/bin/pic30-elf-ar /usr/local/bin/pic30-elf-ar

# as
rm -f /usr/local/bin/pic30-as
ln -s $prefix/bin/pic30-coff-as /usr/local/bin/pic30-as
rm -f /usr/local/bin/pic30-coff-as
ln -s $prefix/bin/pic30-coff-as /usr/local/bin/pic30-coff-as

rm -f /usr/local/bin/pic30-elf-as
ln -s $prefix/bin/pic30-elf-as /usr/local/bin/pic30-elf-as

# nm
rm -f /usr/local/bin/pic30-nm
ln -s $prefix/bin/pic30-coff-nm /usr/local/bin/pic30-nm
rm -f /usr/local/bin/pic30-coff-nm
ln -s $prefix/bin/pic30-coff-nm /usr/local/bin/pic30-coff-nm

rm -f /usr/local/bin/pic30-elf-nm
ln -s $prefix/bin/pic30-elf-nm /usr/local/bin/pic30-elf-nm

# objdump
rm -f /usr/local/bin/pic30-objdump
ln -s $prefix/bin/pic30-coff-objdump /usr/local/bin/pic30-objdump
rm -f /usr/local/bin/pic30-coff-objdump
ln -s $prefix/bin/pic30-coff-objdump /usr/local/bin/pic30-coff-objdump

rm -f /usr/local/bin/pic30-elf-objdump
ln -s $prefix/bin/pic30-elf-objdump /usr/local/bin/pic30-elf-objdump

# objcopy
rm -f /usr/local/bin/pic30-objcopy
ln -s $prefix/bin/pic30-coff-objcopy /usr/local/bin/pic30-objcopy
rm -f /usr/local/bin/pic30-coff-objcopy
ln -s $prefix/bin/pic30-coff-objcopy /usr/local/bin/pic30-coff-objcopy

rm -f /usr/local/bin/pic30-elf-objcopy
ln -s $prefix/bin/pic30-elf-objcopy /usr/local/bin/pic30-elf-objcopy

# ranlib
rm -f /usr/local/bin/pic30-ranlib
ln -s $prefix/bin/pic30-coff-ranlib /usr/local/bin/pic30-ranlib
rm -f /usr/local/bin/pic30-coff-ranlib
ln -s $prefix/bin/pic30-coff-ranlib /usr/local/bin/pic30-coff-ranlib

rm -f /usr/local/bin/pic30-elf-ranlib
ln -s $prefix/bin/pic30-elf-ranlib /usr/local/bin/pic30-elf-ranlib

# strip
rm -f /usr/local/bin/pic30-strip
ln -s $prefix/bin/pic30-coff-strip /usr/local/bin/pic30-strip
rm -f /usr/local/bin/pic30-coff-strip
ln -s $prefix/bin/pic30-coff-strip /usr/local/bin/pic30-coff-strip

rm -f /usr/local/bin/pic30-elf-strip
ln -s $prefix/bin/pic30-elf-strip /usr/local/bin/pic30-elf-strip

# strings
rm -f /usr/local/bin/pic30-strings
ln -s $prefix/bin/pic30-coff-strings /usr/local/bin/pic30-strings
rm -f /usr/local/bin/pic30-coff-strings
ln -s $prefix/bin/pic30-coff-strings /usr/local/bin/pic30-coff-strings

rm -f /usr/local/bin/pic30-elf-strings
ln -s $prefix/bin/pic30-elf-strings /usr/local/bin/pic30-elf-strings

# size
rm -f /usr/local/bin/pic30-size
ln -s $prefix/bin/pic30-coff-size /usr/local/bin/pic30-size
rm -f /usr/local/bin/pic30-coff-size
ln -s $prefix/bin/pic30-coff-size /usr/local/bin/pic30-coff-size

rm -f /usr/local/bin/pic30-elf-size
ln -s $prefix/bin/pic30-elf-size /usr/local/bin/pic30-elf-size

# addr2line
rm -f /usr/local/bin/pic30-addr2line
ln -s $prefix/bin/pic30-coff-addr2line /usr/local/bin/pic30-addr2line
rm -f /usr/local/bin/pic30-coff-addr2line
ln -s $prefix/bin/pic30-coff-addr2line /usr/local/bin/pic30-coff-addr2line

rm -f /usr/local/bin/pic30-elf-addr2line
ln -s $prefix/bin/pic30-elf-addr2line /usr/local/bin/pic30-elf-addr2line

# readelf
rm -f /usr/local/bin/pic30-readelf
ln -s $prefix/bin/pic30-coff-readelf /usr/local/bin/pic30-readelf
rm -f /usr/local/bin/pic30-coff-readelf
ln -s $prefix/bin/pic30-coff-readelf /usr/local/bin/pic30-coff-readelf

rm -f /usr/local/bin/pic30-elf-readelf
ln -s $prefix/bin/pic30-elf-readelf /usr/local/bin/pic30-elf-readelf

# bin2hex
rm -f /usr/local/bin/pic30-bin2hex
ln -s $prefix/bin/pic30-coff-bin2hex /usr/local/bin/pic30-bin2hex
rm -f /usr/local/bin/pic30-coff-bin2hex
ln -s $prefix/bin/pic30-coff-bin2hex /usr/local/bin/pic30-coff-bin2hex

rm -f /usr/local/bin/pic30-elf-bin2hex
ln -s $prefix/bin/pic30-elf-bin2hex /usr/local/bin/pic30-elf-bin2hex

