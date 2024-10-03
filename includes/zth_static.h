#ifndef STATIC_H
#define STATIC_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* types of support and drive */

#define VIRTUAL_DRIVE

//#define DIRECT_DRIVE_SUPPORT

//#define ZENITH_EXCLUDE_INIT


/* ================================================= */


/* fs tab precompiler costant */

#define SIZE 16777215 /* define 16 megabyte partition */


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



#ifdef VIRTUAL_DRIVE

extern uint8_t* virtual_drive;

#endif


/* static address variable in ram, use only as a return endpoint, so when a function, like malloc and free, need to return an entire address */
extern uint8_t cache_adr_lb;
extern uint8_t cache_adr_hb;
extern uint8_t cache_adr_xlb;

extern uint8_t cache_adr_lb_2;
extern uint8_t cache_adr_hb_2;
extern uint8_t cache_adr_xlb_2;

/* zenith_general_node variable in RAM, use as a return endpoint */

extern zenith_general_node* cache_node;
extern zenith_general_node* cache_node_2;

/* static copy of the first node in RAM, use for navigation */

extern zenith_general_node* zenith_root_node;
/* driver types */


/* selected driver variable is used to change in all the zenith function the way to do call in the interface layer */

extern uint8_t zenith_selected_driver;

/* define static variable */



#ifdef STATIC_IMPLEMENTATION
#define STATIC_IMPLEMENTATION


#endif


#endif
