#ifndef ZENITH_H
#define ZENITH_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "./zth_dev_in.h"
 
/* types of support and drive */

#define VIRTUAL_DRIVE

//#define DIRECT_DRIVE_SUPPORT

//#define ZENITH_EXCLUDE_INIT


/* ================================================= */


/* fs tab precompiler costant */

#define SIZE 16777215

/* size precompiler costant that shoud be defined to specify the partition size */

#ifndef SIZE

#define SIZE 65535 /* default partition size ( 64kb ) */

#endif

#define MAX_FILESYSTEM_SIZE 16777215 /* max size adressable by the filesystem ( in byte ) */


#define NAME_LENGTH 8
#define NODE_COUNT ( SIZE / 255 )    /* node number */
#define PERM_MASK_FILE 0xc0
#define PERM_MASK_DIR 0x80
#define BOOL_SIZE (sizeof(bool))
#define UINT8_T_SIZE (sizeof(uint8_t))
#define CHAR_SIZE (sizeof(char))

#define CONTENT_SIZE (0xff - BOOL_SIZE - UINT8_T_SIZE*8 - CHAR_SIZE*NAME_LENGTH)


/* ================================================= */

/* permission and node types */

#define W_PERM  ( 0b00000011 )
#define R_PERM  ( 0b00001100 )
#define E_PERM  ( 0b00110000 )

#define TYPES_MASK 0xf0
#define FILE_TYPE ( TYPES_MASK | 0x0d )
#define DIR_TYPE ( TYPES_MASK | 0x0e)

#define STACK_PROTECTION_LIMIT 64

/* ================================================= */

/* node and fs tab structure */


typedef struct zenith_gn{
    /* file name, max 8 chars */
    char name[NAME_LENGTH];
    /* type of node, if it's a folder or a directory */
    uint8_t type;
    /* file permission, in many cases would be usefull */
    uint8_t perm;
    /* variable to check if the file is extended to another */
    bool extended;
    /* the actual content of the file */
    uint8_t content[CONTENT_SIZE];
    /* address of the extension address */
    uint8_t extended_adr_lb;
    uint8_t extended_adr_hb;
    uint8_t extended_adr_xlb;
    /* current node adr */
    uint8_t adr_lb;
    uint8_t adr_hb;
    uint8_t adr_xlb;

}zenith_general_node;


typedef struct{
    /* name for the partition */
    char name[NAME_LENGTH];
    /* partition size, max 16mb */
    uint8_t partition_size; 
    /* allocated node variable */
    uint8_t free_page;
    bool allocated_page[NODE_COUNT];
    /* address for each node */
    uint8_t page_address[NODE_COUNT*3];
    /* first_node_address */
    uint8_t first_node_lb;
    uint8_t first_node_hb;
    uint8_t first_node_xlb;

}zenith_fstab;

/* ================================================= */

/* size of the standard filesystem node and size of the zenith filesystem table */

#define ZENITH_NODE_SIZE (sizeof(zenith_general_node))
#define ZENITH_FSTAB_SIZE (sizeof(zenith_fstab))

#define ZENITH_FSTAB_NODE_SPACE (UINT8_T_SIZE*4 + CHAR_SIZE*NAME_LENGHT)

#define LOCAL_SAVING_PATH "/out/fstab.bin"

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

/* static variable and end-point */

#ifdef VIRTUAL_DRIVE

static uint8_t* virtual_drive = NULL;

#endif


/* static address variable in ram, use only as a return endpoint, so when a function, like malloc and free, need to return an entire address */
static uint8_t cache_adr_lb;
static uint8_t cache_adr_hb;
static uint8_t cache_adr_xlb;

static uint8_t cache_adr_lb_2;
static uint8_t cache_adr_hb_2;
static uint8_t cache_adr_xlb_2;

/* zenith_general_node variable in RAM, use as a return endpoint */

static zenith_general_node* cache_node = NULL;
static zenith_general_node* cache_node_2 = NULL;

/* static copy of the first node in RAM, use for navigation */

static zenith_general_node* zenith_root_node = NULL;

/* driver types */


/* selected driver variable is used to change in all the zenith function the way to do call in the interface layer */

static uint8_t zenith_selected_driver = DEFAULT_DRIVER;

/*

    This is the initialization function, call it if you want to 
    initialize a new partition.

*/

#ifndef ZENITH_EXCLUDE_INIT


extern void zenith_initFs(int size, char* part_name);


extern int zenith_loadFs(int size);

#endif
/*

    Function to alloc or dealloc new node, this funtion are not suppose 
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
