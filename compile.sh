#!bin/bash


echo "compiling... "
gcc -c includes/zenith.c -o bin/zenith.o
gcc -c includes/zth_dir.c -o bin/zth_dir.o
gcc -c includes/zth_file.c -o bin/zth_file.o
gcc -c main.c -o bin/main.o
gcc ./bin/* -o a.out

