#!bin/bash


echo "compiling... "
gcc -c include/fs.c -o bin/fs.o
gcc main.c bin/fs.o -o testFs.out
