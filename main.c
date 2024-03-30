#include <stdio.h>
#include <stdlib.h>


#define FS_IMPLEMENTATION
#define VIRTUAL_DISK

#include "include/fs.h"

int main(void){
    fs_tab root = init_fs("generic_g", "1.0", MEDIUM_SIZE);
    

    printf("Hello, World\n");
    return 0;


#ifdef VIRTUAL_DISK
    free(virtual_path);
#endif
}
