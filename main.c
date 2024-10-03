#include <stdio.h>
#include <stdlib.h>

#include "includes/zth_fn.h"


int main(){
  Zth.init();
  Zth.zenith_initFs(SIZE, "test");
  Zth.zenith_loadFs();

  return 0;
}
