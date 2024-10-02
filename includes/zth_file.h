#ifndef ZTH_FILE_H
#define ZTH_FILE_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

#include "./zenith.h"
#include "./zth_dev_in.h"
#include "./zth_static.h"

#ifdef VIRTUAL_DRIVE

uint8_t* virtual_drive;

#endif

uint8_t cache_adr_lb;
uint8_t cache_adr_hb;
uint8_t cache_adr_xlb;

uint8_t cache_adr_lb_2;
uint8_t cache_adr_hb_2;
uint8_t cache_adr_xlb_2;

zenith_general_node* cache_node;
zenith_general_node* cache_node_2;

zenith_general_node* zenith_root_node;

uint8_t zenith_selected_driver;

#ifndef ZTH_FILE_IMPLEMENTATION
#define ZTH_FILE_IMPLEMENTATION

#endif

#endif
