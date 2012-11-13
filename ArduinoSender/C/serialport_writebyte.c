#include <stdint.h>   /* Standard types, in particular uint8_t */
#include "serialport_writebyte.h"
int serialport_writebyte(int fd, uint8_t b)
{
    int n = write(fd,&b,1);
    if( n!=1)
        return -1;
    return 0;
}
