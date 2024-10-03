
build_main: build_zenith ./main.c 
	gcc -Wall -Wextra -c main.c -o bin/main.o
	gcc -Wall -Wextra ./bin/* -o main

build_zenith: build_zth_static build_zenith_fn build_zenith_main build_zth_dir build_zth_file build_zth_dev_in

build_zenith_fn: includes/zth_fn.c includes/zth_fn.h
	gcc -c includes/zth_fn.c -o bin/zth_fn.o

build_zenith_main: includes/zenith.c includes/zenith.h 
	gcc -c includes/zenith.c -o bin/zenith.o

build_zth_dir: includes/zth_dir.c includes/zth_dir.h
	gcc -c includes/zth_dir.c -o bin/zth_dir.o 

build_zth_file: includes/zth_file.c includes/zth_file.h 
	gcc -c includes/zth_file.c -o bin/zth_file.o
	
build_zth_dev_in: includes/zth_dev_in.c includes/zth_dev_in.h
	gcc -c includes/zth_dev_in.c -o bin/zth_dev_in.o

build_zth_static: includes/zth_static.c includes/zth_static.h
	gcc -c includes/zth_static.c -o bin/zth_static.o
