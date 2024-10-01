#ifndef ZTH_FN_H
#define ZTH_FN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

/* zenith library */

#include "./zenith.h"
#include "./zth_dir.h"
#include "./zth_file.h"
#include "./zth_dev_in.h"


/* TODO: create functions to link directly usable functions from the library files */

/* TODO: create a start function to initialize the static zenith variable  */

#define VERSION 1

extern void _initialize();

#ifdef VIRTUAL_DRIVE

typedef void (*zenith_initFs_ptr)(int size, char*path); 
typedef int (*zenith_loadFs_ptr)(int size); 

#endif

typedef int (*zenith_mv_ptr)(char* start_path, char*name, char*dest_path, char*dest_name);

typedef int (*zenith_mkdir_ptr)(char*path, char*name);
typedef int (*zenith_rmdir_ptr)(char*path, char*name); 



typedef struct {
  int version;
  int size;
  #ifdef VIRTUAL_DRIVE
    zenith_initFs_ptr zenith_initFs;
    zenith_loadFs_ptr zenith_loadFs;
  #endif
  zenith_mkdir_ptr zenith_mkdir;
  zenith_rmdir_ptr zenith_rmdir;
  zenith_mv_ptr zenith_mv;
  
}zth_int;

static zth_int Zth = {
  .version = VERSION,
  .size = SIZE,
  #ifdef VIRTUAL_DRIVE
  .zenith_initFs = &zenith_initFs,
  .zenith_loadFs = &zenith_loadFs,
  #endif
  .zenith_mkdir = &zenith_mkdir,
  .zenith_rmdir = &zenith_rmdir,
  .zenith_mv = &zenith_mv
};

#ifndef ZTH_FN_IMPLEMENTATION
#define ZTH_FN_IMPLEMENTATION

#endif


#endif
