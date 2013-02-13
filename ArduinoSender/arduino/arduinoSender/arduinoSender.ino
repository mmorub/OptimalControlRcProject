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
 * Defines for better readability 
 */
 
 #define STEERING 1
 #define THROTTLE 3
 #define OFFSET_STEERING 100
 #define OFFSET_THROTTLE 90
 
 /**
 * Define steering signal data;
 * signal is composed of low signals separated by 30 microsend high signals;
 * duration of low signal determines value of the respective channel;
 * signal of this type is referred to as "PPM signal".
 *
 * Note last value, i.e. time to be filled before next PPM signal starts,
 * is about 450 microseconds for Kopropo EX-1 Mars gun set for two channels.
 * For last entry= 670 a total duration of 1000 microseconds, i.e. 100Hz, results
 * for the 2-channel PPM signal. This is twice as fast as the typical 
 * 50Hz of toy rc controls.  
 */
const int length_steering_signal= 6; // length in bytes
volatile long int steering_signal_data[6]= {
  30,    // index 0, duration of first high that announces channel 1 signal  
  115,   // index 1, duration of channel 1 signal (steering wheel)
  30,    // index 2, announces channel 2 signal
  120,   // index 3, channel 2 signal (throttle/brake)
  30,    // index 4, announces channel 3 signal/marks end of channel 2 signal 
  675};  // index 5, time to be filled before next PPM signal starts
 
/**
 * Following lower and upper bounds for channel durations need to be adjusted for each model 
 * by trial and error.
 */
const int lower_bound_channel_one= 0; // lower bound on signal length (steering)
const int upper_bound_channel_one= 30; 
const int lower_bound_channel_two= 0; //  "        "                  (throttle)
const int upper_bound_channel_two= 60; 

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
 * used for debugging only
 */
int step= 1; 
const int debug_pin= 12;

/**
 * Sets up arduino at upload of code.
 */
void setup(){

  // setup input and output pins
  pinMode(output_pin, OUTPUT); 
  pinMode(debug_pin, OUTPUT);
  
  digitalWrite(output_pin, 1); // default is high
  digitalWrite(debug_pin, 1);

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
    
    // check if MFB is set => yes => steering signal; else throttle signal
    if (usersInteger & 0b10000000) {    // steering
        usersInteger &= ~(1 << 7);    //set MSB to 0 again
        // Accept byte from serial port only if its value is within bounds
        // specified by the user. 
        if(usersInteger<= upper_bound_channel_one && usersInteger>= lower_bound_channel_one){
            //digitalWrite(debug_pin, 0);
            cli(); // disable interrupts while writing to steering_signal_data[]
            steering_signal_data[STEERING] = usersInteger + OFFSET_STEERING;
            // Serial.println(usersInteger, DEC); 
            sei(); // enable interrupts
        }
    }
    else {          // throttle
        if(usersInteger<= upper_bound_channel_two && usersInteger>= lower_bound_channel_two){
            //digitalWrite(debug_pin, 1);
            cli(); 
            steering_signal_data[THROTTLE] = usersInteger + OFFSET_THROTTLE;
            // Serial.println(usersInteger, DEC); 
            sei(); 
        }
    }
    
    // Uncomment the following 2 lines for debugging
    //Serial.print("user requested duration= ");
    //Serial.println(usersInteger, DEC); 
}

/**
 * debugging: send periodic signal to steering wheel
 *
  cli(); // disable interrupts while writing to steering_signal_data
  steering_signal_data[1]+= step;
  sei(); // enable interrupts
  
  // switch step to -1 if upper bound is reached
  // switch step to  1 if lower bound is reached
  if(steering_signal_data[1]> upper_bound_channel_one){
    step= -1;
    delay(2000);
  } 
  else if(steering_signal_data[1]< lower_bound_channel_one){
    step= 1; 
    delay(2000); 
  }
  Serial.println(steering_signal_data[1]); 
  
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
  if(signal_counter>= length_steering_signal) {
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



