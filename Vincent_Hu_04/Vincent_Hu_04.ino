/* Vincent Hu */

int potPin = 0; // pot connections: 1 = 5V, 3= GND, 2=A0

volatile int potVal; // volatile declaration prevents removal by optimizing compiler

int ADC8(int pin)
{
  ADMUX = pin;        // Set mux to pin number
  ADMUX |= (1 << REFS0); // Set internal reference voltage (5V)
  
  ADCSRA |= 1 << ADEN; // Enable analog-digital conversion  
  ADCSRA |= 7;         // Set ADPS to 10bit precision
  ADCSRA |= 1 << ADSC; // ADC start conversion
  
  while(bit_is_set(ADCSRA, ADSC)); // Wait for conversion...
  
  uint8_t low, high;
  low = ADCL;
  high = ADCH;
  
  // Combine and return the two bytes
  return ((high << 8) | low)/4;
}

void ADC_display(int n)

{

  int i;

  Serial.print("|"); // print baseline

  for(i=0; i<n/4; i++) Serial.print(" "); // div by 4 sets resolution (take in

  // 7:0 bits, but only want 2^6=64, dividing by 4 gets 5:0 resolution)

  Serial.println("*"); // print marker

}

void setup()

{

  Serial.begin(115200); // set Monitor baud rate

  noInterrupts(); // disable all interrupts

    TCCR1A = 0; // clear Timer1 control registers

  TCCR1B = 0;

  TCCR1B |= (1 << WGM12); // CTC mode (Clear Timer on Compare)

  TCCR1B |= (1 << CS12); // 256 prescaler

  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt

  OCR1A = 3125; // compare match register 16MHz/prescale 3125/(16MHz/256) (50ms)

  TCNT1 = 0; // clear counter

  interrupts(); // enable all interrupts 

}

ISR(TIMER1_COMPA_vect) // timer compare interrupt service routine

{

  potVal = ADC8(potPin); // read ADC into global variable

}

void loop()

{

  ADC_display(potVal); // display current pot value

  delay(50); // delay 50 ms

}

