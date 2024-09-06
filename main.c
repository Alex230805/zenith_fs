#include <stdio.h>
#include <stdlib.h>

#include "includes/zth_fn.h"


int main(){
    /* test program */
    uint8_t size = CONTENT_SIZE;
    int size_of_fs_tab = sizeof(zenith_fstab);
    uint8_t dim = size+BOOL_SIZE+(UINT8_T_SIZE*7);
    printf("%d, full size: %d\n", size, dim);
    printf("%d or %x -> size of fstab at %d mb space",ZENITH_FSTAB_SIZE, ZENITH_FSTAB_SIZE, SIZE);
    return 0;
}