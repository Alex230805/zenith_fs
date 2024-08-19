#ifndef FS_FILE_H
#define FS_FILE_H

#include "./fs_table.h"
#include "./fs_dir.h"

extern uint32_t* fs_is_file_present(fs_tab*root,node*n, char*filename);
extern void fs_file_ram_copy(fs_tab*root, node*n,char*ptr, int start_point);

extern char* fs_fopen(fs_tab* root, char* path, char* filename);
extern void fs_fwrite(fs_tab root, uint32_t* file_pointer,char* path, char* filename);


node* __current_file_open__ = NULL;

#ifndef FS_FILE_IMPLEMENTATION
#define FS_FILE_IMPLEMENTATION

#endif

#endif