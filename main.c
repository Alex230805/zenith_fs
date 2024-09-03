#include <stdio.h>
#include <stdlib.h>

#include "include/fs.h"

int main(void){
    fs_tab root = init_fs("dev/sdc_virtual", "Zenith 1.0", MEDIUM_SIZE);
    free(virtual_path);
    return 0;
}
