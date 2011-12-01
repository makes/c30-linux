#!/bin/bash

for p in $( ls patches/pic30-binutils ); do
    echo "applying patch: pic30-binutils/$p"
    patch -p0 -N -b -r - < patches/pic30-binutils/$p
done

for p in $( ls patches/pic30-gcc ); do
    echo "applying patch: pic30-gcc/$p"
    patch -p0 -N -b -r - < patches/pic30-gcc/$p
done

