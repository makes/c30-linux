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

if [ ! -f build_done ];
then
    echo "error: 'make' must be run first."
    echo ""
    echo "Usage: ./configure [installdir]"
    echo "       make"
    echo "       sudo make install"
    echo ""
    exit 1
fi

prefix=`cat install_prefix`

# BINUTILS
# --------

cd pic30-binutils/coff-build/
make install
cd ../elf-build/
make install

# GCC
# ---

cd ../../pic30-gcc/coff-build/
make install
cd ../elf-build/
make install

cd ../..

# Create softlinks
if [ ! -f $prefix/libexec/gcc/pic30-coff/4.0.3/pic30-coff-as ];
then
    ln -s $prefix/bin/pic30-coff-as $prefix/libexec/gcc/pic30-coff/4.0.3/
fi

if [ ! -f $prefix/libexec/gcc/pic30-coff/4.0.3/pic30-coff-ld ];
then
    ln -s $prefix/bin/pic30-coff-ld $prefix/libexec/gcc/pic30-coff/4.0.3/
fi

if [ ! -f $prefix/libexec/gcc/pic30-elf/4.0.3/pic30-elf-as ];
then
    ln -s $prefix/bin/pic30-elf-as $prefix/libexec/gcc/pic30-elf/4.0.3/
fi

if [ ! -f $prefix/libexec/gcc/pic30-elf/4.0.3/pic30-elf-ld ];
then
    ln -s $prefix/bin/pic30-elf-ld $prefix/libexec/gcc/pic30-elf/4.0.3/
fi

