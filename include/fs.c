#define FS_C

#include "./fs.h" 

node get_from_device(uint32_t*address){
    node n;
#ifdef VIRTUAL_DISK
    memcpy(n, virtual_path[address], sizeof(n));
#endif
    return n;
}

fs_tab get_fs_tab(){
    fs_tab root;
#ifdef VIRTUAL_DISK
    memcpy(root, virtual_path, sizeof(fs_tab));
#endif
    return root;
}

bool write_fs_tab(fs_tab root){
#ifdef VIRTUAL_DISK
    memcpy(virtual_path, root, sizeof(fs_tab));
    return true;
#endif
}
bool write_into_device(node n){

#ifdef VIRTUAL_DISK
    memcpy(n.adr, n, sizeof(fs_tab));
    return true;
#endif
}


bool fs_free(fs_tab root, uint32_t *adr){

  for(int i=0;i<MAX_PAGE_COUNT;i++){
    if(root.page_pointer[i] == adr && root.allocation_table[i] ==  0xFF){
        root.allocation_table[i] = 0x00;
        root.used_space -= 1;
        root.free_space += 1;
    }
  }
  if(write_fs_tab(root)){
    return true;
  }
  return false;
   
}

uint32_t* fs_alloc(fs_tab root, int type){
    int i=0;
    node n;

    while(root.page_pointer[i] != 0xFF){
        i+=1;
    }
    root.allocation_table[i] = 0xFF;
    root.used_space += 1;
    root.free_space -= 1;
    n.adr = root.page_pointer[i];
    strcpy(n.name, "NewNode");
    n.type = type;
    n.next = NULL;

    for(int i=0;i<296;i++){
        n.content[i] = NULL;
    }

    if(!write_into_device(n)){
        root.allocation_table[i] = 0x00;
        return NULL;
    }

    return root.page_pointer[i];
}

fs_tab init_fs(char*name, char*version, int size){
    fs_tab root;
    uint32_t * pointer = NULL;
    int j=0;
    if(size != BIG_SIZE || size != MEDIUM_SIZE || size != SMALL_SIZE){
        return root;    
    }
#ifdef VIRTUAL_DISK
    virtual_path = (uint32_t*)malloc(sizeof(uint32_t) * size);
#endif
    strcpy(root.name, name);
    strcpy(root.version, version);
    root.total_space = size;
    root.used_space = sizeof(fs_tab) / 512;
    root.free_space = root.total_space - root.used_space;

    for(int i = 0; i < root.total_space;i++){
#ifdef VIRTUAL_DISK
        root.page_pointer[i] = &virtual_path[j];
        j+=512;
#else
        root.page_pointer[i] = pointer;
        pointer+=512;
#endif
    }
    for(int i = 0; i < root.total_space;i++){
        root.allocation_table[i] = 0x00;
    }

    for(int i = 0; i < root.used_space;i++){
        root.allocation_table[i] = 0xFF;
    }

    root.fs_table_byte_space = MAX_DIM_BYTE;

    if(write_fs_tab(root)){
        return root;
    }
}

