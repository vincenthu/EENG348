//P05
//Vincent Hu

// variables
int arr[35];   // array to store samples
int cnt = 0;   // counter to keep track of sampling progress
int period;    // sample period
char val[10];  // used to get sample period
bool gotnum = false; // used to ensure sample period was received
int potPin = 0; // pot connections: 1 = 5V, 3= GND, 2=A0

int ADC8(int chan_num){

  // set up the ADCSRA register

  ADCSRA |= 128; // sets the ADEN bit, which enables the ADC, to 1
  ADMUX |= 32; // sets ADLAR to 1 to left-adjust the data
  ADMUX |= 1<<6; // sets the reference voltage
  PRR &= B11111110; // turn off PRADC
  ADCSRA &= B11111001; //set prescaler to 2 (minimum)
  ADCSRA |= 1; 
  ADMUX &= B11110000; // resets the mux bits
  ADMUX |= chan_num; // this selects the correct input A0-A2
  delay(1); // give the mux register a chance to settle in before conversion

  ADCSRA |= 1<<6; // start a single conversion

  while ((ADCSRA & 1<<4) == 0); // after this while loop is done, the ADIF flag will be up

  int ADC8 = ADCH; // extract this number and put into ADC8

  return ADC8;

}

ISR(TIMER1_COMPA_vect) // timer compare interrupt service routine
{
  arr[cnt++] = ADC8(potPin); // read ADC into global variable
}

 Print samples and max sample value
void printSamples(int *samples, int T)
{
  int maxval = samples[0]; //record max value

  // print the period
  Serial.print("T_s (ms) = "); 
  Serial.println(T);

  // print cycles
  for(int i = 0; i < 35 ; i++)
  {
    // keep track of max value
    if( samples[i] > maxval)
    {
      maxval = samples[i]; 
    }

    // make sure formatting is consistent
    if(i < 10)
    {
      Serial.print(" ");
    }
    Serial.print(i);
    Serial.print(" ");
    if(samples[i] == 0)
    {
      Serial.println("*"); 
    }
    else
    {
      Serial.print("|");
      for(int k = 0; k < samples[i] - 1; k++)
      {
        Serial.print(" ");
      }
      Serial.println("*");
    }

  }

  // print max val
  Serial.print("Max val= ");
  Serial.println(maxval);
}

void setup(){
  Serial.begin(9600);
}


void loop() {
  if(!gotnum && Serial.available() >0) {
    delay(10);
    // get period

    for (int i=0;i<10;i++){
      val[i] = Serial.read();  //read one byte
      if (val[i] == -1) break; //if buffer is empty serial.read produces -1
    }
    period = atoi(val);
    Serial.println(period);
    gotnum = true;

    noInterrupts(); // disable all interrupts

    TCCR1A = 0; // clear Timer1 control registers
    TCCR1B = 0;
    TCCR1B |= (1 << WGM12); // CTC mode (Clear Timer on Compare)
    TCCR1B |= (1 << CS01); // No prescaler (min)
    TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
    OCR1A = period * (16/1) * 1000; // compare match register 16MHz/prescale 3125/(16MHz/256) (50ms)
    TCNT1 = 0; // clear counter

    interrupts(); // enable all interrupts
  }

  if(cnt == 35)
  {
    noInterrupts(); //no more sampling
    printSamples(arr, period);
    cnt++; //make sure this does not get executed again
  }

}










