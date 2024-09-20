#define ZTH_FN_IMPLEMENTATION

#include "./zth_fn.h"


extern void _initialize(){
  #ifdef VIRTUAL_DRIVE

  virtual_drive = (uint8_t*)malloc(SIZE);

  #endif 
  
  cache_node = (zenith_general_node*)malloc(ZENITH_NODE_SIZE);
  zenith_root_node = (zenith_general_node*)malloc(ZENITH_NODE_SIZE);
}
