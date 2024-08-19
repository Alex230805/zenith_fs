#ifndef FS_DIR_H
#define FS_DIR_H

#include "./fs_table.h"


extern node fs_navigate(fs_tab *root,char*path);
extern int get_subdir_num(char*name);
extern bool nest_folder(fs_tab *root,char*path, uint32_t* adr);
extern bool fs_is_empty(node *n);
extern bool fs_mkdir(fs_tab *root, char*path, char*name);
extern bool fs_rmdir(fs_tab *root, char*path, char*name);
extern bool fs_mv(fs_tab *root, char*path,char*name, char*dest);
extern void fs_get_dir_content(fs_tab*root, char*path);
extern bool fs_have_subdir(uint32_t*address);
extern bool fs_is_present(node ng , char* name);
extern int fs_is_present_pos(node ng, char*name);



#ifndef FS_DIR_IMPLEMENTATION
#define FS_DIR_IMPLEMENTATION


#endif

#endif