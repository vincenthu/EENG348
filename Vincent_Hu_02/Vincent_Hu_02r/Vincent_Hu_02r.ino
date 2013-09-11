// Project 2r
// Vincent Hu

#define A 2
#define B 4
#define C 6
#define D 7
#define E 8
#define F2 11 //F couldn't be used for some reason
#define G 12
#define SRC 13

#define ISPORTD(n) ((n) <= 7 && (n) >= 0)
#define ISPORTB(n) ((n) <= 13 && (n) >=8)

int inByte; // for incoming serial data 
int pins[7] = {A,B,C,D,E,F2,G}; // array of active pins

/*Functions to return specific bit of number*/
int b0(int n) { return n&1; }
int b1(int n) { return (n>>1)&1; }
int b2(int n) { return (n>>2)&1; }
int b3(int n) { return (n>>3)&1; }

/*Functions to return boolean value for activating segs*/
int seg_a(int n){ return !(b1(n)||b3(n)||(b2(n)&&b0(n))||(!b2(n)&&!b0(n))); }
int seg_b(int n){ return (b2(n)&&!b1(n)&&b0(n))||(b2(n)&&b1(n)&&!b0(n)); }
int seg_c(int n){ return !(!b1(n)||b0(n)||b2(n)); }
int seg_d(int n){ return !(b3(n)||(b1(n)&&!b0(n))||(!b2(n)&&b1(n))||(!b2(n)&&!b0(n))||(b2(n)&&!b1(n)&&b0(n))); }
int seg_e(int n){ return !((b1(n)&&!b0(n))||(!b2(n)&&!b0(n))); }
int seg_f(int n){ return (b1(n)&&b0(n))||(!b2(n)&&b1(n))||(!b3(n)&&!b2(n)&&b0(n)); }
int seg_g(int n){ return (b2(n)&&b1(n)&&b0(n))||(!b3(n)&&!b2(n)&&!b1(n)); }

/*Prints out truth table for each digit*/
void debug_seg()
{
  for(int num = 0; num < 10; num++)
  {   
   Serial.print(seg_a(num));
   Serial.print(seg_b(num));
   Serial.print(seg_c(num));
   Serial.print(seg_d(num));
   Serial.print(seg_e(num));
   Serial.print(seg_f(num));
   Serial.print(seg_g(num));
   Serial.println();
  }
}

// turns segs on/off based on num
void displayNum(int num)
{
    // reset the pins
    PORTB &= 0;
    PORTD &= 0;
    configPinPort(SRC, 1);
    
    // display new pin config
    for(int i=0; i < 8; i++)
    {
      int pin = pins[i];
      switch(pin) {
        case A: configPinPort(A, seg_a(num)); break;
        case B: configPinPort(B, seg_b(num)); break;
        case C: configPinPort(C, seg_c(num)); break;
        case D: configPinPort(D, seg_d(num)); break;
        case E: configPinPort(E, seg_e(num)); break;
        case F2: configPinPort(F2, seg_f(num)); break;
        case G: configPinPort(G, seg_g(num)); break;
      }
    }  
}

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

void setup() { 

  Serial.begin(9600); // opens serial port, sets data rate to 9600 baud  
  
  configPinDDR(SRC, 1);
  configPinPort(SRC, 1);
  
  for(int i = 0; i < 8; i++)
  {
    configPinDDR(pins[i], 1);
    configPinPort(pins[i], 1);
  }
} 

void loop() { 
 if (Serial.available() > 0) { 
    // read the incoming byte: 

    inByte = Serial.read(); 
    int num = inByte - '0';
    
    // display num if is a num, otherwise just loop through the nums for fun
    if(num >= 0 && num < 10)
    {
      // print out decimal and binary representations of number
      Serial.print("DEC: "); 
      Serial.println(num, DEC); // decimal 
      
      Serial.print(b3(num));
      Serial.print(b2(num));
      Serial.print(b1(num));
      Serial.println(b0(num));
      
      displayNum(num);  
    }
 }

}
