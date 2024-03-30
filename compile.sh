#!bin/bash


echo "compiling... "
gcc -c -g include/fs.c -o bin/fs.o
gcc -c -g main.c -o bin/main.o
gcc -o a.out bin/*
