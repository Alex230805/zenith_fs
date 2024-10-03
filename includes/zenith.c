#define ZENITH_IMPLEMENTATION

#include "./zenith.h"

/* TODO: create and assembly function dedicated to the increment of 32bit number ( only 24 bit are used ) */

#ifndef ZENITH_EXCLUDE_INIT


extern void zenith_initFs(int size, char* part_name){

  // TODO: check this functions
 


  FILE* fstab_saved = fopen(LOCAL_SAVING_PATH, "wb");

  zenith_fstab fstab;
  strcpy(fstab.name, part_name);
  fstab.partition_size = size;
  fstab.free_page = size / ZENITH_NODE_SIZE;

  for(int i=0;i<NODE_COUNT;i++){
    fstab.allocated_page[i] = false;
  }

  uint32_t mem_adr = 0x00000000;
  int cursor = 0;

  for(int i=0;i<NODE_COUNT;i++){
    fstab.page_address[cursor] = (uint8_t)mem_adr;
    fstab.page_address[cursor+1] = (uint8_t)mem_adr>>8;
    fstab.page_address[cursor+2] = (uint8_t)mem_adr>>16;
    mem_adr += ZENITH_NODE_SIZE;
    cursor+=3;
  }

  int index = 0;

  for(int i=0; i<ZENITH_FSTAB_SIZE/255; i++){
    fstab.allocated_page[i] = true;
    index += 1;
  }
  
  fstab.first_node_lb = fstab.page_address[index+1];
  fstab.first_node_hb = fstab.page_address[index+2];
  fstab.first_node_xlb = fstab.page_address[index+3];

  if(virtual_drive == NULL){
    printf("Virtual driver not initialized yet");
    return; 
  };


  memcpy(virtual_drive, &fstab, ZENITH_FSTAB_SIZE);

  /* write dow a binary file with the fstab */
  fseek(fstab_saved, 0x00, SEEK_SET);
  fwrite(&fstab, 1, ZENITH_FSTAB_SIZE, fstab_saved); 
  fclose(fstab_saved);

  return;
}

extern int zenith_loadFs(){
  #ifdef VIRTUAL_DRIVE
 
  if(virtual_drive == NULL){
    printf("Virtual driver not initialized yet");
    return ABORTING_OPERATION; 
  };

  FILE *fp;

  fp = fopen(LOCAL_SAVING_PATH, "rb");
  if(fp == NULL) __FATAL_ERROR();

  fseek(fp, 0, SEEK_END);
  int flen = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  
  int lenght = fread(virtual_drive, 1, flen, fp);

  if(lenght == 0) __FATAL_ERROR();

  fclose(fp);

  return 0;
  #endif

  #ifndef VIRTUAL_DRIVE
   return ABORTING_OPERATION;
  #endif
  return 0;
}


#endif

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

    data = zenith_single_pop(adr_lb,adr_hb,adr_xlb,zenith_selected_driver);

    if(data == false){

      end = true;
      count+=1;

      zenith_single_push(adr_lb,adr_hb,adr_xlb, end, zenith_selected_driver);

    }else{
      full_address+=1;
      count+=1;
}
  }

  full_address = (uint8_t)DATA_FROM_FLAG_OFFSET + (count*3);
  
  for(int i=0;i<3;i++){

    adr_lb = (uint8_t)full_address;
    adr_hb = (uint8_t)full_address >> 8;
    adr_xlb = (uint8_t)full_address >> 16;

    switch(i){
      case 0:
        cache_adr_lb = zenith_single_pop(adr_lb,adr_hb,adr_xlb, zenith_selected_driver);
        break;
      case 1:
        cache_adr_hb = zenith_single_pop(adr_lb,adr_hb,adr_xlb, zenith_selected_driver);
        break;
      case 2:
        cache_adr_xlb = zenith_single_pop(adr_lb,adr_hb,adr_xlb, zenith_selected_driver);
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
  zenith_push(zenith_selected_driver);

  return;
}


extern void zenith_free(){

  bool end = false;
  uint16_t erase_position = 0x00000;


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

      adr_data = zenith_single_pop(adr_lb,adr_hb,adr_xlb, zenith_selected_driver);

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

  zenith_single_push(adr_lb,adr_hb,adr_xlb, end, zenith_selected_driver);

  return;
}



extern void zenith_set_target(uint8_t target){
  
  /* set target output for zenith i/o interface, take a look at the zenith.h header for more info */
 
  if(target < 0x0A){
    zenith_selected_driver = target;
  }  
    
  return;
}



extern bool zenith_is_present(uint8_t adr_lb, uint8_t adr_hb, uint8_t adr_xlb, char* name){
  bool state = false;
  bool end = false;
  uint8_t index = 0;
  
  uint8_t lb,hb,xlb;

    
  zenith_pop(adr_lb,adr_hb,adr_xlb, zenith_selected_driver);
  memcpy(cache_node_2, cache_node, ZENITH_NODE_SIZE);

  while(!end && index < CONTENT_SIZE){
      lb = cache_node_2->content[index];
      hb = cache_node_2->content[index+1]; 
      xlb = cache_node_2->content[index+2];

      if(lb != 0x00 && hb != 0x00 && xlb != 0x00){
        zenith_pop(lb,hb,xlb, zenith_selected_driver);
        if(strcmp(cache_node->name, name) == 0){
          end = true;
          state = true;
        }
      }
      index+=3;
  }

  memset(cache_node_2, 0x00, ZENITH_NODE_SIZE);
  memset(cache_node, 0x00, ZENITH_NODE_SIZE);
    
  return state;
}


extern void zenith_get_root(){

  
  uint32_t start_address = DATA_FROM_ROOT_OFFSET;

  uint8_t adr_lb = 0x00;
  uint8_t adr_hb = 0x00;
  uint8_t adr_xlb = 0x00;

  uint8_t lb,hb,xlb;

  for(int i=0;i<3;i++){
    lb = (uint8_t)start_address;
    hb = (uint8_t)start_address>>8;
    xlb = (uint8_t)start_address>>16;

    switch(i){
      case 0:
        adr_lb = zenith_single_pop(lb,hb,xlb, zenith_selected_driver);
        break;
      case 1:
        adr_hb = zenith_single_pop(lb,hb,xlb, zenith_selected_driver);
        break;
      case 2:
        adr_xlb = zenith_single_pop(lb,hb,xlb, zenith_selected_driver);
        break;
    }
    start_address+=1;
  }
    
  zenith_pop(adr_lb,adr_hb,adr_xlb, zenith_selected_driver);
  memcpy(zenith_root_node, cache_node, ZENITH_NODE_SIZE);

  return;
}

extern void zenith_navigate(char*path){

  char* token = strtok(path,"/");
   
  cache_adr_lb = zenith_root_node->adr_lb;
  cache_adr_hb = zenith_root_node->adr_hb;
  cache_adr_xlb = zenith_root_node->adr_xlb;

  while(token != NULL){
    if(!zenith_is_present(cache_adr_lb, cache_adr_hb, cache_adr_xlb, token)){
      printf("Error: no such file in the directory: %s", token);
      
      cache_adr_lb = 0x00;
      cache_adr_hb = 0x00;
      cache_adr_xlb = 0x00;

      return;
    }
    token = strtok(NULL, "/");
  }

  zenith_pop(cache_adr_lb,cache_adr_hb,cache_adr_xlb, zenith_selected_driver);

  return;
}
