#define FS_FILE_IMPLEMENTATION

#include "./fs_file.h"


extern uint32_t* fs_is_file_present(fs_tab*root,node*n, char*filename){
    int i=0;
    bool end = false;
    node *copy = n;
    node* mn = NULL;

    while(end != true){
        for(i=0;i<59;i++){
            if(copy->content[i] != NULL){
                mn = (node*)copy->content[i];
                if((strcmp(filename, mn->name) == 0) && (mn->type == DATA_MASK)){
                    printf("file trovato");
                    end = true;
                }
            }
        }
        if(!end){
            if(copy->next != NULL){
                copy = (node*)copy->next;
                if(strcmp(copy->name, "__CONTIGUOUS__") != 0){
                    return NULL;
                   
                }
            }else{
                return NULL;
            }
        }
    }
    return (uint32_t*)mn;
}

extern void fs_file_ram_copy(fs_tab*root, node* n,char*ptr, int start_point){
    int i=0;
    bool end=false;
    if(__current_file_open__ != NULL) return;

    for(i=0;i<59 && !end;i++){
        if(n->content[i+start_point] != NULL){
            memcpy(&ptr[i+start_point], n->content[i+start_point], sizeof(char));
            if(ptr[i+start_point] == EOF){
                end=true;  
            }
        }
    }
    if(n->next != NULL && !end){
        memcpy(n,n->next, sizeof(node));
        if(strcmp(n->name, "__CONTIGUOUS__") == 0){
            fs_file_ram_copy(root,n,ptr, i);
        }
    }else{
        return;
    }
}


extern char* fs_fopen(fs_tab* root, char* path, char* filename){
    char* ptr = NULL;
    node n;
    node* file_ptr;
    bool end = false;

    n = fs_navigate(root, path);
    if(strcmp(n.name, "NO_NODE_FOUND") == 0 || __current_file_open__ != NULL){
        return ptr;
    }
    /* check for presence */
    file_ptr = (node*)fs_is_file_present(root,&n,filename);

    if(file_ptr == NULL){
        file_ptr = (node*)fs_alloc(root, DATA_MASK, filename);
        if(nest_folder(root, path, file_ptr->adr)){
            __current_file_open__ = file_ptr;
            ptr = (char*)malloc(sizeof(char)*BUFFER_LIMIT);
        }
    } 
    else{
        __current_file_open__ = file_ptr;
        ptr = (char*)malloc(sizeof(char)*BUFFER_LIMIT);
        fs_file_ram_copy(root, __current_file_open__ , ptr, 0);
    }
    write_fs_tab(root);
    return ptr;

}

extern void fs_fwrite(fs_tab* root, uint32_t* file_pointer){
    int size=0;
    int i = 0;
    bool end = false;
    while(size<BUFFER_LIMIT){
        if(strcmp((char*)&file_pointer[size], "\0") == 0){
            return;
        }
        size++;
    }
    
    while(end != true){
        for(i=0;i<59;i++){
            if(strcmp((char*)&file_pointer[size], "\0") != 0){
                __current_file_open__->content[i] = &file_pointer[i];
            }else{ 
                end = true;
            }
        }
        if(end == true){
            return;
        }
        if(i==58){
            if(__current_file_open__->next!=NULL){
                __current_file_open__ = (node*)__current_file_open__->next;
                if(strcmp(__current_file_open__->name,"__CONTIGUOUS__") != 0){
                    __current_file_open__->next = fs_alloc(root, DATA_MASK, "__CONTIGUOUS__");
                    write_fs_tab(root);
                    __current_file_open__ = (node*)__current_file_open__->next;
                }
            }
        }
    }

    __current_file_open__ = NULL;

    return;
}
