/* Vincent Hu */

#define SOUND 343  //speed of SOUND
#define TRIG 3     //trigger pin

volatile int cnt = 0;
int tof;

void triggerPulse()
{
  TCNT2 = 0;
  digitalWrite(TRIG, HIGH); //transmit pulse
  delay(2);
  digitalWrite(TRIG, LOW); //end pulse
  
  TCNT2 = 0; //set timer to 0 at the downward transition of the Trig pulse
  delay(100);
}

void setup() {
  Serial.begin(9600);
  
  /*Configure pins*/
  pinMode(TRIG, OUTPUT);//set the trigger pin as an output
  digitalWrite(TRIG, LOW); //set the tirgger pin low
  
  /*Configure external interrupt*/
  noInterrupts(); //turn off interrupts during config
  TCCR2A = 0; // clear Timer1
  TCCR2B = 0; // set to counter mode
  
  TCCR2B |= (1 << CS21); // set prescaler to 64
  TCCR2B |= (1 << CS20);
  
  EIMSK = 0;  //clear external interrupt mask
  EIMSK |= 1; //enable external interrupt

  EICRA = 0;  //clear external interrupt control register
  EICRA |= 2; // INT0 triggers interrupt on falling edge
  
  interrupts(); // turn on interrupts
}

ISR(TIMER1_COMPA_vect){
  cnt = TCNT2; // set counter to TCNT2 value
}

void loop()
{
  triggerPulse();
  //delay(100);
  Serial.println(cnt); //print range based on counter
  
  //while(1); //one-time operation
}


