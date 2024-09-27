
all: build_main
	./main

build_main: build_zenith
	gcc -c main.c -o bin/main.o
	gcc ./bin/* -o main

build_zenith:
	gcc -c includes/zenith.c -o bin/zenith.o
	gcc -c includes/zth_dev_in.c -o bin/zth_dev_in.o
	gcc -c includes/zth_dir.c -o bin/zth_dir.o
	gcc -c includes/zth_file.c -o bin/zth_file.o
	gcc -c includes/zth_fn.c -o bin/zth_fn.o
