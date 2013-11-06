
//Sonar macros
#define TRIG 3
#define INT0 2

//Servo macros
#define PWM_PIN 11
#define PERIOD 4166 // With 64 prescaler, 16.667ms/(1/16MHz/64) = 4166.75

//64 Prescaler (f=250kHz, T=4us; For 1ms, OCR1A=250. 1.5ms, 375. 2.0ms, 500)

//Sonar vars
int mult = 0; //multiplier
int count = 0; //count 5 times and then find the median

//Servo vars
boolean servoHi;
int pulse = 250; // 250*4us = 1ms -> 0 degrees
int currdegree = 0;

void setup()
{
  Serial.begin(9600);
  
  //Sonar init pins
  pinMode(TRIG, OUTPUT);
  pinMode(INT0, INPUT);
  digitalWrite(TRIG, LOW);
  
  //Servo init pins
  digitalWrite(PWM_PIN, LOW);
  pinMode(PWM_PIN, OUTPUT);
  
  //Set up interrupts
  noInterrupts();
  
  //Set up sonar interrupts
  TCCR2A = 0; // clear timer1
  TCCR2B = 0;

  TCCR2B |= (1 << CS21); // set prescaler to 64
  TCCR2B |= (1 << CS20);

  TCNT2 = 0;

  TIMSK2 |= (1 << OCIE2A);
  
  EIMSK = 1; // enable exter int
  EICRA = 2; // INT0 triggers int on falling edge
  
  //Set up servo interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B =(1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS11) | (1 << CS10); // 64 prescaler
  
  TCNT1 = 0;
  OCR1A = pulse; //Start at pulse = 250 where 0 degrees
  
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
  
  //Init servo pins
  digitalWrite(PWM_PIN, HIGH);
  servoHi = true;
}

void loop()
{
  servoSetPosition(-90.);
  triggerPulse();
  delay(1000);
  servoSetPosition(0.);
  triggerPulse();
  delay(1000);
  servoSetPosition(90.);
  triggerPulse();
  delay(1000);
}

/***Sonar functions***/
// External interrupt routine
ISR(INT0_vect)
{
   gettof(); 
}

// Internal interrupt routine
ISR(TIMER2_COMPA_vect){
  mult++; // increment multiplier
}

void triggerPulse()
{
  digitalWrite(TRIG, HIGH); //transmit pulse
  delay(2);
  digitalWrite(TRIG, LOW); //end pulse

  TCNT2 = 0; //set timer to 0 at the downward transition of the Trig pulse
  mult = 0;
  delay(100);
}

void gettof()
{
  const float error = 7.75;
  const float sound = 343.2;
  int tof = TCNT2;
  if(mult > 0)
    tof *= mult;
  
  Serial.print((tof/2)*(4*sound/1000)/10); //cm
  Serial.print("\t");
  Serial.print(mult);
  Serial.print(" ");
  Serial.println(currdegree);
  mult = 0;
}

/***Servo functions***/

// Sets position of the servo
void servoSetPosition(float degree)
{
  float realdegree = 1.4 * degree + 33.33; // the servo was off, so map (-90,-90), (0,30), (90,160) to line of best fit
  TIMSK1 &= ~(1 << OCIE1A); // disable timer compare interrupt
  pulse = (int)((25/9)*realdegree + 250);
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  currdegree = degree;
}

ISR(TIMER1_COMPA_vect)
{
  if(servoHi)
  {
    digitalWrite(PWM_PIN, LOW);
    OCR1A = 4166 - pulse;
    servoHi = false;
  }
  else
  {
    digitalWrite(PWM_PIN, HIGH);
    OCR1A = pulse;
    servoHi = true;
  }
}
