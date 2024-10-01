#define ZTH_DEV_IN_IMPLEMENTATION

#include "./zth_dev_in.h"



extern void zenith_push(uint8_t support_types){
    bool status;
    #ifndef VIRTUAL_DRIVE

    uint8_t adr_lb = cache_node->adr_lb;
    uint8_t adr_hb = cache_node->adr_hb;
    uint8_t adr_xlb = cache_node->adr_xlb;


    for(uint8_t i=0;i<0xff;i++){
        /* 
        
            WARNING: this is an example code, we highly reccommend to link
            and external assembly file with the direct instruction from your
            system to exclude some undesired effects.

            The main goal in this step is to calculate the 24 bit value from
            scratch using the given address and adding 1 to it. 
            To add more than an 8 bit number in an 8 bit architecture can be done
            by some alternative ways.
            An example of code that can add 24 bit number together is the following,
            written in the MOS 6502 Assembly language:


            IN THIS PIECE OF ASSEMBLY CODE THE DECIMAL 10 IS 
            ADDED TO A  24 BIT ADDRESS 0xEAEAEA BY USING THE
            CARRY FLAG FROM THE ALU OPERATION. 
            THE RESOULT FROM THIS OPERATION IS 0xEAEAF4

            BOOT_VECTOR=$fffc   variable vector
            CODE_SEGMENT=$9000  variable vector

            MAIN_VARIABLE=$00   location where there is the non-calculated addres
            RESULT=$10          location where the resoult would be stored


                .org $00        set code segment 
                .word $00   
                .org CODE_SEGMENT


            add_24_bit:         main routine     

                lda #$ea            loading 0xea in the main variable location
                sta MAIN_VARIABLE
                lda #$ea            loading 0xea in the main variable location+1
                sta MAIN_VARIABLE+1
                lda #$ea            loading 0xea in the main variable location+2
                sta MAIN_VARIABLE+2
                jsr add_routine     jump to the addition sub routine
                jmp void_loop       end programm


            add_routine:            addition sub routine

                clc                 clear carry
                lda MAIN_VARIABLE   load data from main variable 
                adc #10             add decimal 10 to it
                sta RESULT          store it in the resoult location
                lda MAIN_VARIABLE+1 load data from main variable+1
                adc #$00            add 0 to it and the carry ( if present ) from the previous operation
                sta RESULT+1        store it in the resoult location+1
                lda MAIN_VARIABLE+2 load data from main variable+2
                adc #$00            add 0 to it and the carry ( if present ) from the previous operation
                sta RESULT+2        store it in the resoult location+1

                rts                 return from subroutine

            void_loop:              end loop

                nop
                jmp void_loop



                .org BOOT_VECTOR   set startup vector
                .word add_24_bit

        */


        uint32_t mem = (adr_xlb << 16) | (adr_hb << 8) | (adr_lb);

        mem += 1;

        adr_lb = (uint8_t)mem;
        adr_hb = (uint8_t)(mem>>8);
        adr_xlb = (uint8_t)(mem>>16);


        switch(support_types){
            case 0:
                memcpy((void*)ADR_LB_PORT, &adr_lb, UINT8_T_SIZE);
                memcpy((void*)ADR_HB_PORT, &adr_hb, UINT8_T_SIZE);
                memcpy((void*)ADR_XLB_PORT, &adr_xlb, UINT8_T_SIZE);

                memcpy((void*)DATA_PORT, &cache_node[i], UINT8_T_SIZE);
                break;

            /* add your case here to add support driver */
            default:
                break;
        }

    }

    #endif

    #ifdef VIRTUAL_DRIVE
      uint32_t offset =  cache_node->adr_lb | cache_node->adr_hb<<8 | cache_node->adr_xlb << 16;
      memcpy(virtual_drive+offset, cache_node, ZENITH_NODE_SIZE);

    #endif
    return;

}

extern void zenith_single_push(uint8_t adr_lb,uint8_t adr_hb,uint8_t adr_xlb ,uint8_t byte, uint8_t support_type){
 
  #ifndef VIRTUAL_DRIVE
  
  /* it matter the same i said before */

  switch(support_type){
    case 0:
        memcpy((void*)ADR_LB_PORT, &adr_lb, UINT8_T_SIZE);
        memcpy((void*)ADR_HB_PORT, &adr_hb, UINT8_T_SIZE);
        memcpy((void*)ADR_XLB_PORT, &adr_xlb, UINT8_T_SIZE);

        memcpy((void*)DATA_PORT,&byte, UINT8_T_SIZE);
        break;
    default:
          break;
  }
  
  #endif

  #ifdef VIRTUAL_DRIVE
    uint32_t offset = adr_lb | adr_hb << 8 | adr_xlb << 16;
    virtual_drive[offset] = byte;
  #endif

  return;
}


extern void zenith_pop(uint8_t l_adr_lb,uint8_t l_adr_hb,uint8_t l_adr_xlb,uint8_t support_types){

    #ifndef VIRTUAL_DRIVE

    uint8_t adr_lb = l_adr_lb;
    uint8_t adr_hb = l_adr_hb;
    uint8_t adr_xlb = l_adr_xlb;
     

    for(uint8_t i=0;i<0xff;i++){
                /* 
        
            WARNING: this is an example code, we highly reccommend to link
            and external assembly file with the direct instruction from your
            system to exclude some undesired effects.

            The main goal in this step is to calculate the 24 bit value from
            scratch using the given address and adding 1 to it. 
            To add more than an 8 bit number in an 8 bit architecture can be done
            by some alternative ways.
            An example of code that can add 24 bit number together is the following,
            written in the MOS 6502 Assembly language:


            IN THIS PIECE OF ASSEMBLY CODE THE DECIMAL 10 IS 
            ADDED TO A  24 BIT ADDRESS 0xEAEAEA BY USING THE
            CARRY FLAG FROM THE ALU OPERATION. 
            THE RESOULT FROM THIS OPERATION IS 0xEAEAF4

            BOOT_VECTOR=$fffc   variable vector
            CODE_SEGMENT=$9000  variable vector

            MAIN_VARIABLE=$00   location where there is the non-calculated addres
            RESULT=$10          location where the resoult would be stored


                .org $00        set code segment 
                .word $00   
                .org CODE_SEGMENT


            add_24_bit:         main routine     

                lda #$ea            loading 0xea in the main variable location
                sta MAIN_VARIABLE
                lda #$ea            loading 0xea in the main variable location+1
                sta MAIN_VARIABLE+1
                lda #$ea            loading 0xea in the main variable location+2
                sta MAIN_VARIABLE+2
                jsr add_routine     jump to the addition sub routine
                jmp void_loop       end programm


            add_routine:            addition sub routine

                clc                 clear carry
                lda MAIN_VARIABLE   load data from main variable 
                adc #10             add decimal 10 to it
                sta RESULT          store it in the resoult location
                lda MAIN_VARIABLE+1 load data from main variable+1
                adc #$00            add 0 to it and the carry ( if present ) from the previous operation
                sta RESULT+1        store it in the resoult location+1
                lda MAIN_VARIABLE+2 load data from main variable+2
                adc #$00            add 0 to it and the carry ( if present ) from the previous operation
                sta RESULT+2        store it in the resoult location+1

                rts                 return from subroutine

            void_loop:              end loop

                nop
                jmp void_loop



                .org BOOT_VECTOR   set startup vector
                .word add_24_bit

        */

        uint32_t mem = (adr_xlb << 16) | (adr_hb << 8) | (adr_lb);

        mem += 1;

        adr_lb = (uint8_t)mem;
        adr_hb = (uint8_t)(mem>>8);
        adr_xlb = (uint8_t)(mem>>16);

        switch(support_types){
            case 0:

                memcpy((void*)ADR_LB_PORT, &adr_lb, UINT8_T_SIZE);
                memcpy((void*)ADR_HB_PORT, &adr_hb, UINT8_T_SIZE);
                memcpy((void*)ADR_XLB_PORT, &adr_xlb, UINT8_T_SIZE);

                memcpy(&cache_node[i], (void*)DATA_PORT, UINT8_T_SIZE);
                break;
            /* add your case here to add support driver */
            default:
                break;
        }

    }

    #endif
    #ifdef VIRTUAL_DRIVE
      uint32_t offset =  l_adr_lb | l_adr_hb << 8 | l_adr_xlb << 16;
      memcpy(cache_node, virtual_drive+offset, ZENITH_NODE_SIZE);

    #endif
    return;
}



extern uint8_t zenith_single_pop(uint8_t adr_lb,uint8_t adr_hb,uint8_t adr_xlb, uint8_t support_types){
  uint8_t byte = 0x00;

  /* it matter the same i said before */

  #ifndef VIRTUAL_DRIVE

  switch(support_types){
    case 0:
        memcpy((void*)ADR_LB_PORT, &adr_lb, UINT8_T_SIZE);
        memcpy((void*)ADR_HB_PORT, &adr_hb, UINT8_T_SIZE);
        memcpy((void*)ADR_XLB_PORT, &adr_xlb, UINT8_T_SIZE);

        memcpy(&byte, (void*)DATA_PORT, UINT8_T_SIZE);
        break;
      /* add your case here to add support driver */
    default:
        break;
  }

  #endif

  #ifdef VIRTUAL_DRIVE
    uint32_t offset = adr_lb | adr_hb << 8 | adr_xlb << 16;
    byte = virtual_drive[offset];  
#endif



  return byte;
}

