#define FS_TALBE_IMPLEMENTATION

#include "./fs_table.h"


extern fs_tab init_fs(char*name, char*version, int size){
    fs_tab root;
    uint32_t * pointer = NULL;
    int j=0;
    if(size != BIG_SIZE && size != MEDIUM_SIZE && size != SMALL_SIZE){
        return root;    
    }

    #ifdef __VIRTUAL_DISK__

    virtual_path = (uint32_t*)malloc(sizeof(uint32_t) * size);

    #endif
    #ifdef __x86_64b_OS__

    #endif
    #ifdef __ARDUINO__

    #endif

    strcpy(root.name, name);
    strcpy(root.version, version);
    root.fs_table_byte_space = size;
    root.total_space = size/512;
    root.used_space = (sizeof(fs_tab) / 512)+1;
    root.free_space = root.total_space - root.used_space;

    for(int i = 0; i < root.total_space;i++){
        #ifdef __VIRTUAL_DISK__

        root.page_pointer[i] = &virtual_path[j];
        j+=512;

        #endif
    }
    for(int i = 0; i < root.total_space;i++){
        root.allocation_table[i] = 0x00;
    }

    for(int i = 0; i < root.used_space;i++){
        root.allocation_table[i] = 0xFF;
    }
    node * n = (node*)fs_alloc(&root,DIR_MASK, "/");
    root.first_node = n;

    if(!write_fs_tab(&root)){
        fs_tab fk_tab;
        return fk_tab;
    }
    return root;
}

extern fs_tab get_fs_tab(){
    fs_tab root;

    #ifdef __VIRTUAL_DISK__

    memcpy(&root, virtual_path, sizeof(fs_tab));

    #endif
    #ifdef __x86_64b_OS__

    #endif
    #ifdef __ARDUINO__

    #endif

    return root;
}


extern void fs_get_info(fs_tab*root){
    printf("===============================\n\n");
    printf("Partition name: %s\n", root->name);
    printf("fs version: %s\n", root->version);
    printf("Partition size ( in byte ): %d\n", root->fs_table_byte_space);
    printf("\n-> free space: %d\n", root->free_space*512);
    printf("-> used space: %d\n\n", root->used_space*512);
    printf("===============================\n");
}


extern node get_from_device(uint32_t*address){
    node n;
    if(!fs_is_alloc(address)){
        strcpy(n.name, "NO_NODE_FOUND");
    }else{
        #ifdef __VIRTUAL_DISK__
                memcpy(&n, address, sizeof(node));
        #endif
        #ifdef __x86_64b_OS__

        #endif
        #ifdef __ARDUINO__

        #endif
    }
    
    return n;
}

extern bool write_fs_tab(fs_tab* root){
    #ifdef __VIRTUAL_DISK__

    memcpy(virtual_path, root, sizeof(fs_tab));

    #endif
    #ifdef __x86_64b_OS__

    #endif
    #ifdef __ARDUINO__

    #endif
    return true;
}


extern bool write_into_device(node n){
    uint32_t *adr = n.adr;
    #ifdef __VIRTUAL_DISK__

    memcpy(adr, &n, sizeof(node));

    #endif
    #ifdef __x86_64b_OS__

    #endif
    #ifdef __ARDUINO__

    #endif
    return true;
}




extern uint32_t* fs_alloc(fs_tab *root, int type, char*name){
    int i=0;
    node n;
    while(root->allocation_table[i] == 0xFF){
        i+=1;
    }
    root->allocation_table[i] = 0xFF;
    root->used_space += 1;
    root->free_space -= 1;
    n.adr = root->page_pointer[i];
    strcpy(n.name, name);
    n.type = type;
    n.next = NULL;
    for(int i=0;i<59;i++){
        n.content[i] = 0x00;
    }
    update_fs_tab(root);
    if(!write_into_device(n)){
        root->allocation_table[i] = 0x00;
        return NULL;
    }
    return root->page_pointer[i];
}


extern bool fs_free(fs_tab* root, uint32_t *adr){

  for(int i=0;i<MAX_PAGE_COUNT;i++){
    if(root->page_pointer[i] == adr && root->allocation_table[i] ==  0xFF){
        root->allocation_table[i] = 0x00;
        root->used_space -= 1;
        root->free_space += 1;
    }
  }
  if(update_fs_tab(root)){
    return true;
  }
  return false;
   
}

bool update_fs_tab(fs_tab*root){
    #ifdef __VIRTUAL_DISK__

    memcpy(virtual_path, root, sizeof(fs_tab));

    #endif
    #ifdef __x86_64b_OS__

    #endif
    #ifdef __ARDUINO__

    #endif
    return true;
}

extern bool fs_is_alloc(uint32_t*address){
    fs_tab root = get_fs_tab();
    bool found = false;
    for(int i=0;i<MAX_PAGE_COUNT;i++){
        if(root.page_pointer[i] == address) found = true;
    }
    return found;
}
