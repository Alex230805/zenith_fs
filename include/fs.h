#ifndef FS_H
#define FS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_DIM_BYTE 1048576

#define BIG_SIZE MAX_DIM_BYTE
#define MEDIUM_SIZE (MAX_DIM_BYTE/2)
#define SMALL_SIZE (MEDIUM_SIZE/2)
#define MAX_PAGE_COUNT 2048
#define PAGE_LENGHT 512

#define DATA_MASK 0x01
#define DIR_MASK 0x10


typedef struct{
    uint32_t *adr;
    char name[16];
    uint8_t type;
    uint32_t * content[296];
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


}fs_tab;

#define ROOT_SIZE sizeof(fs_tab);


node get_from_device(uint32_t *address);
bool write_into_device(node n);
fs_tab get_fs_tab();
bool write_fs_tab(fs_tab root);
uint32_t* fs_alloc(fs_tab root, int type);
bool fs_free(fs_tab root, uint32_t *adr);
fs_tab init_fs(char*name, char*version, int size);


uint32_t * virtual_path;

#ifdef FS_IMPLEMENTATION
#define FS_C
#endif



#endif
