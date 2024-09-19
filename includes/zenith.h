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

//#define VIRTUAL_DRIVE

#define DIRECT_DRIVE_SUPPORT


//#define DIRECT_DRIVE_SUPPORT

/* ================================================= */


/* fs tab precompiler costant */

#define SIZE 16777216

/* size precompiler costant that shoud be defined to specify the partition size */

#ifndef SIZE

#define SIZE 65536 /* default partition size ( 64kb ) */

#endif

#define MAX_FILESYSTEM_SIZE 16777216 /* max size adressable by the filesystem ( in byte ) */


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
    
    #ifndef VIRTUAL_DRIVE
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
    #endif

    #ifdef VIRTUAL_DRIVE
        /* DevMode: the actual content of the file */
        struct zenith_gn* content[CONTENT_SIZE];
        /* DevMode: address of the extension address */
        struct zenith_gn* extended_adr;
        /* DevMode: current node adr */
        struct zenith_gn* current_address;
    #endif

}zenith_general_node;


typedef struct{
    /* name for the partition */
    char name[NAME_LENGTH];
    /* partition size, max 16mb */
    uint8_t partition_size; 
    /* allocated node variable */
    uint8_t free_page;
    bool allocated_page[NODE_COUNT];
    #ifndef VIRTUAL_DRIVE

    /* address for each node */
    uint8_t page_address[NODE_COUNT*3];

    /* first_node_address */

    uint8_t first_node_lb;
    uint8_t first_node_hb;
    uint8_t first_node_xlb;

    #endif

    #ifdef VIRTUAL_DRIVE

    /* address for each node */
    zenith_general_node* page_address[NODE_COUNT];

    /* first_node_address */

    zenith_general_node* first_node;

    #endif

}zenith_fstab;

/* ================================================= */

/* size of the standard filesystem node and size of the zenith filesystem table */

#define ZENITH_NODE_SIZE (sizeof(zenith_general_node))
#define ZENITH_FSTAB_SIZE (sizeof(zenith_fstab))


#define LOCAL_SAVING_PATH "/out/fstab.bin"
#define DATA_FROM_FLAG_OFFSET (NODE_COUNT | CHAR_SIZE | UINT8_T_SIZE*2);

/* static variable and end-point */

#ifdef VIRTUAL_DRIVE


    static uint8_t* virtual_drive = NULL;

    /* static address variable in ram */

    static void* cache_adr = NULL;


    zenith_fstab fstab_global;

#endif


#ifndef VIRTUAL_DRIVE

/* static address variable in ram */

static uint8_t cache_adr_lb;
static uint8_t cache_adr_hb;
static uint8_t cache_adr_xlb;

#endif

/* zenith_general_node variable in RAM */


/* with the user-layer this variable would be allocated if it's not  */


static zenith_general_node* cache_node = NULL;



/*

    This is the initialization function, call it if you want to 
    initialize a new partition.

*/
extern void zenith_initFs(int size, char* part_name, uint8_t drive_type);

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



#ifndef ZENITH_IMPLEMENTATION
#define ZENITH_IMPLEMENTATION
#endif


#endif
