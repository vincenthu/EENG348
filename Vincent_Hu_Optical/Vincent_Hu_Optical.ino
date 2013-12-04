//Vincent Hu Project Self Clock

#define LED 13 // LED anode (HIGH), cathode to GND pin
#define COLL 2 // PT collector (HIGH)
#define EMIT 3 // PT emitter (LOW)

/*Variables for transmission module*/
#define DEL 5   //minimum threshold delay time for LED change
volatile int d[16]; // data array 

int tcnt = 0; // cnt is index for data array 
volatile boolean transition = false; //Determines whether to toggle LED 
volatile int state = 0; //Determines whether LED is HIGH (LOW is default)

/*Variables for receiving module*/
int rt[50];
int rcnt = 0;        //counter for counting ext ints

ISR(INT0_vect){
  rt[rcnt++] = TCNT1;
  TCNT1 = 0;
}

void doTransition()
{
  (state)?(state=0):(state=1); // toggle state
  (state)?digitalWrite(LED,HIGH):digitalWrite(LED, LOW); // write depending on state
}

//Converts an int array of bit values to single char value
char toASCII(int* arr)
{
  char data = 0; //initialize data for return
  // construct the ASCII character
  for(int i = 7; i >=0 ; i--)
  {
    int shift = 7 - i;   // determines where to put the bit
    data |= arr[i]<<shift; // set the bit
  }
  return data;
}

// Converts a char to a int array with 4 0's, then bits representing that char
// Example: k -> { 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1 }
void toArray(char c)
{
  //Serial.print("Transmitting: ");

  // Fill first four zeroes
  for(int i = 0; i < 4; i++) 
  {
    d[i] = 0; 
  }

  // Fill rest of array with bits representing char
  for(int i = 4; i < 12; i++)
  {
    int shift = 7 - (i - 4);
    d[i] = (c & ( 1 << shift )) >> shift;
    //Serial.print(d[i]);
  }

  // Fill last four zeroes
  for(int i = 12; i < 16; i++)
  {
    d[i] = 0; 
  }
}

void transmit()
{
  // Transmit 12 bits of data
  while(tcnt < 16)
  {
    //Serial.println(d[tcnt]);

    //Toggle state and make transition.
    (d[tcnt++]==1)?transition=true:transition=false; //Get ready for transition if data = 1
    doTransition();                                  //Do transition regardless of value

    delay(DEL);                                      //Delay T_p/2
    if(transition)                                   //Do transition if 1, then unset transition
    {
      doTransition();
      transition = false;
    }
    delay(DEL);                                      //Delay T_p/2
  }

  //If last bit was 0, make sure to make extra transition so last bit can be set on receive
  if(tcnt == 16 && d[tcnt-1] == 0)
  {
    doTransition();
  }
}

void setup()
{ 
  Serial.begin(115200);

  /*Configure pins*/
  pinMode(LED, OUTPUT); // set LED to output to control
  pinMode(COLL, INPUT); // set COLL to input to generate external interrupts
  pinMode(EMIT, OUTPUT); // set EMIT to output to set to GND

  digitalWrite(LED, state); // connect LED to 5V
  digitalWrite(COLL, HIGH); // connect PT with built-in resistor and 5V
  digitalWrite(EMIT, LOW); // set emitter to ground

  /*Configure receiving module external interrupt*/
  noInterrupts(); //turn off interrupts during config

  TCCR1A = 0; // clear Timer1
  TCCR1B = 0; // set to counter mode

  TCCR1B |= (1 << CS12); // set prescaler to 1024
  TCCR1B |= (1 << CS10);

  EIMSK = 1; // enable exter int on INT0
  EICRA = 1; // INT0 triggers int on any change 

  interrupts(); // turn on interrupts

    Serial.println();
}

void loop()
{
  if(Serial.available()) { // data received on com port 

    char val = Serial.read(); // read numdat
    val = toupper(val);
    //Serial.println(val);
    toArray(val);        // stores bits of val into transmit array d[12]
    transmit();          // transmits bits
  }
  
  //Process transitions array and print char after ensuring
  //1: Received at least 16 transitions (4 0's+8 transitions+4 0's =shortest possible message)
  //2: TCNT1 is greater than 1000 (64 ms, since it is longer than any possible transition rated 10-50ms)
  if(rcnt >= 16 && TCNT1 > 1000)
  { 
    float avg; // effective T_p value
    float half;
    float threshold; // separates T_p from T_p/2 (0 from 1)
    for(int i = rcnt - 4; i < rcnt; i++)
    {
       avg+=rt[i];
    }
    avg = avg/4;
    half = avg/2;
    threshold = half + half/2;
      
    int arri = 7;              //counter for array to be built
    int arr[8];                //array to be built for toASCII
    int j = rcnt - 5;          //counter for reading array backwards
    
    // starting with first transition in the back of the array,
    // get the bits according to transition time
    while(arri >= 0)
    {
       if(rt[j] > threshold)
       {
         arr[arri] = 0;
       }
       else
       {
         arr[arri] = 1;
         j--;
       }
       arri--;
       j--;
    }
    
    // Debug for array
//    for(int i = 0; i < 8; i++)
//    {
//     Serial.print(arr[i]); 
//    }
//    Serial.println();
    Serial.println(toASCII(arr));
    
    while(1);
  }
}




