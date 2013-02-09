/**
***********************************************************************************
*
* Code reads bytes from serial port and sends signal that corresponds to these
* bytes to the rc toy car sending unit (RF901SM); use this code with 
* ./tests/loop_steering_angle.c to test the connection.
*
***********************************************************************************
*/

/**
 * Define steering signal data;
 * signal is composed of low signals separated by 30 microsend high signals;
 * duration of low signal determines value of the respective channel;
 * signal of this type is referred to as "PPM signal".
 */
const int length_steering_signal= 6; // length in bytes
volatile long int steering_signal_data[6]= {
  30,    // index 0, duration of first high that announces channel 1 signal  
  120,   // index 1, duration of channel 1 signal (steering wheel)
  30,    // index 2, announces channel 2 signal
  120,   // index 3, channel 2 signal (throttle/brake)
  30,    // index 4, announces channel 3 signal/marks end of channel 2 signal 
  1220}; // index 6, time to be filled before next PPM signal starts 
const int lower_bound_channel_one= 100; // lower bound on signal length
const int upper_bound_channel_one= 125; 
const int lower_bound_channel_two= 110; 
const int upper_bound_channel_two= 130; 

/**
 * pins and boolean variables
 */
const int input_pin= 7;  // arduino detects 12V power source to be on with pin 7
const int output_pin= 8; // arduino sends PPM signal through pin 8
boolean sender_switched_on= 0; 
boolean next_signal= 0; 
unsigned long int interrupt_counter= 0;
unsigned int signal_counter= 0; 

/**
 * Sets up arduino at upload of code.
 */
void setup(){

  // setup input and output pins
  pinMode(input_pin, INPUT);
  pinMode(output_pin, OUTPUT); 
  digitalWrite(output_pin, 1); // default is high

  // when serial port is initiated by the control program, arduino resets, so wait for a few seconds
  delay(2000);
  
  // setup serial interface 
  Serial.begin(9600); 

  // setup interrupt, interrupt handler is used to construct reliable PPM signal 
  setup_interrupt2_100kHz(); 
}

/**
 * Sets channel one of the PPM signal; 
 * used for convenience and easier reading only.
 */
inline void set_channel_one(int signal){
  steering_signal_data[1]= signal;
}

/**
 * Returns current value of channel one of the PPM signal; 
 * used for conveniece and easier reading only. 
 */
inline int get_channel_one(){
  return steering_signal_data[1];
}

/**
 * Loop is carried out periodically as long as arduino is powered 
 * (approximately at 600Hz, but without any real time reliability).
 */
void loop(){
 
 // If a byte is available at the serial port, interpret it to be 
 // the requested channel one value.
 if (Serial.available()> 0) {
    int usersInteger= Serial.read();
    // Uncomment the following 2 lines for debugging
    //Serial.print("user requested duration= ");
    //Serial.println(usersInteger, DEC); 

    // Accept byte from serial port only if its value is within bounds
    // specified by the user. 
    if(usersInteger<= upper_bound_channel_one && usersInteger>= lower_bound_channel_one){
      cli(); // disable interrupts while writing to steering_signal_data[]
      steering_signal_data[1]= usersInteger;
      sei(); // enable interrupts
    } 
  }

/**
 * debugging: send periodic signal to steering wheel
 *
  int step= 1; 
  if (send_steering_signal) {
      send_steering_signal= 0; 
      steering_signal_data[1]+= step;
      send_steering_signal= 1; 
    if(steering_signal_data[1]> upper_bound_channel_one){
      step= -1;
      delay(2000);
    } 
    else if(steering_signal_data[1]< lower_bound_channel_one){
      step= 1; 
      delay(2000); 
    }
    Serial.println(steering_signal_data[1]); 
  }
  delay(20);
 */

}

/**
 * Interrupt handler for timer 2 carries out one of three tasks (or does nothing):
 * step 1: wait for 12V power supply to be switched on (look for if(!sender_switched_on))
 * step 2: send binding signal (look for if(send_binding_signal))
 * step 3: send current PPM signal that determines steering wheel and throttle of toy rc car
 * (look for if(send_steering_signal)).
 */
ISR(TIMER2_COMPA_vect){
  adjust_steering_signal(); 
}


/**
 * Adjusts steering signal as a function of time, where time is measured
 * by counting interrupts.
 */
inline void adjust_steering_signal(){
  
  // toggle output signal if it is time for next rising or falling edge
  if(interrupt_counter >= steering_signal_data[signal_counter]){ // check if it is time for next edge
    digitalWrite(output_pin, next_signal); 
    signal_counter++; 
    interrupt_counter= 0; // reset to start counting to next toggle time
    next_signal= !next_signal; // toggle signal for next use
  } else {
    interrupt_counter++; 
  }
  
  // check if steering signal has been completed
  // if so, start over
  if(signal_counter== length_steering_signal) {
    signal_counter= 0;
    interrupt_counter= 0; 
  } 
}

/**
 * Set timer 2 for interrupts at 100kHz.  
 */
void setup_interrupt2_100kHz(){
  cli(); // stop interrupts

  TCCR2A = 0; // set TCCR2A register to 0
  TCCR2B = 0; // set TCCR2B register to 0 
  TCNT2  = 0; // initialize counter value to 0

  // set interrupt_frequency to 100kHz by setting
  // prescaler to 8 and counter to 19 for
  // 16MHz/prescaler/(counter+ 1)= 16MHz/8/20= 100kHz
  OCR2A = 19; 
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // set prescaler to 8
  TCCR2B |= (1<< CS11); 
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);

  sei(); // enable interrupts
}



