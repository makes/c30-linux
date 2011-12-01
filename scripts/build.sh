#!/bin/bash

if [ ! -f install_prefix ];
then
    echo "error: ./configure must be run first."
    echo ""
    echo "Usage: ./configure [installdir]"
    echo "       make"
    echo "       sudo make install"
    echo ""
    exit 1
fi

# BINUTILS
# --------

# Build COFF binutils:
cd pic30-binutils/coff-build/
find .. -name *.y -type f -exec touch '{}' ';'
find .. -name *.l -type f -exec touch '{}' ';'
make

# Build ELF binutils:
cd ../elf-build/
find .. -name *.y -type f -exec touch '{}' ';'
find .. -name *.l -type f -exec touch '{}' ';'
make

# GCC
# ---

# Build COFF gcc:
cd ../../pic30-gcc/coff-build/
find .. -name *.y -type f -exec touch '{}' ';'
find .. -name *.l -type f -exec touch '{}' ';'
make

# Build ELF gcc:
cd ../elf-build/
find .. -name *.y -type f -exec touch '{}' ';'
find .. -name *.l -type f -exec touch '{}' ';'
make

cd ../..

echo "1" > build_done

