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

uint32_t* fs_alloc(fs_tab root, int type, char*name){
    int i=0;
    node n;

    while(root.page_pointer[i] != 0xFF){
        i+=1;
    }
    root.allocation_table[i] = 0xFF;
    root.used_space += 1;
    root.free_space -= 1;
    n.adr = root.page_pointer[i];
    strcpy(n.name, name);
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

int get_subdir_num(char*name){
    char str[strlen(name)];
    strcpy(str, name);
    char* token;
    token = strtok(str, "/");
    int i = 0;
    while(token != NULL){
        i+=1;
        token = strtok(NULL, "/");
    }
    return i;
}

node fs_navigate(fs_tab root,char*path){
    node n = get_from_device((uint32_t*)root.first_node);
    int subdir_num = get_subdir_num(path);
    char str[strlen(path)];
    strcpy(str, path);
    char* token;
    bool end = false;
    token = strtok(str, "/");
    int i = 0;
    while(token != NULL){
        for(int i=0;i<296 && !end;i++){
            node ng = get_from_device(n.content[i]);
            if(ng.type == DIR_MASK && strcmp(ng.name, token) == 0 ){
                n = ng;
                i+=1;
                end = true;
            }
        }

        end = false;
        token = strtok(NULL, "/");
    }
    if(subdir_num == i){
        return n;    
    }
    node nz;
    return nz;
}

bool fs_mkdir(fs_tab root,char*path,char*name){
    node ng = fs_navigate(root,path);
    int content = 0;
    int no_occur = 0;
    for(int i=0;i<296;i++){
        if(ng.content[i] != NULL){
            content+=1;
            node mv = get_from_device(ng.content[i]);
            if(strcmp(mv.name, name)) no_occur+=1; 
        }
    }
    if(content == no_occur){
        node * n = (node*)fs_alloc(root,DIR_MASK,name);
        int i = 0;
        while(ng.content[i] != NULL){
            i+=1;
        }
        ng.content[i] = n;
        if(!write_into_device(ng)){
            return false;
        }
        return true;
    }
    return false;
}

bool nest_folder(fs_tab root, char*path, uint32_t *adr){
    node n = fs_navigate(root,path);
    int i = 0;
    while(n.content[i] != NULL){
        i+=1;
    }
    n.content[i] = adr;
    if(!write_into_device(n)){
        return false;
    }
    return true;
}

fs_tab init_fs(char*name, char*version, int size){
    fs_tab root;
    uint32_t * pointer = NULL;
    int j=0;
    if(size != BIG_SIZE || size != MEDIUM_SIZE || size != SMALL_SIZE){
        return root;    
    }
    node * n = (node*)fs_alloc(root,DIR_MASK, "/");
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

    root.first_node = n;
    root.fs_table_byte_space = MAX_DIM_BYTE;

    if(write_fs_tab(root)){
        return root;
    }
}

