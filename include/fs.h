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


#define __VIRTUAL_DISK__ 0x01
#define __x86_64b_OS__ 0x02
#define __ARDUINO__ 0x03

enum device_type{VIRTUAL_DISK,x86_64b_OS, ARDUINO};

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

#define ROOT_SIZE sizeof(fs_tab);

void fs_set_device(int type);
node get_from_device(uint32_t *address);
bool write_into_device(node n);
fs_tab get_fs_tab();
bool write_fs_tab(fs_tab *root);
uint32_t* fs_alloc(fs_tab *root, int type, char*name);
bool fs_free(fs_tab* root, uint32_t *adr);
fs_tab init_fs(char*name, char*version, int size);
node fs_navigate(fs_tab *root,char*path);
int get_subdir_num(char*name);
bool nest_folder(fs_tab *root,char*path, uint32_t* adr);
bool fs_mkdir(fs_tab *root, char*path, char*name);
bool fs_rmdir(fs_tab *root, char*path, char*name);
bool fs_mv(fs_tab *root, char*path,char*name, char*dest);
bool fs_is_present(node ng , char* name);
int fs_is_present_pos(node ng, char*name);
bool update_fs_tab(fs_tab*root);
void fs_get_dir_content(fs_tab*root, char*path);
void fs_get_info(fs_tab*root);
bool fs_is_alloc(uint32_t*address);
bool fs_have_subdir(uint32_t*address);

uint32_t * virtual_path;
int __DEVICE__;

#ifndef ZENITH_C
#define ZENITH_C
#endif

#endif
