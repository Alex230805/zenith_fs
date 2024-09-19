#define ZENITH_IMPLEMENTATION

#include "./zenith.h"

/* TODO: create and assembly function dedicated to the increment of 32bit number ( only 24 bit are used ) */

extern void zenith_initFs(int size, char* part_name, uint8_t drive_type){
  #ifndef VIRTUAL_DRIVE

  FILE* fstab_saved = fopen(LOCAL_SAVING_PATH, "w+");

  #endif

  zenith_fstab fstab;
  strcpy(fstab.name, part_name);
  fstab.partition_size = size;
  fstab.free_page = size / ZENITH_NODE_SIZE;

  for(int i=0;i<NODE_COUNT;i++){
    fstab.allocated_page[i] = false;
  }

  #ifdef VIRTUAL_DRIVE

  for(int i=ZENITH_FSTAB_SIZE;i<NODE_COUNT;i++){

    fstab.page_address[i] = (zenith_general_node*)&virtual_drive[i*NODE_COUNT];

  }

  fstab.allocated_page[0] = true;
  memcpy(&fstab, virtual_drive, ZENITH_FSTAB_SIZE);

  fstab_global = fstab;

  #endif

  #ifndef VIRTUAL_DRIVE

    uint32_t mem_adr = 0x00000000;

    for(int i=ZENITH_FSTAB_SIZE;i<NODE_COUNT;i++){
        fstab.page_address[i] = (uint8_t)mem_adr;
        fstab.page_address[i+1] = (uint8_t)mem_adr>>8;
        fstab.page_address[i+2] = (uint8_t)mem_adr>>16;
        mem_adr+=NODE_COUNT;
    }

    fstab.first_node_lb = fstab.page_address[0];
    fstab.first_node_hb = fstab.page_address[1];
    fstab.first_node_xlb = fstab.page_address[2];


    /* write dow a binary file with the fstab */
    fseek(fstab_saved, 0x00, SEEK_SET);
    fwrite(&fstab, ZENITH_FSTAB_SIZE, 1, fstab_saved); 
    fclose(fstab_saved);
    #endif

    return;
}


extern void zenith_malloc(int type, char*name){
    
    /* do a check for the types */

  zenith_general_node node;
  zenith_general_node* node_adr = &node;

  uint8_t true_write = 0xff;

  strcpy(node.name, name);
  node.perm = R_PERM | W_PERM;
  node.extended = false;
  
  switch(type){
    case FILE_TYPE:
        node.perm = node.perm | PERM_MASK_FILE;
        break;
    case DIR_TYPE:
        node.perm = node.perm | PERM_MASK_DIR;
        break;
    default:
        return;
  }
  node.type = type;


  #ifdef VIRTUAL_DRIVE    

  node.extended_adr = NULL;
  int pos = 0;

  while(fstab_global.allocated_page[i] == true){
    pos+=1;
  }
  
  fstab_global.allocated_page[pos+1] = true;

  node.current_address = fstab_global.page_address[pos+1];
  memcpy((void*)node.current_address, &node, ZENITH_NODE_SIZE);
  
  cache_node = node.current_address;

  #endif

  #ifndef VIRTUAL_DRIVE

  node.extended_adr_lb = 0x00;
  node.extended_adr_hb = 0x00;
  node.extended_adr_xlb = 0x00;

  /* read directly from device for checking and writing things, ONLY for malloc and free, on other just take reference from the device interface layer ( zenith_push, zenith_pop ) */

  uint8_t adr_lb = CHAR_SIZE + UINT8_T_SIZE * 2; /* starting point */
  uint8_t adr_hb = 0x00;
  uint8_t adr_xlb = 0x00;
  uint32_t full_address = (adr_xlb << 16) | (adr_hb << 8) |( adr_lb );

  uint8_t data = 0x00;
  uint16_t count = 0;
   
  bool end = false;
  
  while(!end){

    adr_lb = (uint8_t)full_address;
    adr_hb = (uint8_t)full_address >> 8;
    adr_xlb = (uint8_t)full_address >> 16;

    memcpy((void*)ADR_LB_PORT, &adr_lb, UINT8_T_SIZE);
    memcpy((void*)ADR_HB_PORT, &adr_hb, UINT8_T_SIZE);
    memcpy((void*)ADR_XLB_PORT, &adr_xlb, UINT8_T_SIZE);
    memcpy(&data,(void*)DATA_PORT, UINT8_T_SIZE);

    if(data == false){

      end = true;
      count+=1;

      memcpy((void*)ADR_LB_PORT, &adr_lb, UINT8_T_SIZE);
      memcpy((void*)ADR_HB_PORT, &adr_hb, UINT8_T_SIZE);
      memcpy((void*)ADR_XLB_PORT, &adr_xlb, UINT8_T_SIZE);
      memcpy((void*)DATA_PORT, &true_write, UINT8_T_SIZE);

    }else{
      full_address+=1;
      count+=1;
    }
  }

  full_address = DATA_FROM_FLAG_OFFSET + (count*3);
  
  for(int i=0;i<3;i++){

    adr_lb = (uint8_t)full_address;
    adr_hb = (uint8_t)full_address >> 8;
    adr_xlb = (uint8_t)full_address >> 16;

    memcpy((void*)ADR_LB_PORT, &adr_lb, UINT8_T_SIZE);
    memcpy((void*)ADR_HB_PORT, &adr_hb, UINT8_T_SIZE);
    memcpy((void*)ADR_XLB_PORT, &adr_xlb, UINT8_T_SIZE);

    switch(i){
      case 0:
        memcpy(&cache_adr_lb, (void*)DATA_PORT, UINT8_T_SIZE);
        break;
      case 1:
        memcpy(&cache_adr_hb, (void*)DATA_PORT, UINT8_T_SIZE);
        break;
      case 2:
        memcpy(&cache_adr_xlb, (void*)DATA_PORT, UINT8_T_SIZE);
        break;
      default:
        break;
    }

    full_address+=1;
  }
  node.adr_lb = cache_adr_lb;
  node.adr_hb = cache_adr_hb;
  node.adr_xlb = cache_adr_xlb;

  memcpy(&cache_node[0],&node, ZENITH_NODE_SIZE);
  zenith_push(0);

#endif

  return;
}


extern void zenith_free(){

  bool end = false;
  uint16_t erase_position = 0x00000;

  #ifdef VIRTUAL_DRIVE

  /* free using the virtual drive */

  while(!end && erase_position < (uint16_t)NODE_COUNT){
    if((void*)fstab_global.page_address[erase_position] == cache_adr){
      end = true;
      fstab_global.allocated_page[erase_position] = false;
    }else{
      erase_position+=1;
    }
  }

  #endif

  #ifndef VIRTUAL_DRIVE

  uint32_t full_address = DATA_FROM_FLAG_OFFSET;
  uint8_t count = 0;
  uint8_t adr_data = 0x00;

  uint8_t adr_lb = (uint8_t)full_address ;
  uint8_t adr_hb = (uint8_t)full_address>>8;
  uint8_t adr_xlb = (uint8_t)full_address>>16;

  /* cicle from the address table and find the address index equal to the cached one */

  while(!end && erase_position < (uint16_t)NODE_COUNT){
    for(int i=0;i<3;i++){
      adr_lb = (uint8_t)full_address ;
      adr_hb = (uint8_t)full_address>>8;      
      adr_xlb = (uint8_t)full_address>>16;

      memcpy((void*)ADR_LB_PORT, &adr_lb, UINT8_T_SIZE);
      memcpy((void*)ADR_HB_PORT, &adr_hb, UINT8_T_SIZE);
      memcpy((void*)ADR_XLB_PORT, &adr_xlb, UINT8_T_SIZE);

      memcpy(&adr_data, (void*)DATA_PORT, UINT8_T_SIZE);

      switch(i){
        case 0:
            if(adr_data == cache_adr_lb) count+=1;
            break;
        case 1:
            if(adr_data == cache_adr_hb) count+=1;
            break;
        case 2:
            if(adr_data == cache_adr_xlb) count+=1;
            break;
      }
      full_address+=1;
    }
    if(count == 3){
      end = true;
    }else{
      count = 0;
      erase_position+=1;
    }
  }

  /* calculate the exact address and erase the flag from the address allocation table */
  full_address = UINT8_T_SIZE*2 + CHAR_SIZE + erase_position;
  
  adr_lb = (uint8_t)full_address ;
  adr_hb = (uint8_t)full_address>>8;      
  adr_xlb = (uint8_t)full_address>>16;

  end = 0x00;

  memcpy((void*)ADR_LB_PORT, &adr_lb, UINT8_T_SIZE);
  memcpy((void*)ADR_HB_PORT, &adr_hb, UINT8_T_SIZE);
  memcpy((void*)ADR_XLB_PORT, &adr_xlb, UINT8_T_SIZE);

  /* use the end flag from before to write down a false statement for the referred flag */
  memcpy((void*)DATA_PORT, &end, UINT8_T_SIZE);

  #endif

  return;
}
