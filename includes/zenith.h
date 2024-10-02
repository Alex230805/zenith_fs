#ifndef ZENITH_H
#define ZENITH_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "./zth_dev_in.h"
#include "./zth_static.h"
 
/* ================================================= */

/* permission and node types */

#define W_PERM  ( 0b00000011 )
#define R_PERM  ( 0b00001100 )
#define E_PERM  ( 0b00110000 )

#define TYPES_MASK 0xf0
#define FILE_TYPE ( TYPES_MASK | 0x0d )
#define DIR_TYPE ( TYPES_MASK | 0x0e)

#define STACK_PROTECTION_LIMIT 64

/* size of the standard filesystem node and size of the zenith filesystem table */

#define ZENITH_NODE_SIZE (sizeof(zenith_general_node))
#define ZENITH_FSTAB_SIZE (sizeof(zenith_fstab))

#define ZENITH_FSTAB_NODE_SPACE (UINT8_T_SIZE*4 + CHAR_SIZE*NAME_LENGHT)

#define LOCAL_SAVING_PATH "out/fstab.bin"

#define DATA_FROM_FLAG_OFFSET ((sizeof(bool) & NODE_COUNT) | (CHAR_SIZE & NAME_LENGTH) | (UINT8_T_SIZE*2))
#define DATA_FROM_ROOT_OFFSET (DATA_FROM_FLAG_OFFSET | (UINT8_T_SIZE & (NODE_COUNT*3)))



#define __STACK_PROTECTION_SYSTEM() if(stack_id > STACK_PROTECTION_LIMIT){\
    printf("Fatal: stack protection system enabled, hard return from recursive routine\n");\
    return STACK_PROTECTION_ERROR;\
  }

#define __NAME_CHECK(path, name) if(path == NULL && name == NULL){\
    printf("Error: no such name provided\n");\
    return NO_NAME_PROVIDED_ERROR;\
}

#define __ADDRESS_CHECK(adr_lb, adr_hb,adr_xlb) if(adr_lb == 0x00 && adr_hb == 0x00 && adr_xlb == 0x00){\
    printf("Error: failed to reach the destination provided\n");\
    return ROUTE_NOT_FOUND_ERROR;\
}

#define __EXTENSION_ADDRESS_CHECK(adr_lb,adr_hb,adr_xlb) if(adr_lb == 0x00 && adr_hb == 0x00 && adr_xlb == 0x00){\
  printf("Fatal: null pointer detected in extension folder address space\n");\
  return NO_ADDRESS_FOUND;\
}

#define __FATAL_ERROR() printf("Fatal: something went wrong during the operation, please retry \n");\
  return WRITING_ERROR;

#define __FEATURE_INTERRUPT() printf("Feature currently under development n");\
  return;

#define __FEATURE_INTERRUPT_RET(ret_message) printf("Feature currently under development\n");\
  return ret_message;



typedef enum{
  MEMORY_ERROR = 1,
  WRITING_ERROR,
  READING_ERROR,
  ROUTE_NOT_FOUND_ERROR,
  NO_NAME_PROVIDED_ERROR,
  NO_ADDRESS_FOUND,
  STACK_PROTECTION_ERROR,
  NOT_IMPLEMENTED_YET,
  FILE_NOT_FOUND,
  ABORTING_OPERATION
}zentih_status;

typedef enum{
  DEFAULT_DRIVER = 0x00,
  DRIVER_1,
  DRIVER_2,
  DRIVER_3,
  DRIVER_4,
  DRIVER_5,
  DRIVER_6,
  DRIVER_7,
  DRIVER_8,
  DRIVER_9,
  DRIVER_10,
}zenith_drive;

#ifdef VIRTUAL_DRIVE

uint8_t* virtual_drive;

#endif

uint8_t cache_adr_lb;
uint8_t cache_adr_hb;
uint8_t cache_adr_xlb;

uint8_t cache_adr_lb_2;
uint8_t cache_adr_hb_2;
uint8_t cache_adr_xlb_2;

zenith_general_node* cache_node;
zenith_general_node* cache_node_2;

zenith_general_node* zenith_root_node;

uint8_t zenith_selected_driver;


/*

    This is the initialization function, call it if you want to 
    initialize a new partition.

*/

#ifndef ZENITH_EXCLUDE_INIT


extern void zenith_initFs(int size, char* part_name);


extern int zenith_loadFs(int size);

#endif
/*

    Function to alloc or dealloc new node, this funtion  suppose 
    to be used by any software directly.
    Those function will be used by other zenith_fs functions with an higher 
    lever of abstraction.

*/


/* 

    malloc like function, write the fs_tab and use the static address variable 
    in RAM to store the allocated address.
    The other zenith function will read the saved address in ram and proceed to
    operate.

*/


extern void zenith_malloc(int type, char*name);

/* 

    free like function, it use the cache address in RAM for reference and clear the fstab.
    IT DOESN'T CLEAR BYTE-BY-BYTE the memory location. IT'S NOT A LOW LEVEL ERASING.

*/

extern void zenith_free();


/* 
    Set target for Zenith I/O layer

 */


extern void zenith_set_target(uint8_t target);


/* search in the node tree and find out if somethings called "name" is present */

extern bool zenith_is_present(uint8_t adr_lb, uint8_t adr_hb, uint8_t adr_xlb, char* name);

/* get root node, aka the first node of the disk, and save it in a copy in RAM ( it will be used to navigate the root tree ) */

extern void zenith_get_root();

/*
  navigate in a specific path and it return the static node copy in RAM.
  For reference if you need to navigate into a specific directory, it start to
  navigate from the root_point and use a compination of zenith_is_present, zenith_pop
  and the single name if each path to find out the tree and finally return in the cache
  node the copy of the desire folder, ready to be process by any other command


 */

extern void zenith_navigate(char*path);


#ifndef ZENITH_IMPLEMENTATION
#define ZENITH_IMPLEMENTATION
#endif


#endif
