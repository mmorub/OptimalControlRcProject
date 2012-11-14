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


