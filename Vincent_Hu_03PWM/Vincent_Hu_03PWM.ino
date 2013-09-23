//P03r
//Vincent Hu

//Red
#define RL_AN 11 // Anode PWM
#define RP_HI 9  // Pot hi
#define RP_LO 4  // Pot lo
#define RP_ADC 1 // Pot ADC

//Green
#define GL_AN 6
#define GP_HI 10
#define GP_LO 7
#define GP_ADC 2

//Blue
#define BL_AN 5
#define BP_HI 3
#define BP_LO 2
#define BP_ADC 0

// Port ranges
#define ISPORTD(n) ((n) <= 7 && (n) >= 0)
#define ISPORTB(n) ((n) <= 13 && (n) >=8)

const int colors = 3;
int anodes[colors] = { RL_AN, GL_AN, BL_AN };
int pot_hi[colors] = { RP_HI, GP_HI, BP_HI };
int pot_lo[colors] = { RP_LO, GP_LO, BP_LO };
int adc[colors]    = { RP_ADC, GP_ADC, BP_ADC};

void setup() 

{ 
  Serial.begin(9600); // opens serial port, sets data rate to 9600 baud 
  
  // Set up pins
  for(int i = 0; i < colors; i++)
  {
    configPinDDR(anodes[i], 1); // PWM pin
        
    configPinDDR(pot_lo[i], 1); // Pot Low
    configPinPort(pot_lo[i], 0);
       
    configPinDDR(pot_hi[i], 1); //Pot high
    configPinPort(pot_hi[i], 1);
  }
} 

int timecnt; //used for printing vals
int RADC_val, GADC_val, BADC_val; 
int* ADC_vals[colors] = { &RADC_val, &GADC_val, &BADC_val };

void loop() 

{ 
  
  // Read value from ADC and convert then write to 8-bit PWM
  for(int i = 0; i < colors; i++)
  {
    *ADC_vals[i] = ADC8(adc[i]);
    
    // PWM func doesn't use this
    if(anodes[i] == RL_AN) 
    {
      analogWrite(anodes[i], *ADC_vals[i]);  
    }
    // Pins 5 and 6 use PWM func
    else
    {
       PWM(anodes[i], *ADC_vals[i]);
    }
  }
  
  // Only print vals every second and reset timecnt.
  if(timecnt == 1000)
  {
   printVals();
   timecnt = 0; 
  }
  
  //Update color every 10 ms.
  delay(10); 
  timecnt+=10;

}

void PWM(int chan_num, int PWM_val)
{
  
  // 0x05: delay(62) or 62 millis() ~ 1 second
   TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);  //to use fast pwn on timer 0
   TCCR0B = _BV(CS00) | _BV(CS02); //to use prescale 1024
   if(chan_num == 6)
   {
     OCR0A = PWM_val;
   }
   if(chan_num == 5)
   {
     OCR0B = PWM_val;
   }
}

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

// Sets voltage settings for pin
void configPinPort(int pin, int val)
{
  if(ISPORTB(pin))
  {
   pin -= 8;
   PORTB |= val<<pin;
  }
  else if(ISPORTD(pin))
  {
    PORTD |= val<<pin;
  }  
}

// Sets input/output settings for pin
void configPinDDR(int pin, int val)
{
  if(ISPORTB(pin))
  {
   pin -= 8;
   DDRB |= val<<pin;
  }
  else if(ISPORTD(pin))
  {
    DDRD |= val<<pin;
  }  
}

void printVals()
{
  Serial.print("RED: ");
  Serial.println(RADC_val/4);
  
  Serial.print("GREEN: ");
  Serial.println(GADC_val/4);
  
  Serial.print("BLUE: ");
  Serial.println(BADC_val/4);
  
  Serial.println();
}
