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
  digitalWrite(CTRL, HIGH); // set RS to 1 (data)

  LCD_xfer( value >> 4 ); // get higher nibble
  delay(1);
  LCD_xfer( value );      // get lower nibble
  delay(1);
} 

void LCD_cmd( unsigned char value ) 
{  
  digitalWrite(CTRL, LOW); // set RS to 0 (instruction)
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

    digitalWrite(STROBE, HIGH); 
  delay(1); 
  digitalWrite(STROBE, LOW);
} 

void LCD_row( unsigned char value )
{
  /*Move to line 1*/
  if(value == 1)
  {
    /*Write command 0x80*/
    LCD_cmd(8); 
    delay(10); 

    LCD_cmd(0);
    delay(100);
  }

  /*Move to line 2*/
  if(value == 2)
  {
    /*Write command 0xC0*/
    LCD_cmd(0x0C); 
    delay(10); 

    LCD_cmd(0);
    delay(100); 
  }
}

void setup() 
{ 
  Serial.begin(9600);

  /*Output pins*/
  pinMode(4, OUTPUT);  // D4 
  pinMode(5, OUTPUT);  // D5
  pinMode(6, OUTPUT);  // D6
  pinMode(7, OUTPUT);  // D7
  pinMode(STROBE, OUTPUT); // Enable
  pinMode(CTRL, OUTPUT);   // RS 

  /* Initialize the LCD as the very first thing */
  InitLCD(); 

  /* Write a simple message to the LCD */
  WriteLCD( "Vincent Hu" ); 
  LCD_row(2); //move to second line to get ready for input
}

char val[10];

void loop()
{
  if(Serial.available() >0) {
    delay(10);
    
    for (int i=0;i<10;i++){
      val[i] = Serial.read();  //read one byte
      WriteLCD(val); // print the number 
      if (val[i] == -1){
        WriteLCD("µs"); // print µs after the number
        break; //if buffer is empty serial.read produces -1
      }  
    }
  }
}


