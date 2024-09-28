
all: build_main
	./main

build_main: build_zenith ./main.c 
	gcc -c main.c -o bin/main.o
	gcc ./bin/* -o main


build_zenith: build_zenith_fn build_zenith_main build_zth_dir build_zth_file build_zth_dev_in

build_zenith_fn: includes/zth_fn.c includes/zth_fn.h
	echo "Compiling zenith function layer"
	gcc -c includes/zth_fn.c -o bin/zth_fn.o

build_zenith_main: includes/zenith.c includes/zenith.h 
	echo "Compiling zenith main header"
	gcc -c includes/zenith.c -o bin/zenith.o

build_zth_dir: includes/zth_dir.c includes/zth_dir.h
	echo "Compiling zenith dir functions"
	gcc -c includes/zth_dir.c -o bin/zth_dir.o

build_zth_file: includes/zth_file.c includes/zth_file.h 
	echo "Compiling zenith file functions"
	gcc -c includes/zth_file.c -o bin/zth_file.o
	
build_zth_dev_in: includes/zth_dev_in.c includes/zth_dev_in.h
	echo "Compiling zenith device interface"
	gcc -c includes/zth_dev_in.c -o bin/zth_dev_in.o	

