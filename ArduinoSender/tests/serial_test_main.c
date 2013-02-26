#include <stdio.h> /* Standard input/output definitions */
#include <stdlib.h> /* needed for EXIT_SUCCESS */
#include <stdint.h> /* Standard types, in particular uint8_t */
#include <fcntl.h> /* File control definitions, in particular O_RDWR etc. */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

#include "serialport_init.h" // see Makefile for path to included and linked code
#include "serialport_writebyte.h" // see Makefile for path to included and linked code

/**
* Sends a single byte ("current_signal") every 1/10s to the serial port (defined in variable serialport);
* loops values of byte between bounds set in lower_bound_signal and upper_bound_signal.
*
* Typical pitfalls:
* - make sure serial port selected here (variable "serialport") and on other end are equal
* - make sure baudrate here and on other end are equal
* - make sure receiving end can handle values between lower_bound_signal and upper_bound_signal
*
*/
int main(int argc, char *argv[])
{
    int fd = 0;
    char serialport[]= {"/dev/ttyACM0"};		
    const int baudrate = B9600;
    int rc;
    uint8_t steering_signal= 15;				// steering signal ->	left:0	center:15	right:30
    uint8_t throttle_signal = 30;					// throttle signal ->	full speed:0	center:30	brake:60
    const uint8_t lower_bound_signal= 0;
    const uint8_t upper_bound_signal= 30;
    uint8_t delta_signal= 1;
    fd = serialport_init(serialport, baudrate);			// open handle for serialport
    while (1){
      // send steering signal --> we have to set the MSB to '1'
      steering_signal |= 1<<7;
      rc = serialport_writebyte(fd, steering_signal);
      steering_signal &= ~(1<<7); //set MSB back to '0'
      if(rc==-1) {
    	printf("failed to send byte [steering]");
    	  return -1;
      }
      // send throttle signal
      rc = serialport_writebyte(fd, throttle_signal);
      if(rc==-1) {
    	  printf("failed to send byte [throttle]");
    	  return -1;
      }
      usleep(100000); // sleep 1/10 s

     /* if (steering_signal>= 30){
    	  delta_signal= -1;
      } else if (steering_signal<= 0){
    	  delta_signal= 1;
      }
      steering_signal += delta_signal;*/
    }
    close(fd);				// close handle for serialport
    exit(EXIT_SUCCESS);
}
