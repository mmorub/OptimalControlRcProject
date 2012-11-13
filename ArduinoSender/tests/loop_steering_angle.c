
#include <stdio.h>    /* Standard input/output definitions */
#include <stdlib.h>   /* needed for EXIT_SUCCESS */
#include <stdint.h>   /* Standard types, in particular uint8_t */
#include <fcntl.h>    /* File control definitions, in particular O_RDWR etc. */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */

#include "serialport_init.h"      // see Makefile for path to included and linked code
#include "serialport_writebyte.h" // see Makefile for path to included and linked code

/**
 * Sends a single byte ("current_signal") every 1/10s to the serial port (defined in variable serialport);
 * loops values of byte between bounds set in lower_bound_signal and upper_bound_signal.
 * 
 * Typical pitfalls:
 * - make sure serial port selected here (varialbe "serialport") and on other end are equal
 * - make sure baudrate here and on other end are equal
 * - make sure receiving end can handle values between lower_bound_signal and upper_bound_signal
 * 
 */
int main(int argc, char *argv[]) 
{
    int fd = 0;
    char serialport[]= {"/dev/ttyACM3"};  
    const int baudrate = B9600;  
    int rc; 
    uint8_t current_signal= 120;
    const uint8_t lower_bound_signal= 100;
    const uint8_t upper_bound_signal= 125;
    uint8_t delta_signal= 1; 
    fd = serialport_init(serialport, baudrate);
    while (1){
      rc = serialport_writebyte(fd, current_signal);
      if(rc==-1) return -1;
      usleep(100000); // sleep 1/10 s
      
      if (current_signal>= 125){
	delta_signal= -1;
      } else if (current_signal<= 100){
	delta_signal= 1;
      }
      current_signal += delta_signal;  
    }
    exit(EXIT_SUCCESS);    
} 
    
