#ifndef ZTH_DIR_H
#define ZTH_DIR_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

#include "./zenith.h"
#include "./zth_dev_in.h"

/* zenith function to create a directory and store the address */

int zenith_mkdir(char*path, char*name);

#ifndef VIRTUAL_DRIVE

int __zenith_mkdir(uint8_t adr_lb,uint8_t adr_hb,uint8_t adr_xlb, char*name);

#endif

/* zenith function to remove a directory and return a boolean state abount whats happened */

int zenith_rmdir(char*path, char*name);

/* zenith_rmdir sub-function to make it recursive and capable of using the extension address feature */

#ifndef VIRTUAL_DRIVE

int __zenith_rmdir(uint8_t adr_lb, uint8_t adr_hb, uint8_t adr_xlb, char*name);

#endif

/* zenith function to move a directory from a destination to another, you can change the name in the process */

void zenith_mv(char*start_path, char*name, char*dest_path, char*dest_name);

#ifndef ZTH_DIR_IMPLEMENTATION
#define ZTH_DIR_IMPLEMENTATION

#endif

#endif
