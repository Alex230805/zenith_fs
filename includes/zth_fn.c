#define ZTH_FN_IMPLEMENTATION

#include "./zth_fn.h"


extern void _initialize(){
  #ifdef VIRTUAL_DRIVE

  virtual_drive = (uint8_t*)malloc(UINT8_T_SIZE * (SIZE/255) );

  #endif 
  
  cache_node = (zenith_general_node*)malloc(ZENITH_NODE_SIZE);
  cache_node_2 = (zenith_general_node*)malloc(ZENITH_NODE_SIZE);
  zenith_root_node = (zenith_general_node*)malloc(ZENITH_NODE_SIZE);

  cache_adr_lb = 0x00;
  cache_adr_hb = 0x00;
  cache_adr_xlb = 0x00;

  cache_adr_lb_2 = 0x00;
  cache_adr_hb_2 = 0x00;
  cache_adr_xlb_2 = 0x00;

  //zenith_get_root();
}
