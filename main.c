#include <stdio.h>
#include <stdlib.h>

#define FS_IMPLEMENTATION

#include "include/fs.h"

int main(void){
    enum device_type tp = VIRTUAL_DISK;
    fs_set_device(tp);
    fs_tab root = init_fs("sddc_virtual", "1.0", MEDIUM_SIZE);

    fs_mkdir(&root,"/", "test_dir");
    fs_mkdir(&root,"/", "test_dir2");
    fs_mkdir(&root,"/", "test_dir3");
    fs_mkdir(&root,"/test_dir3/", "test_dir3");
    fs_mkdir(&root,"/test_dir3/", "test_dir4");
    fs_mkdir(&root,"/test_dir3/", "sdfs");
    fs_mkdir(&root,"/test_dir3/", "656");
    fs_mkdir(&root,"/test_dir/", "ggf");
    fs_mkdir(&root,"/test_dir/", "473");
    fs_mkdir(&root,"/test_dir/", "472");
    printf("content of /:\n");
    fs_get_dir_content(&root,"/");
    printf("content of /test_dir3/:\n");
    fs_get_dir_content(&root,"/test_dir3/");
    printf("content of /test_dir/:\n");
    fs_get_dir_content(&root,"/test_dir");
    printf("content of /test_dir/:\n");
    fs_rmdir(&root, "/test_dir/", "ggf");
    fs_get_dir_content(&root,"/test_dir");
    fs_get_info(&root);    
    printf("Hello, World\n");
    free(virtual_path);
    return 0;


#ifdef VIRTUAL_DISK
    free(virtual_path);
#endif
}
