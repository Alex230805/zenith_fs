#include <stdio.h>
#include <stdlib.h>

#include "include/fs.h"

int main(void){
    fs_tab root = init_fs("dev/sdc_virtual", "Zenith 1.0", MEDIUM_SIZE);
    fs_get_info(&root);
    
    fs_mkdir(&root,"/", "home");
    fs_mkdir(&root,"/", "bin");
    fs_mkdir(&root,"/", "lib");
    fs_mkdir(&root,"/", "usr");
    fs_mkdir(&root,"/", "var");
    fs_mkdir(&root,"/", "etc");
    fs_mkdir(&root,"/", "tmp");
    fs_mkdir(&root,"/", "opt");


    fs_mkdir(&root,"/home", "am");
    fs_mkdir(&root,"/home/am", "Documents");
    fs_mkdir(&root,"/home/am", "Downloads");
    fs_mkdir(&root,"/home/am", "Images");
    fs_mkdir(&root,"/home/am", "Desktop");
    fs_mkdir(&root,"/home/am", "Scripts");
    fs_mkdir(&root,"/home/am", "Pictures");

    fs_get_dir_content(&root,"/");
    fs_get_dir_content(&root,"/home");
    fs_get_dir_content(&root,"/etc");

    char* document = fs_fopen(&root, "/home/am/Desktop", "text.txt");
    if(document != NULL){

    }
    printf("Document written into ram\n");
    fs_get_info(&root);
    free(virtual_path);
    return 0;
}
