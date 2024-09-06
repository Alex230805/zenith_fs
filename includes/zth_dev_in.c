#define ZTH_DEV_IN_IMPLEMENTATION

#include "./zth_dev_in.h"


extern void zenith_push(zenith_general_node *node){
    bool status;
    uint8_t adr_lb = node->adr_lb;
    uint8_t adr_hb = node->adr_hb;
    uint8_t adr_xlb = node->adr_xlb;

    
    #ifndef VIRTUAL_DRIVE

    for(uint8_t i=0;i<0xff;i++){
        /* 
        
            TODO: put a function to increase the 24bit 
            separated address by one 

        */

        memcpy((void*)ADR_LB_PORT, &adr_lb, UINT8_T_SIZE);
        memcpy((void*)ADR_HB_PORT, &adr_hb, UINT8_T_SIZE);
        memcpy((void*)ADR_XLB_PORT, &adr_xlb, UINT8_T_SIZE);

        memcpy((void*)DATA_PORT, &node[i], UINT8_T_SIZE);
    }

    #endif

    #ifdef VIRTUAL_DRIVE
    
    void* address =  (void*)(adr_xlb<<16 | adr_hb<<8 | adr_lb);
    memcpy(address,node,sizeof(zenith_general_node));

    #endif

}



extern zenith_general_node* zenith_pop(uint8_t l_adr_lb,uint8_t l_adr_hb,uint8_t l_adr_xlb){
    zenith_general_node* node = NULL;
    node = (zenith_general_node*)malloc(ZENITH_NODE_SIZE);
    
    uint8_t adr_lb = l_adr_lb;
    uint8_t adr_hb = l_adr_hb;
    uint8_t adr_xlb = l_adr_xlb;

    #ifndef VIRTUAL_DRIVE

    for(uint8_t i=0;i<0xff;i++){
        /* 
        
            TODO: put a function to increase the 24bit 
            separated address by one 

        */

        memcpy((void*)ADR_LB_PORT, &adr_lb, UINT8_T_SIZE);
        memcpy((void*)ADR_HB_PORT, &adr_hb, UINT8_T_SIZE);
        memcpy((void*)ADR_XLB_PORT, &adr_xlb, UINT8_T_SIZE);

        memcpy((void*)&node[i], (void*)DATA_PORT, UINT8_T_SIZE);
    }

    return zenith_general_node;

    #endif

    #ifdef VIRTUAL_DRIVE

    uint32_t* address =  (uint32_t*)(adr_xlb<<16 | adr_hb<<8 | adr_lb);
    memcpy(node, address, ZENITH_NODE_SIZE);

    #endif

    return node;
}
