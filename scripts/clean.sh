#!/bin/bash

rm -fd install_prefix
rm -fd build_done
cd pic30-binutils/
rm -fdr coff-build
rm -fdr elf-build
cd ../pic30-gcc/
rm -fdr coff-build
rm -fdr elf-build
cd ..

