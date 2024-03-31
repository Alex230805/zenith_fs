#!bin/bash


echo "compiling... "
clang -c -g include/fs.c -o build/Debug/fs.o
clang -c -g include/fs.c -o bin/fs.o
clang -c -g main.c -o build/Debug/main.o
clang -c -g main.c -o bin/main.o
clang -o build/Debug/outDebug build/Debug/*
clang -o a.out bin/*
