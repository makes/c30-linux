#!/bin/bash

if [ -z "$1" ]
then
    echo "error: prefix must be specified as command line argument."
    exit 1
else
    prefix=$1
fi

./scripts/apply-patches.sh

# BINUTILS
# --------

cd pic30-binutils/

# Build COFF binutils:
rm -fr coff-build && mkdir coff-build && cd coff-build
find .. -name *.y -type f -exec touch '{}' ';'
find .. -name *.l -type f -exec touch '{}' ';'
CFLAGS='-DMCHP_VERSION="v3_30" -DC30_SMARTIO_RULES=3' ../acme/configure --prefix=$prefix --program-prefix=pic30-coff- --target=pic30-coff
cd ..

# Build ELF binutils:
rm -fr elf-build && mkdir elf-build && cd elf-build
find .. -name *.y -type f -exec touch '{}' ';'
find .. -name *.l -type f -exec touch '{}' ';'
CFLAGS='-DMCHP_VERSION="v3_30" -DC30_SMARTIO_RULES=3' ../acme/configure --prefix=$prefix --program-prefix=pic30-elf- --target=pic30-elf
cd ../..

# GCC
# ---

# Build COFF gcc:
cd pic30-gcc/
rm -fr coff-build && mkdir coff-build && cd coff-build
find .. -name *.y -type f -exec touch '{}' ';'
find .. -name *.l -type f -exec touch '{}' ';'
CFLAGS='-DMCHP_VERSION="v3_30" -DC30_SMARTIO_RULES=3' ../gcc-4.0.2/gcc-4.0.2/configure --prefix=$prefix --target=pic30-coff --program-prefix=pic30-coff- --enable-languages=c --with-included-gettext

# Build ELF gcc:
cd .. && rm -fr elf-build && mkdir elf-build && cd elf-build
find .. -name *.y -type f -exec touch '{}' ';'
find .. -name *.l -type f -exec touch '{}' ';'
CFLAGS='-DMCHP_VERSION="v3_30" -DC30_SMARTIO_RULES=3' ../gcc-4.0.2/gcc-4.0.2/configure --prefix=$prefix --target=pic30-elf --program-prefix=pic30-elf- --enable-languages=c --with-included-gettext

cd ../..

