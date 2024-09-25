#define ZTH_DIR_IMPLEMENTATION

#include "./zth_dir.h"



/* zenith function to create a directory and store the address */


int zenith_mkdir(char*path, char*name){

  /* node: zenith_malloc is only called out after the essential checks like the name or 
   * the destination folder ( if exist ) to just avoid poissible allocation with no 
   * usage, pratically occupying space for nothing
   * */
  int state = 0;   

  /* check if the path or the name are a null pointer */
  if(path == NULL || name == NULL){
    /* return error */
    printf("Error: no such name or path provided");
    state = 2;
    return state;
  }
  /* go to the destination folder */
  zenith_navigate(path);
  if(cache_adr_lb != 0x00 && cache_adr_hb != 0x00 && cache_adr_xlb != 0x00 ){
    /* if cache_adr is null that mean zenith_navigare has failed to go inside the indicated path,so return an error and exit from mkdir */

    printf("Error: failed to reach the destination provided");
    state = 2;
    return state;
  }
 
  if(zenith_is_present(cache_adr_lb, cache_adr_hb, cache_adr_xlb, name)){
    printf("Error: found something with the same name");
    return state;
  }

  state = __zenith_mkdir(cache_adr_lb, cache_adr_hb,cache_adr_xlb, name);

  switch(state){
    case 0:
        printf("Unable to create folder");
        break;
    case 1:
        break;
    case 2:
        printf("Error: aborting operation");
        break;
  }
  

  return state;
}


int __zenith_mkdir(uint8_t lb,uint8_t hb,uint8_t xlb, char*name){
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
  if(index == CONTENT_SIZE && pointer == 0x00){
    /* if there is no space available */
    if(cache_node_2->extended == true){
      /* check if it's possible to get the existing extension to continue */
      if(cache_node_2->extended_adr_lb != 0x00 && cache_node_2->extended_adr_hb != 0x00 && cache_node_2->extended_adr_xlb != 0x00){
        cache_adr_lb = cache_node_2->extended_adr_lb;
        cache_adr_hb = cache_node_2->extended_adr_hb;
        cache_adr_xlb = cache_node_2->extended_adr_xlb;
        state = __zenith_mkdir(cache_adr_lb, cache_adr_hb, cache_adr_xlb, name);
      }else{
        /* if there is an error with the address then flag it with an error code */
        state = 2;
      }

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
      /* continue to search insside the allocated folder */
      state = __zenith_mkdir(cache_adr_lb, cache_adr_hb, cache_adr_xlb, name);
    }

  }else{
    zenith_malloc(DIR_TYPE, name);
    cache_node_2->content[pointer] = cache_adr_lb;
    cache_node_2->content[pointer+1] = cache_adr_hb;
    cache_node_2->content[pointer+2] = cache_adr_xlb;
    memcpy(cache_node, cache_node_2, ZENITH_NODE_SIZE);
    zenith_push(zenith_selected_driver);
    state = 1;
  }
  return state;
}


/* zenith function to remove a directory and return a boolean state abount whats happened */

int zenith_rmdir(char*path, char*name){

  int state = 0;

   /* check if the path or the name are a null pointer */
  if(path == NULL || name == NULL){
    /* return error */
    printf("Error: no such name or path provided");
    return state;
  }
  /* go to the destination folder */
  zenith_navigate(path);
  if(cache_adr_lb != 0x00 && cache_adr_hb != 0x00 && cache_adr_xlb != 0x00 ){
    /* if cache_adr is null that mean zenith_navigare has failed to go inside the indicated path,so return an error and exit from mkdir */

    printf("Error: failed to reach the destination provided");
    return state;
  } 

  state =__zenith_rmdir(cache_adr_lb, cache_adr_hb, cache_adr_xlb, name);
  
  switch(state){
    case 0:
        printf("No such member named this way");
        break;
    case 1:
        break;
    case 2:
        printf("Error: somehting went wrong");
        break;
  }
  

  return state;
}


int __zenith_rmdir(uint8_t lb, uint8_t hb, uint8_t xlb, char*name){
  bool end = false;
  int state = 0;

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
        cache_adr_lb = cache_node->adr_lb;
        cache_adr_hb = cache_node->adr_hb;
        cache_adr_xlb = cache_node->adr_xlb;
        /* call the zenith_free function */
        zenith_free();
        /* clear the pointer inside the folder */
        cache_node_2->content[index] = 0x00;
        cache_node_2->content[index+1] = 0x00;
        cache_node_2->content[index+2] = 0x00;
        /* prepare to push */
        memcpy(cache_node, cache_node_2, ZENITH_NODE_SIZE);
        /* push updated node */
        zenith_push(zenith_selected_driver);
        /* end cicle */
        state = 1;
        end = true;
    }
  }else{
      /* increment the pointer if there is no element named in the same way */
      index +=3;
    }
  }
  /* if there is no menber found */
  if(!state){
    /* check if may be in the extension of the folder */
    if(cache_node_2->extended == false){
      printf("Error: no member found");
      state = 2;
      return state;
    }
    /* then check if those pointers are not NULL */
    if(cache_node_2->extended_adr_lb == 0x00 && cache_node_2->extended_adr_hb == 0x00 && cache_node_2->extended_adr_xlb == 0x00){
      printf("Error: null pointer detected in the folder extension address");
      state = 2;
      return state;
    }
    /* then copy it into the cached address */

    cache_adr_lb = cache_node_2->extended_adr_lb; 
    cache_adr_hb = cache_node_2->extended_adr_hb; 
    cache_adr_xlb = cache_node_2->extended_adr_xlb;

    /* and recall the remove dir sub-function  */
    
    state = __zenith_rmdir(cache_adr_lb, cache_adr_hb, cache_adr_xlb, name); 
    
  }

  return state;


}


/* zenith function to move a directory from a destination to another, you can change the name in the process */

void zenith_mv(char*start_path, char*name, char*dest_path, char*dest_name){

  return;
}



