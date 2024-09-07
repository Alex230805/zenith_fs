#ifndef ZENITH_H
#define ZENITH_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>


/* types of support and drive */

//  #define VIRTUAL_DRIVE
//  #define DIRECT_DRIVE_SUPPORT


#define DIRECT_DRIVE_SUPPORT

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

#define W_PERM  (0b00000011 | PERM_MASK)
#define R_PERM  (0b00001100 | PERM_MASK)
#define E_PERM  (0b00110000 | PERM_MASK)

#define TYPES_MASK 0xf
#define FILE_TYPE ( TYPES_MASK | 0x0d )
#define DIR_TYPE ( TYPES_MASK | 0x0e)

/* ================================================= */

/* node and fs tab structure */


typedef struct{
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

    #ifdef VIRUTAL_DRIVE
        /* DevMode: the actual content of the file */
        void* content[CONTENT_SIZE];
        /* DevMode: address of the extension address */
        void* extended;
        /* DevMode: current node adr */
        void* current_address;
    #endif

}zenith_general_node;


typedef struct{
    /* name for the partition */
    char name[NAME_LENGTH];
    /* partition size, max 16mb */
    int partition_size; 
    /* allocated node variable */
    int free_space;
    int free_page;
    bool allocated_page[NODE_COUNT];
    /* address for each node */
    #ifndef VIRTUAL_DRIVE

    uint8_t page_address[NODE_COUNT*3];

    #endif

    #ifdef VIRTUAL_DRIVE

    void* page_address[NODE_COUNT];

    #endif

}zenith_fstab;

/* ================================================= */

/* size of the standard filesystem node and size of the zenith filesystem table */

#define ZENITH_NODE_SIZE (sizeof(zenith_general_node))
#define ZENITH_FSTAB_SIZE (sizeof(zenith_fstab))


/* static variable and end-point */

static uint8_t* virtual_drive = NULL;

#ifndef ZENITH_IMPLEMENTATION
#define ZENITH_IMPLEMENTATION

#endif


#endif