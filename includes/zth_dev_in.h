#ifndef ZTH_DEV_IN_H
#define ZTH_DEV_IN_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>


/*  To write and read somethings from the drive indicating
    the dataport and the low byte, the hight byte and 
    the extra low bite that are related to the drive 
    in the address space of the OS.
    it's needed and zenith would not be compiled without it if the
    flag for the virtual drive is not enabled
 */

#ifndef VIRTUAL_DRIVE

#define DATA_PORT 0xeaea /* some address */
#define ADR_LB_PORT 0xeaea /* some address */
#define ADR_HB_PORT 0xeaea /* some address */
#define ADR_XLB_PORT 0xeaea /* some address */


#endif

#include "./zenith.h"


/*  zenith_push() is used as a standard layer to write
    the filesystem changes and new node into the phisical 
    drive.
    It accept an allocated node in ram and write it down into the
    local drive.
    It return a boolean status variable to identify what's appened.

 */

extern void zenith_push(zenith_general_node *node);

/*  zenith_pop() is used as a standard layer to get things from
    the filesystem, it allocate automatically new static node in 
    RAM ready to be read or write, and retur the RAM address of the node. 
    It accept an address directly related to the local drive and get the node
    referred to it.
*/

extern zenith_general_node* zenith_pop(uint8_t adr_lb,uint8_t adr_hb,uint8_t adr_xlb);




#ifndef ZTH_DEV_IN_IMPLEMENTATION
#define ZTH_DEV_IN_IMPLEMENTATION

#endif

#endif