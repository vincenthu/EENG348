// Project 1b 

// R.Kuc 

// 8/29/2013 

// 

// This program transmits with single byte inputs only 

// displays byte in different formats 

// controls on-board LED ("a" turns it on, else turned off) 

// Replaces Arduino commands with register values 

int inByte; // for incoming serial data 

void setup() { 

  Serial.begin(9600); // opens serial port, sets data rate to 9600 baud 

  //pinMode(13, OUTPUT); // on-board LED is pin 13 (port B, pin 5) 

  DDRB = 1<<5; // Data direction register for port B, pin 5 set as output 

} 

void loop() { 

  // display data only when you receive data: 

  if (Serial.available() > 0) { 

    // read the incoming byte: 

    inByte = Serial.read(); 

    // say what you got: 

    Serial.println("I received: "); 

    Serial.print("DEC: "); 

    Serial.println(inByte, DEC); // decimal 

    Serial.print("HEX: "); // octal 

    Serial.println(inByte, HEX); 

    Serial.print("Binary: "); 

    Serial.println(inByte, BIN); 

    Serial.print("ASCII: "); 

    Serial.write(inByte); 

    Serial.println(); // line feed + CR 

    PORTB &= 255 - 32; // turn off LED 

    if(inByte == 'a'){ 

      // PORTB = 255; // turn on if 'a" 

      // PORTB = 1<<5; // PB5 = 1, all others = 0 

      PORTB |= 1<<5; // PB5 = 1, all others unchanged 

    } 

  } 

} 

