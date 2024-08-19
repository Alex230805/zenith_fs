#define FS_FILE_IMPLEMENTATION

#include "./fs_file.h"


extern uint32_t* fs_is_file_present(fs_tab*root,node*n, char*filename){

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
    node n, file;
    node* moving_node;
    node* file_ptr;
    bool end = false;

    n = fs_navigate(root, path);
    if(strcmp(n.name, "NO_NODE_FOUND") == 0 || __current_file_open__ != NULL){
        return ptr;
    }

    /* check for file 
    
    
    
    
    
    
    */


    /* if the file does not exist */
    if(!end){
        file_ptr = (node*)fs_alloc(root, DATA_MASK, filename);
        memcpy(&file, file_ptr, sizeof(node));
        write_into_device(file);

        if(nest_folder(root, path, file_ptr->adr)){
            __current_file_open__ = file_ptr;
            ptr = (char*)malloc(sizeof(char)*4096);
        }
    }   /* if exist */
    else{
        __current_file_open__ = file_ptr;
        ptr = (char*)malloc(sizeof(char)*4096);
        memcpy(moving_node, __current_file_open__ , sizeof(node));
        fs_file_ram_copy(root, moving_node, ptr, 0);
    }
    return ptr;
}

extern void fs_fwrite(fs_tab root, uint32_t* file_pointer,char* path, char* filename){

}
