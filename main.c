#include <stdio.h>
#include <stdlib.h>

#include "includes/zth_fn.h"


int main(){
    Zth.init();
    Zth.zenith_initFs(SIZE, "test");
    Zth.zenith_loadFs(SIZE);

    zenith_fstab * test = (zenith_fstab*)malloc(UINT8_T_SIZE* (SIZE/255));
    memcpy(test, virtual_drive, ZENITH_FSTAB_SIZE);
 
    /* some code here */
    return 0;
}
