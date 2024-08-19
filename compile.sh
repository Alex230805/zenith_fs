#!bin/bash


echo "compiling... "
gcc -c include/fs_table.c -o bin/fs_table.o
gcc -c include/fs_file.c -o bin/fs_file.o
gcc -c include/fs_dir.c -o bin/fs_dir.o
gcc -c include/fs.c -o bin/fs.o
gcc main.c bin/fs.o -o testFs.out
