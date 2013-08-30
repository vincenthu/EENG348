// Project 1 

// R.Kuc 

// 8/29/2013 

// 

// This program transmits with single byte inputs only 

// displays byte in different formats 

// controls on-board LED ("a" turns it on, else turned off) 

int inByte; // for incoming serial data 

void setup() { 

  Serial.begin(9600); // opens serial port, sets data rate to 9600 baud 

  pinMode(13, OUTPUT); // on-board LED is pin 13 (port B, pin 5 ) 

} 

void loop() { 

  // display data only when you receive data: 

  if (Serial.available() > 0) { 

    // read the incoming byte: 

    inByte = Serial.read(); 

    // display what you read: 

    Serial.println("I received: "); 

    Serial.print("DEC: "); 

    Serial.println(inByte, DEC); 

    Serial.print("HEX: "); 

    Serial.println(inByte, HEX); 

    Serial.print("Binary: "); 

    Serial.println(inByte, BIN); 

    Serial.print("ASCII: "); 

    Serial.write(inByte); // Serial.write prints ASCII 

    Serial.println(); // line feed + CR 

    digitalWrite(13,0); // turn off LED 

    if(inByte == 'a'){ 

      digitalWrite(13,1); // turn on if 'a' was sent 

    } 

  } 

} 

