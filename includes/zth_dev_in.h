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


 /*  virtual drive precompiler directive to enable a virtual drive 
    enviorment to test the filesystem in the local ram.
    to enable ONLY the dev enviorment you need to uncomment the 
    definition down here
*/


/*  Other support will require a separate implementation due to
    the nature of the filesystem and the unknown integration with the OS.
    So by default the first implementation would be a direct connection with
    the support, with some sort of access to the address and the data port of the
    drive. An example of the perfect use case for that is if in the system is 
    present a CF Card support. A CF Card require a paralle interaction that can be
    provided by the system by simply dedicate a piece of address space of the cpu 
    to allocate the address port ( in that case 24 for a max o 16mb ) and the data 
    port ( 8bit ).

    To implement other support is simple, you just need the driver from the system
    provided by the kernel and code a custom implementation similar to the default one.

    With that system is possible to integrate a SPI or an I2S support, or even other serial
    protocols by simply adding the dedicated driver.
    For a better usage it's possible to exclude even the default implementation if the
    specs from your system are differents, for general purpose and to cover the simplest 
    type of support for most parallel devices ( like ROMs or CF cards ) are present by default.

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
    It accept the type of driver and use the static node variable in ram to write down things.
    It return a boolean status variable to identify what's appened.

 */

extern void zenith_push(uint8_t support_type);

/*  zenith_pop() is used as a standard layer to get things from
    the filesystem,  it use the static node variable in RAM to read and save things.
    It accept an address directly related to the local drive and get the node
    referred to it and the type of driver.
*/

extern void zenith_pop(uint8_t adr_lb,uint8_t adr_hb,uint8_t adr_xlb,uint8_t support_type);



/*
  zenith push like function but it write a single byte.
  it read the address from the cache and the byte 
  passed by argument and write it down to the drive

 */


extern void zenith_single_push(uint8_t adr_lb,uint8_t adr_hb,uint8_t adr_xlb, uint8_t support_type, uint8_t byte);


/*
  zenith pop like function but it return a single byte.
  it read the address from the cache and return the data
  from there.

 */

extern uint8_t zenith_single_pop(uint8_t adr_lb,uint8_t adr_hb,uint8_t adr_xlb,uint8_t support_types);



#ifndef ZTH_DEV_IN_IMPLEMENTATION
#define ZTH_DEV_IN_IMPLEMENTATION

#endif

#endif
