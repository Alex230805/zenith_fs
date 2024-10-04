#include <stdio.h>
#include <stdlib.h>

#include "includes/zth_fn.h"


int main(){
  Zth.init();
  Zth.zenith_initFs(SIZE, "test");
  Zth.zenith_loadFs();

 
  //Zth.zenith_malloc(DIR_TYPE, "test");
  //Zth.zenith_free();
  return 0;
}
