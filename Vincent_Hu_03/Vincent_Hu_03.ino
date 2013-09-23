//P03h
//Vincent Hu

//Red
#define RL_AN 11 // Anode PWM
#define RP_HI 5  // Pot hi
#define RP_LO 4  // Pot lo
#define RP_ADC 1 // Pot ADC

//Green
#define GL_AN 10
#define GP_HI 6
#define GP_LO 7
#define GP_ADC 2

//Blue
#define BL_AN 9
#define BP_HI 3
#define BP_LO 2
#define BP_ADC 0

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
    pinMode(anodes[i], OUTPUT); // PWM pin
   
    pinMode(pot_lo[i], OUTPUT); // Pot Low      
    digitalWrite(pot_lo[i], LOW);
    
    pinMode(pot_hi[i], OUTPUT); //Pot high    
    digitalWrite(pot_hi[i], HIGH);    
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
    *ADC_vals[i] = analogRead(adc[i]);
    
    analogWrite(anodes[i], *ADC_vals[i]/4);
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
