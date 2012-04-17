#!/bin/bash

rm -f install_prefix
rm -f build_done
cd pic30-binutils/
rm -fr coff-build
rm -fr elf-build
cd ../pic30-gcc/
rm -fr coff-build
rm -fr elf-build
cd ..

