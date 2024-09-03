#ifndef FS_TABLE_H
#define FS_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

/* device selection 
*   Defien device target for compilation, define only one target
*
*   #define __VIRTUAL_DISK__
*   #define __x86_64b_OS__
*   #define __ARDUINO__
*
*/

#define __VIRTUAL_DISK__

#ifdef __ARDUINO__

#inlcude <Arduino.h>

#endif

#define MAX_DIM_BYTE 1048576

#define BIG_SIZE MAX_DIM_BYTE
#define MEDIUM_SIZE (MAX_DIM_BYTE/2)
#define SMALL_SIZE (MEDIUM_SIZE/2)
#define MAX_PAGE_COUNT 2048
#define PAGE_LENGHT 512

#define DATA_MASK 0x01
#define DIR_MASK 0x10

#define ROOT_SIZE sizeof(fs_tab);

typedef struct{
    uint32_t *adr;
    char name[16];
    uint8_t type;
    uint32_t * content[59];
    uint32_t * next;
}node;


typedef struct{
    char name[16];
    char version[16];

    uint32_t total_space;
    uint32_t used_space;
    uint32_t free_space;

    uint32_t fs_table_byte_space;

    uint32_t* page_pointer[MAX_PAGE_COUNT];
    uint32_t allocation_table[MAX_PAGE_COUNT];
    
    node * first_node;
}fs_tab;


extern fs_tab init_fs(char*name, char*version, int size);
extern fs_tab get_fs_tab();
extern void fs_get_info(fs_tab*root);
extern node get_from_device(uint32_t*address);
extern bool write_fs_tab(fs_tab *root);
extern bool write_into_device(node n);
extern uint32_t* fs_alloc(fs_tab *root, int type, char*name);
extern bool fs_free(fs_tab* root, uint32_t *adr);
extern bool update_fs_tab(fs_tab*root);
extern bool fs_is_alloc(uint32_t*address);

static uint32_t * virtual_path;

#ifndef FS_TABLE_IMPLEMENTATION
#define FS_TABLE_IMPLEMENTATION

#endif

#endif