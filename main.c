#include <stdio.h>
#include <stdlib.h>


#include "include/fs.h"

int main(void){
    enum device_type tp = VIRTUAL_DISK;
    fs_set_device(tp);
    fs_tab root = init_fs("dev/sdc_virtual", "Zenith 1.0", MEDIUM_SIZE);
    fs_mkdir(&root,"/", "usr");
    fs_mkdir(&root,"/", "share");
    fs_mkdir(&root,"/", "bin");
    fs_mkdir(&root,"/", "lib");
    fs_mkdir(&root,"/", "etc");
    fs_mkdir(&root,"/", "usr");
    fs_mkdir(&root,"/", "home");
    fs_mkdir(&root,"/home/", "am");
    fs_mkdir(&root,"/home/am/", "Desktop");
    fs_mkdir(&root,"/home/am/", "Download");
    fs_mkdir(&root,"/home/am/", "Document");
    fs_mkdir(&root,"/home/am/", "Images");
    fs_mkdir(&root,"/home/am/", "Music");
    fs_get_dir_content(&root, "/");
    fs_get_dir_content(&root, "/home");
    fs_get_dir_content(&root, "/home/am");
    fs_rmdir(&root, "/home/am", "Desktop");
    fs_get_dir_content(&root,"/home/am");
    free(virtual_path);
    return 0;
}
