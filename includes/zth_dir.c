#define ZTH_DIR_IMPLEMENTATION

#include "./zth_dir.h"



/* zenith function to create a directory and store the address */


int zenith_mkdir(char*path, char*name){
  uint8_t stack_id = 0x00;

  uint8_t state = 0;   

  __NAME_CHECK(path,name);

  zenith_navigate(path);
  
  __ADDRESS_CHECK(cache_adr_lb, cache_adr_hb, cache_adr_xlb);
 
  if(zenith_is_present(cache_adr_lb, cache_adr_hb, cache_adr_xlb, name)){
    printf("Aborting: there is something with the same name in the destination folder");
    return WRITING_ERROR;
  }

  state = __zenith_mkdir(cache_adr_lb, cache_adr_hb,cache_adr_xlb, name, stack_id);

  return state;
}


int __zenith_mkdir(uint8_t lb,uint8_t hb,uint8_t xlb, char*name, uint8_t stack_id){
  
  __STACK_PROTECTION_SYSTEM();

  bool end = false;
  int state = 0;
  uint8_t adr_lb = 0x00;
  uint8_t adr_hb = 0x00;
  uint8_t adr_xlb = 0x00;

  uint8_t index = 0x00;

  uint8_t pointer = 0x00;


  zenith_pop(lb, hb, xlb, zenith_selected_driver);
  memcpy(cache_node_2, cache_node, ZENITH_NODE_SIZE);
  /* call to zenith_malloc to allocate a new directory */

  /* start to check where is possible to place the folder inside the target folder */
  while(!end && index < CONTENT_SIZE){

    /* first get the index for all the element in there */
    adr_lb = cache_node_2->content[index];
    adr_hb = cache_node_2->content[index+1];
    adr_xlb = cache_node_2->content[index+2];

    /* then check if those address are null or not, and if their value is null it's possible to place a new folder in there */
    if(adr_lb == 0x00 && adr_hb == 0x00 && adr_xlb == 0x00){ 
      end = true;
      pointer = index;

    }else{
      index+=3;
    }
  }
  if(index >= CONTENT_SIZE && pointer == 0x00){
    /* if there is no space available */
    if(cache_node_2->extended == true){
      /* check if it's possible to get the existing extension to continue */
      __EXTENSION_ADDRESS_CHECK(cache_node_2->extended_adr_lb,cache_node_2->extended_adr_hb,cache_node_2->extended_adr_xlb);
        cache_adr_lb = cache_node_2->extended_adr_lb;
        cache_adr_hb = cache_node_2->extended_adr_hb;
        cache_adr_xlb = cache_node_2->extended_adr_xlb;
        state = __zenith_mkdir(cache_adr_lb, cache_adr_hb, cache_adr_xlb, name, stack_id+=1);
    }else{    
      /* if there is no extension */
      
      zenith_malloc(DIR_TYPE, cache_node_2->name);
      /* allocate a new folder and link it with the current directory */
      cache_node_2->extended = true;
      cache_node_2->extended_adr_lb = cache_node->adr_lb;
      cache_node_2->extended_adr_hb = cache_node->adr_hb;
      cache_node_2->extended_adr_xlb = cache_node->adr_xlb;
      /* cache the address of the current folder */
      cache_adr_lb = cache_node->adr_lb;
      cache_adr_hb = cache_node->adr_hb;
      cache_adr_xlb = cache_node->adr_xlb;
      /* copy the current cache_node_2 into the main cache_node and push it into drive to save changes */
      memcpy(cache_node, cache_node_2, ZENITH_NODE_SIZE);
      zenith_push(zenith_selected_driver);
      /* continue to search inside the allocated folder, so put the new folder in the first place */
      state = __zenith_mkdir(cache_adr_lb, cache_adr_hb, cache_adr_xlb, name, stack_id+=1);
    }

  }else{
    zenith_malloc(DIR_TYPE, name);
    cache_node_2->content[pointer] = cache_adr_lb;
    cache_node_2->content[pointer+1] = cache_adr_hb;
    cache_node_2->content[pointer+2] = cache_adr_xlb;
    memcpy(cache_node, cache_node_2, ZENITH_NODE_SIZE);
    zenith_push(zenith_selected_driver);
    state = 0;
  }


  return state;
}


/* zenith function to remove a directory and return a boolean state abount whats happened */

int zenith_rmdir(char*path, char*name){
  uint8_t stack_id = 0x00;
  int state = 0;

  __NAME_CHECK(path,name);
  zenith_navigate(path);
  __ADDRESS_CHECK(cache_adr_lb, cache_adr_hb, cache_adr_xlb);

  state =__zenith_rmdir(cache_adr_lb, cache_adr_hb, cache_adr_xlb, name, stack_id);
  
  return state;
}


int __zenith_rmdir(uint8_t lb, uint8_t hb, uint8_t xlb, char*name, uint8_t stack_id){
  
  __STACK_PROTECTION_SYSTEM();

  bool end = false;
  uint8_t state = 0;

  uint8_t index = 0x00;

  uint8_t adr_lb = 0x00;
  uint8_t adr_hb = 0x00;
  uint8_t adr_xlb = 0x00;


  /* get the folder and save it inside the second cached node */
  zenith_pop(lb, hb, xlb,zenith_selected_driver);
  memcpy(cache_node_2, cache_node, ZENITH_NODE_SIZE);

  while(!end && index < CONTENT_SIZE){
    /* get the address */
    adr_lb = cache_node_2->content[index];
    adr_hb = cache_node_2->content[index+1];
    adr_xlb = cache_node_2->content[index+2];

    /* grep the folder or the file inside */
    if(adr_lb != 0x00 && adr_hb != 0x00 && adr_xlb != 0x00){
      zenith_pop(adr_lb, adr_hb,adr_xlb, zenith_selected_driver);

      if(strcmp(cache_node->name, name) == 0 && cache_node->type == DIR_TYPE){
        /* save node address for wipe it out of the filesystem table */
        cache_adr_lb= cache_node->adr_lb;
        cache_adr_hb = cache_node->adr_hb;
        cache_adr_xlb = cache_node->adr_xlb;
        /* call the zenith_free function */
        zenith_free();
        /* clear the pointer inside the folder */
        cache_node_2->content[index] = 0x00;
        cache_node_2->content[index+1] = 0x00;
        cache_node_2->content[index+2] = 0x00;
      
        /* end cicle */
        state = 0;
        end = true;
    }
  }else{
      /* increment the pointer if there is no element named in the same way */
      index +=3;
    }
  }
  /* if there is no menber found */
  if(!state && index >= CONTENT_SIZE){
    /* check if may be in the extension of the folder */
    if(cache_node_2->extended == false){
      printf("Error: no member found");
      state = 2;
      return state;
    }
    /* then check if those pointers are not NULL */
    
    __EXTENSION_ADDRESS_CHECK(cache_node_2->extended_adr_lb,cache_node_2->extended_adr_hb,cache_node_2->extended_adr_xlb);
    /* then copy it into the cached address */

    cache_adr_lb = cache_node_2->extended_adr_lb; 
    cache_adr_hb = cache_node_2->extended_adr_hb; 
    cache_adr_xlb = cache_node_2->extended_adr_xlb;

    /* and recall the remove dir sub-function  */
    stack_id+=1;
    state = __zenith_rmdir(cache_adr_lb, cache_adr_hb, cache_adr_xlb, name, stack_id); 
    
  }
  /* prepare to push */
  memcpy(cache_node, cache_node_2, ZENITH_NODE_SIZE);
  /* push updated node */
  zenith_push(zenith_selected_driver);

  return state;
}


/* zenith function to move a directory from a destination to another, you can change the name in the process */

int zenith_mv(char*start_path, char*name, char*dest_path, char*dest_name){
  uint8_t state_pick = 0x00;
  uint8_t state_put = 0x00;
 
  __NAME_CHECK(start_path, name);
  __NAME_CHECK(dest_path, dest_name);
  zenith_navigate(start_path);

  cache_adr_lb_2 = cache_adr_lb;
  cache_adr_hb_2 = cache_adr_hb;
  cache_adr_xlb_2 = cache_adr_xlb;
  
  zenith_navigate(dest_path);


  if(zenith_is_present(cache_adr_lb_2, cache_adr_hb_2, cache_adr_xlb_2, name) == 0){
    printf("No such file in the directory");
    return READING_ERROR;
  }

  if(zenith_is_present(cache_adr_lb, cache_adr_hb, cache_adr_xlb, dest_name)){
    printf("Aborting: there is something with the same name in the destination folder");
    return WRITING_ERROR;
  }

  state_pick = __zenith_move_pick(cache_adr_lb_2, cache_adr_hb_2, cache_adr_xlb_2, name, 0);
  
  if(state_pick != 0){
    printf("Aborting .. \n");
    return ABORTING_OPERATION;
  }

  state_put = __zenith_move_put(cache_adr_lb, cache_adr_hb, cache_adr_xlb, dest_name, 0);

  return state_put;

}


int __zenith_move_pick(uint8_t adr_lb, uint8_t adr_hb, uint8_t adr_xlb, char*name, uint8_t stack_id){
  
  __STACK_PROTECTION_SYSTEM();

  bool end = false;
  uint8_t index = 0x00;
  uint8_t state = 0x00;

  uint8_t lb, hb, xlb;

  zenith_pop(adr_lb, adr_hb, adr_xlb, zenith_selected_driver);
  memcpy(cache_node_2, cache_node, ZENITH_NODE_SIZE);

  while(!end && index < CONTENT_SIZE){
    lb = cache_node_2->content[index];
    hb = cache_node_2->content[index+1];
    xlb = cache_node_2->content[index+2];

    zenith_pop(lb,hb,xlb, zenith_selected_driver);
    if(strcmp(cache_node->name, name) == 0 && cache_node->type == DIR_TYPE){
      cache_adr_lb_2  = cache_node->content[index];
      cache_adr_hb_2  = cache_node->content[index+1];
      cache_adr_xlb_2 = cache_node->content[index+2];

      cache_node_2->content[index] = 0x00;
      cache_node_2->content[index+1] = 0x00;
      cache_node_2->content[index+2] = 0x00;
      state = 0;
      end = true;
    }else{
      index += 3;
    }
  }

  if(index >= CONTENT_SIZE && end == false){
      if(cache_node_2->extended == true){
        __EXTENSION_ADDRESS_CHECK(cache_node_2->extended_adr_lb, cache_node_2->extended_adr_hb,cache_node_2->extended_adr_xlb);
        cache_adr_lb_2 = cache_node_2->extended_adr_lb;
        cache_adr_hb_2 = cache_node_2->extended_adr_hb;
        cache_adr_xlb_2 = cache_node_2->extended_adr_xlb;

        state = __zenith_move_pick(cache_adr_lb_2, cache_adr_hb_2, cache_adr_xlb_2, name, stack_id+=1);
      }else{
        printf("No such file in the directory\n");
        return FILE_NOT_FOUND;
    }
    __FEATURE_INTERRUPT_RET(NOT_IMPLEMENTED_YET);
  }
  memcpy(cache_node, cache_node_2, ZENITH_NODE_SIZE);
  zenith_push(zenith_selected_driver);

  return state;
}

int __zenith_move_put(uint8_t adr_lb, uint8_t adr_hb, uint8_t adr_xlb, char*name, uint8_t stack_id){
  __STACK_PROTECTION_SYSTEM();

  uint8_t state = 0x00;
  bool end = false;
  uint8_t lb, hb, xlb;
  uint8_t index = 0x00;
  uint8_t lstate = 0x00;

  zenith_pop(adr_lb, adr_hb, adr_xlb, zenith_selected_driver);
  memcpy(cache_node_2, cache_node, ZENITH_NODE_SIZE);

  while(!end && index < (uint8_t)CONTENT_SIZE){
    lb = cache_node_2->content[index];
    hb = cache_node_2->content[index+1];
    xlb = cache_node_2->content[index+2];
 
    if(lb == 0x00 && adr_hb == 0x00 && adr_xlb == 0x00){
      cache_node->content[index] = cache_adr_lb_2;
      cache_node->content[index+1] = cache_adr_hb_2;
      cache_node->content[index+2] = cache_adr_xlb_2;
      zenith_pop(cache_adr_lb_2, cache_adr_hb, cache_adr_xlb, zenith_selected_driver);
      strcpy(cache_node->name, name);
      zenith_push(zenith_selected_driver);
      state = 0;
      end = true;
    }else{
      index +=3;
    }
  }
  
  if(!end && index >= CONTENT_SIZE){
    if(cache_node_2->extended){
      __EXTENSION_ADDRESS_CHECK(cache_node_2->extended_adr_lb,cache_node_2->extended_adr_hb,cache_node_2->extended_adr_xlb);   
      cache_adr_lb = cache_node_2->extended_adr_lb;
      cache_adr_hb = cache_node_2->extended_adr_hb;
      cache_adr_xlb = cache_node_2->extended_adr_xlb;
      
      state = __zenith_move_put(cache_adr_lb, cache_adr_hb, cache_adr_xlb, name, stack_id+=1);

    }else{
      cache_adr_lb = cache_node_2->adr_lb;
      cache_adr_hb = cache_node_2->adr_hb;
      cache_adr_xlb = cache_node_2->adr_xlb;
      
      lstate = __zenith_mkdir(cache_adr_lb, cache_adr_hb, cache_adr_xlb, cache_node_2->name, 0);
      if(lstate != 0){
        __FATAL_ERROR();
      }
      cache_node_2->extended = true;
      cache_adr_lb = cache_node_2->extended_adr_lb;
      cache_adr_lb = cache_node_2->extended_adr_hb;
      cache_adr_lb = cache_node_2->extended_adr_xlb;

      memcpy(cache_node, cache_node_2, ZENITH_NODE_SIZE);
      zenith_push(zenith_selected_driver);
      
      state = __zenith_move_put(cache_adr_lb, cache_adr_hb, cache_adr_xlb, name, stack_id+=1);
    
    }
  }

  memcpy(cache_node, cache_node_2, ZENITH_NODE_SIZE);
  zenith_push(zenith_selected_driver);

  return state;
}


