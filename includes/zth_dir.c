#define ZTH_DIR_IMPLEMENTATION

#include "./zth_dir.h"



/* zenith function to create a directory and store the address */

void zenith_mkdir(char*path, char*name){

  /* node: zenith_malloc is only called out after the essential checks like the name or 
   * the destination folder ( if exist ) to just avoid poissible allocation with no 
   * usage, pratically occupying space for nothing
   * */
  bool end = false;
  
  #ifndef VIRTUAL_DRIVE
  
  uint8_t adr_lb = 0x00;
  uint8_t adr_hb = 0x00;
  uint8_t adr_xlb = 0x00;

  uint8_t index = 0x00;

  /* check if the path or the name are a null pointer */
  if(path == NULL || name == NULL){
    /* return error */
    printf("Error: no such name or path provided");
    return;
  }
  /* go to the destination folder */
  zenith_navigate(path);
  if(cache_adr_lb != 0x00 && cache_adr_hb != 0x00 && cache_adr_xlb != 0x00 ){
    /* if cache_adr is null that mean zenith_navigare has failed to go inside the indicated path,so return an error and exit from mkdir */

    printf("Error: failed to reach the destination provided");
    return;
  }
 
  if(zenith_is_present(cache_adr_lb, cache_adr_hb, cache_adr_xlb, name) == false){
    /* if there is no element named in the same way */

    /* get the target folder */
    zenith_pop(cache_adr_lb, cache_adr_hb, cache_adr_xlb, zenith_selected_driver);
    /* copy it in the 2nd cache node in ram */
    memcpy(cache_node_2, cache_node, ZENITH_NODE_SIZE);
    /* call to zenith_malloc to allocate a new directory */
    zenith_malloc(DIR_TYPE, name);
    
    /* start to check where is possible to place the folder inside the target folder */
    while(!end && index < CONTENT_SIZE){

      /* first get the index for all the element in there */
      adr_lb = cache_node_2->content[index];
      adr_hb = cache_node_2->content[index+1];
      adr_xlb = cache_node_2->content[index+2];

      /* then check if those address are null or not, and if their value is null it's possible to place a new folder in there */
      if(adr_lb == 0x00 && adr_hb == 0x00 && adr_xlb == 0x00){
        /* saving the address returned by zenith_malloc and save_it */
        cache_node_2->content[index] = cache_adr_lb;
        cache_node_2->content[index+1] = cache_adr_hb;
        cache_node_2->content[index+2] = cache_adr_xlb;
        /* end the cicle */
        end = true;
      }else{
        /* else check for the next cluster of address */
        index+=3;
      }
    }
    if(index == CONTENT_SIZE){
      /* no space available, use the extension address capabilities */
      zenith_free();
      printf("NOTE: unable to allocate, node extension address integration is under development");
      printf("NOTE: unable to create beacuse of lack of space");
      return;
    }
    /* move the node to the main one */
    memcpy(cache_node,cache_node_2, ZENITH_NODE_SIZE);
    /* push it into trhe drive */
    zenith_push(zenith_selected_driver);

  }else{
    /* if there is somethings named in the same way you want to name your directory */
    printf("Error: name already taken");
    return;
  }
  #endif

  return;
}

/* zenith function to remove a directory and return a boolean state abount whats happened */

bool zenith_rmdir(char*path, char*name){

  return false;
}

/* zenith function to move a directory from a destination to another, you can change the name in the process */

void zenith_mv(char*start_path, char*name, char*dest_path, char*dest_name){

  return;
}



