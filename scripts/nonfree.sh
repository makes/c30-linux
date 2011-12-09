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

prefix=`cat install_prefix`

if [ $(ls -1A ./pic30-nonfree | wc -l) -eq 0 ];
then
    echo "No non-free components found in 'pic30-nonfree'"
    echo ""
    echo "If you wish to install the non-free components, copy the following"
    echo "files from a C30 Windows installation to 'pic30-nonfree':"
    echo ""
    echo "bin/c30_device.info"
    echo "lib"
    echo "support"
    echo "bin/device_files"
    echo "include"
    echo ""
    echo "and run 'make nonfree'."
    echo ""
    exit 0
fi

if [ ! -f pic30-nonfree/bin/c30_device.info ];
then
    echo "error: pic30-nonfree/bin/c30_device.info was not found."
    exit 1
fi

if [ ! -d pic30-nonfree/bin/device_files ];
then
    echo "error: pic30-nonfree/bin/device_files was not found."
    exit 1
fi

if [ ! -d pic30-nonfree/lib ];
then
    echo "warning: pic30-nonfree/lib was not found."
fi

if [ ! -d pic30-nonfree/include ];
then
    echo "warning: pic30-nonfree/include was not found."
fi

if [ ! -d pic30-nonfree/support ];
then
    echo "warning: pic30-nonfree/support was not found."
fi

rm -fdr $prefix/pic30-nonfree
cp -Rv ./pic30-nonfree $prefix/pic30-nonfree

rm -f $prefix/c30_device.info
ln -s $prefix/pic30-nonfree/bin/c30_device.info $prefix/
rm -fdr $prefix/libexec/gcc/pic30-coff/4.0.3/device_files
ln -s $prefix/pic30-nonfree/bin/device_files $prefix/libexec/gcc/pic30-coff/4.0.3/
rm -f $prefix/libexec/gcc/pic30-coff/4.0.3/c30_device.info
ln -s $prefix/pic30-nonfree/bin/c30_device.info $prefix/libexec/gcc/pic30-coff/4.0.3/

# Try to install procedural abstractor.
if [ ! -f pic30-nonfree/bin/pic30-coff-pa ];
then
    echo "warning: pic30-nonfree/bin/pic30-coff-pa was not found."
else
    rm -f $prefix/bin/pic30-coff-pa
    ln -s $prefix/pic30-nonfree/bin/pic30-coff-pa $prefix/bin/
fi

if [ ! -f pic30-nonfree/bin/pic30-elf-pa ];
then
    echo "warning: pic30-nonfree/bin/pic30-elf-pa was not found."
else
    rm -f $prefix/bin/pic30-elf-pa
    ln -s $prefix/pic30-nonfree/bin/pic30-elf-pa $prefix/bin/
fi
