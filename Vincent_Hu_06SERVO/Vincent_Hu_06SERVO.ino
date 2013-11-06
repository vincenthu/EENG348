// By Vincent Hu

#define CTRL 12
#define STROBE 11

void InitLCD() 
{ 
  /* Wait a bit after power-up */
  delay(200); 

  /* Initialize the LCD to 4-bit mode */
  LCD_cmd(3); 
  delay(50); 

  LCD_cmd(3); 
  delay(10); 

  LCD_cmd(3); 
  delay(10); 

  LCD_cmd(2); 
  delay(10); 

  /* Function Set */
  LCD_cmd(2); // Set interface length
  delay(10); 

  LCD_cmd(8); // N = 1: Two lines, DL = F = 0: 4bit 5x7 font
  delay(10); 

  /* Display OFF */
  LCD_cmd(0); 
  delay(10); 

  LCD_cmd(8); // D = C = B = 0: Display, Cursor, Blink off
  delay(10); 

  /* Display ON */
  LCD_cmd(0); 
  delay(10); 

  LCD_cmd(0x0F); // D = C = B = 1: Display, Cursor, Blink on
  delay(10); 

  /* Entry mode */
  LCD_cmd(0); 
  delay(10); 

  LCD_cmd(6); // ID = 1: Increment cursor after each byte write
  delay(10); 

  /* Clear Screen */
  LCD_cmd(0); 
  delay(10); 

  LCD_cmd(1); // Clear display
  delay(100); 

  /* Cursor home */
  LCD_cmd(0); 
  delay(10); 

  LCD_cmd(2); // Return cursor and LCD to home position
  delay(100); 

} 

void WriteLCD( char* message ) 
{ 
  unsigned char i; 
  /* Load each character one by one*/
  for(i=0; i<20; i++ ) 
  { 
    if( !message[i] ) 
      break; 

    LCD_data(message[i]); 
  } 
} 

void LCD_data( unsigned char value ) 
{ 
  //PORTB |= 1 << CTRL; // set RS to 1 (data)
  digitalWrite(CTRL,HIGH);
  LCD_xfer( value >> 4 ); // get higher nibble
  delay(1);
  LCD_xfer( value );      // get lower nibble
  delay(1);
} 

void LCD_cmd( unsigned char value ) 
{  
  //PORTB &= ~(1 << CTRL); // set RS to 0 (instruction)
  digitalWrite(CTRL,LOW);
  LCD_xfer( value );       
  delay(1);
} 

void LCD_xfer( unsigned char value ) 
{ 
  /* Move lower nibble to higher nibble*/
  value = value & 0x0F; // clear high nibble
  value = value << 4;   // move low nibble to high

  /* Set high nibble of PORTD (4,5,6,7) to high nibble of value*/
  PORTD = PORTD & 0x0F; //clear high nibble
  PORTD = PORTD | value;//set high nibble to value

  //PORTB |= 1 << STROBE;  //set Enable high
  digitalWrite(STROBE,HIGH);
  delay(1); 
  //PORTB &= ~(1 << STROBE); // set Enable low
  digitalWrite(STROBE,LOW);
} 

int pos = 0;
volatile double pulse;
int inputangle = 0;

void setup(){
  Serial.begin(9600);

  DDRB |= 255;

  noInterrupts();
  ADCSRA |= 1 << 7; // Enable ADC
  
  //clear Timer1 control registers
  TCCR1A = 0; 
  TCCR1B = 0; 
  
  TCCR1B |= (1 << WGM12);   // Set to CTC mode
  TCCR1B |= (1 << CS12);    // Set prescaler to 256
  TIMSK1 |= (1 << OCIE1A);  // Enable
  TCNT1 = 0;                // Clear timer
  PORTB = 2;                // Set pin 9 to high
  OCR1A = floor(16000.0/256.0 * 1.0);

  interrupts();
  
  
  /*Setup LCD*/
  DDRD = 0;
  DDRD |= 0xF0; // make upper nibble output
  
  InitLCD(); 

}

ISR(TIMER1_COMPA_vect) // timer compare interrupt service routine
{
  if(inputangle == 1){
    PINB = 2;           
    TCNT1 = 0;          // reset timer1 
    OCR1A = 1042-OCR1A; // get period closest to 16.667
  }
}

void loop(){
  char val[10];

  while(Serial.available() <= 0);

  delay(10);
  
  // get input value
  int cnt = 0;
  int i;
  while((i = Serial.read())){
    if( i == -1 ){ // account for negatives
      val[cnt]=i;
      break;
    }
    else{
      LCD_data(i); //display on LCD
      val[cnt]=i;  //initialize array
      cnt++;
    }
  }

  pulse = (0.5/45.0 * atoi(val) + 1.5); // get pulse and add 1.5, which is 0
  OCR1A = floor(16000.0/256.0 * pulse); // set interrupt counter upper bound
  inputangle = 1;
  while(1);
}

void delay_us(int mic_sec){
 volatile int q;

 for(q = 0; q < 35.0/248*1000; q++);
}
