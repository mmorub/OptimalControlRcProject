const int input_pin= 7;
const int output_pin= 8; 
const char* interrupt_frequency= "100"; // kHz

/**
 * Defines steering signal data.
 */
const int length_steering_signal= 6;
long int steering_signal_data[6]= {
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
 * Defines binding signal data; sent at start-up to bind 2.4GHz sender (Kopropo RF-901SM) to car.
 */
const int length_binding_signal= 65; 
const long int binding_signal_data[length_binding_signal]= { 
  0, 67183,                    // indices to these data: 0, 1 
  57, 30, 1555,                //  2.. 4
  30, 120, 30, 120, 30, 1164,  //  5..10 
  30, 120, 30, 120, 30, 1220,  
  30, 120, 30, 120, 30, 1220,  
  30, 120, 30, 120, 30, 1220,  
  30, 120, 30, 120, 30, 1220,  
  30, 120, 30, 120, 30, 1220,  
  30, 120, 30, 120, 30, 1220,  
  30, 120, 30, 120, 30, 1220,  
  30, 120, 30, 120, 30, 1220,  
  30, 120, 30, 120, 30, 1220}; // 59..64

boolean sender_switched_on= 0; 
boolean binding_initiated= 0; 
boolean send_steering_signal= 0;
boolean send_binding_signal= 0; 
boolean next_signal= 0; 
unsigned long int interrupt_counter= 0;
unsigned int signal_counter= 0; 

void setup(){

  pinMode(input_pin, INPUT);
  pinMode(output_pin, OUTPUT); 
  Serial.begin(9600); 

  if(interrupt_frequency== "8"){
    setup_interrupt_8kHz(); 
  } 
  else if(interrupt_frequency== "80"){
    setup_interrupt_80kHz();
  } 
  else if(interrupt_frequency== "100"){
    setup_interrupt_100kHz(); 
  } 
  else if(interrupt_frequency== "125"){
    setup_interrupt_125kHz(); 
  } 
  else {
    Serial.println("FAILED TO SET INTERRUPT FREQUENCY");
  }

  Serial.println("starting over"); 
}

inline void set_channel_one(int signal){
  steering_signal_data[1]= signal;
}

inline int get_channel_one(){
  return steering_signal_data[1];
}

void loop(){
/* 
 * debugging: send four bits that control interrupt handler to screen
 *
  send_steering_signal ? Serial.print("1") : Serial.print("0");
  send_binding_signal ? Serial.print(" 1") : Serial.print(" 0");
  binding_initiated ? Serial.print(" 1") : Serial.print(" 0");
  sender_switched_on ? Serial.println(" 1") : Serial.println(" 0");
*/

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
 
 if (Serial.available()> 0) {
    int usersInteger= Serial.read();
    Serial.print("user requested duration= ");
    Serial.println(usersInteger, DEC); 

    if(usersInteger<= upper_bound_channel_one && usersInteger>= lower_bound_channel_one){
      send_steering_signal= 0; 
      steering_signal_data[1]= usersInteger;
      send_steering_signal= 1; 
    } 
  }

}

/**
 * If-else construction is designed to 
 * - decide with highest priority whether steering signal is to be sent (one boolean comparison required)
 * - decide with second highest priority whether binding signal is to be sent (two comparisons required)
 * - put low priority on waiting for sender to be switched on for the first time (three comparisons required)
 */
ISR(TIMER2_COMPA_vect){
  if(send_steering_signal){
    adjust_steering_signal(); 
  } 
  else if(send_binding_signal){ // equiv. to !send_steering_signal && send_binding_signal
    // carry on binding
    adjust_binding_signal(); 
  }
  else {                        // equiv. to !send_steering_signal && !send_binding_signal 
  
    // do nothing if binding has been finished
    // otherwise, wait for sender to be switched on and trigger binding 
    if(!binding_initiated){     // equiv. to !send_steering_signal && !send_binding_signal && !binding_initiated   
    
      // wait for sender to be switched on for the first time
      if(!sender_switched_on){  // equiv. to !send_steering_signal && !send_binding_signal && !binding_initiated && !sender_switched on
        sender_switched_on= digitalRead(input_pin);  // check if sender has been switched on
      } else {
        send_binding_signal= 1;
        binding_initiated= 1;  
      }
    } else {                    // equiv. to !send_steering_signal && !send_binding_signal && sender_switched_on 
      // do nothing, this case is reached if binding has been completed and loop() sets send_binding_signal to false
    }
  }

}

inline void adjust_binding_signal(){
  
  // toggle output signal if it is time for next rising or falling edge
  if(interrupt_counter== binding_signal_data[signal_counter]){ // check if it is time for next edge
    digitalWrite(output_pin, next_signal); 
    signal_counter++; 
    interrupt_counter= 0; // reset to start counting to next toggle time
    next_signal= !next_signal; // toggle signal for next use
  } else {
    interrupt_counter++; 
  }
  
  // check if binding signal has been completed
  // if so, switch from binding to steering
  if(signal_counter== length_binding_signal) {
    send_steering_signal= 1;
    send_binding_signal= 0;
    signal_counter= 0;
    interrupt_counter= 0; 
  } 
}

inline void adjust_steering_signal(){
  
  // toggle output signal if it is time for next rising or falling edge
  if(interrupt_counter== steering_signal_data[signal_counter]){ // check if it is time for next edge
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
 * Set timer 2 for interrupts at 125kHz.  
 */
void setup_interrupt_125kHz(){
  cli(); // stop interrupts

  TCCR2A = 0; // set TCCR2A register to 0
  TCCR2B = 0; // set TCCR2B register to 0 
  TCNT2  = 0; // initialize counter value to 0

  // set interrupt interrupt_frequency to 125kHz by setting
  // prescaler to 8 and counter to 15 for
  // 16MHz/prescaler/(counter+ 1)= 16MHz/8/16= 125kHz
  OCR2A = 15; 
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // set prescaler to 8
  TCCR2B |= (1<< CS11); 
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);

  sei(); // enable interrupts
}

/**
 * Set timer 2 for interrupts at 100kHz.  
 */
void setup_interrupt_100kHz(){
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

/**
 * Set timer 2 for interrupts at 80kHz.   
 */
void setup_interrupt_80kHz(){
  cli(); // stop interrupts

  TCCR2A = 0; // set TCCR2A register to 0
  TCCR2B = 0; // set TCCR2B register to 0 
  TCNT2  = 0; // initialize counter value to 0

  // set interrupt_frequency to 80kHz by setting
  // prescaler to 8 and counter to 24 for
  // 16MHz/prescaler/(counter+ 1)= 16MHz/8/25= 80kHz
  OCR2A = 24; 
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // set prescaler to 8
  TCCR2B |= (1<< CS11); 
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);

  // set prescaler to 64
  //TCCR2B |= (1 << CS10) | (1 << CS11);   

  sei(); //allow interrupts
}

/**
 * Set timer 2 for interrupts at 8kHz.   
 */
void setup_interrupt_8kHz(){
  cli(); // stop interrupts

  TCCR2A = 0; // set TCCR2A register to 0
  TCCR2B = 0; // set TCCR2B register to 0 
  TCNT2  = 0; // initialize counter value to 0

  // set interrupt_frequency to 8kHz by setting
  // prescaler to 8 and counter to 249 for
  // 16MHz/prescaler/(counter+ 1)= 16MHz/8/250= 8kHz
  OCR2A = 249; 
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // set prescaler to 8
  TCCR2B |= (1<< CS11); 
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);

  // set prescaler to 64
  //TCCR2B |= (1 << CS10) | (1 << CS11);   

  sei(); //allow interrupts
}

