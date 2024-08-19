#define FS_DIR_IMPLEMENTATION

#include "./fs_dir.h"

extern node fs_navigate(fs_tab* root,char*path){
    node n = get_from_device((uint32_t*)root->first_node);
    int subdir_num = get_subdir_num(path);
    int str_len = strlen(path);
    char str[str_len];
    strcpy(str, path);
    char* token;
    bool end = false;
    token = strtok(str, "/");
    int z = 0;
    while(token != NULL){
        for(int i=0;i<59 && !end;i++){
            if(n.content[i] != NULL){
                node ng = get_from_device(n.content[i]);
                if(ng.type == DIR_MASK && strcmp(ng.name, token) == 0 ){
                    n = ng;
                    z+=1;
                    end = true;
                } 
            }
            
        }

        end = false;
        token = strtok(NULL, "/");
    }
    if(subdir_num == z){
        return n;    
    }
    node nz;
    strcpy(nz.name, "NO_NODE_FOUND");
    return nz;
}

extern int get_subdir_num(char*name){
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

extern bool nest_folder(fs_tab* root, char*path, uint32_t *adr){
    node n = fs_navigate(root,path);
    if(strcmp(n.name, "NO_NODE_FOUND") == 0){
        return false;
    }
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


extern bool fs_mkdir(fs_tab* root,char*path,char*name){
    uint32_t * new_page = NULL;
    node ng = fs_navigate(root,path); 
    if(strcmp(ng.name, "NO_NODE_FOUND") == 0){
        return false;
    }
    while(1){
        if(!fs_is_present(ng,name)){
            uint32_t * n = fs_alloc(root,DIR_MASK,name);
            int i = 0;
            while(ng.content[i] != NULL){
                i+=1;
            }
            if(i < 59){
                ng.content[i] = n;
                if(!write_into_device(ng)) return false;
                return true;
            }
        }
        if(ng.next == NULL){
            uint32_t *n = fs_alloc(root,DIR_MASK,"__CONTIGUOUS__");
            ng.next = n;
            if(!write_into_device(ng)) return false;
        }
        ng = get_from_device(ng.next);
    }
   return false;
}


extern bool fs_rmdir(fs_tab*root, char*path, char*name){
    node new_page;
    int occupated = 0;
    node ng = fs_navigate(root,path);
    bool end = false;
    if(strcmp(ng.name, "NO_NODE_FOUND") == 0){
        return false;
    }
    while(1){
        if(fs_is_present(ng,name)){
            int pos = fs_is_present_pos(ng, name);
            if(!fs_have_subdir(ng.content[pos])){
                fs_free(root,ng.content[pos]);
                ng.content[pos] = NULL;
                if(!write_into_device(ng)) return false;
                return true;
            }
        }
        if(ng.next != NULL){
            ng = get_from_device(ng.next);
        }else{
            return false;
        }

    }
    return false;

}

extern bool fs_mv(fs_tab*root,char*path,char*name, char*dest){
    uint32_t *adr = NULL;
    bool end = false;
    node ng = fs_navigate(root,path);
    if(strcmp(ng.name, "NO_NODE_FOUND") == 0){
        return false;
    }
    int pos = fs_is_present_pos(ng,name);
    node mv = get_from_device(ng.content[pos]);
    adr = mv.adr;
    ng.content[pos] = NULL;
    write_into_device(ng);
    ng = fs_navigate(root,dest);
    if(strcmp(ng.name, "NO_NODE_FOUND") == 0){
        return false;
    }
    end = false;
    for(int i = 0; i<59 && !end; i++){
        if(ng.content[i] == NULL){
            ng.content[i] = adr;
        }
    }
    if(write_into_device(ng)) return true;
    return false;
}

extern void fs_get_dir_content(fs_tab*root, char*path){
    node n = fs_navigate(root,path);
    if(strcmp(n.name, "NO_NODE_FOUND") == 0){
        return;
    }
    printf("content of %s :\n", n.name);
    while(1){
        for(int i=0;i<59;i++){
            if(n.content[i] != NULL){
                node lg = get_from_device(n.content[i]);
                printf("- %s - %d\n", lg.name, lg.type);
            }
        }
        if(n.next != NULL){
            n = get_from_device(n.next);
        }else{
            return;
        }
    }
    
}

extern bool fs_have_subdir(uint32_t*address){
    node ng = get_from_device(address);
    if(strcmp(ng.name,"NO_NODE_FOUND") == 0){
        return true;
    }
    while(1){
        for(int i=0;i<59;i++){
            if(ng.content[i] != NULL){
                return true;
            }        
        }
        if(ng.next != NULL){
            ng = get_from_device(ng.next);
        }else{
            return false;
        }
    }
    return true;
}


extern bool fs_is_present(node ng, char*name){
    int no_occur = 0;
    int content = 0;
    for(int i=0;i<59;i++){
        if(ng.content[i] != NULL){
            content+=1;
            node mv = get_from_device(ng.content[i]);
            if(strcmp(mv.name, name) != 0) no_occur+=1; 
        }
    }
    if(content == no_occur){
        return false;
    }
    return true;
}

extern int fs_is_present_pos(node ng, char*name){
    int pos = 0;
    for(int i=0;i<59;i++){
        if(ng.content[i] != NULL){
            node mv = get_from_device(ng.content[i]);
            if(strcmp(mv.name, name) == 0){
              pos = i;  
            } 
        }
    } 
    return pos;    
}
