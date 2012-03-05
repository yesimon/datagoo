byte seven_seg_hex[10][3] = { {B00000000, B00000000, B00000100},  // = 0
                              {B00100000, B00000011, B00001100},  // = 1
                              {B10000000, B00000000, B00001000},  // = 2
                              
                              {B00000000, B00000010, B00001000},  // = 3
                              {B00100000, B00000011, B00000000},  // = 4
                              {B01000000, B00000010, B00000000},  // = 5
                              {B01000000, B00000000, B00000000},  // = 6
                              {B00000000, B00000011, B00001100},  // = 7
                              {B00000000, B00000000, B00000000},  // = 8
                              {B00000000, B00000011, B00000000},  // = 9
                                               };
                                               
                                               
volatile boolean seg_dig = 0;
volatile byte seg_onesD = 0;
volatile byte seg_onesB = 0;
volatile byte seg_onesC = 0;
volatile byte seg_tensD = 0;
volatile byte seg_tensB = 0;
volatile byte seg_tensC = 0;
const byte portD_bitmask = B11100000;
const byte portB_bitmask = B00000011;
const byte portC_bitmask = B00001100;
const byte portC_digits_bitmask = B00000011;

/*
 * Libraries
 */
 
//Expects a number between 0 and 9 to write to a digit

/*
 * Function that writes a number to the seven segment display. Currently supports 0-9, will soon support -> 99
 */ 
void seg_write(int digit){ 
  
  digitalWrite(A5, LOW);
  digitalWrite(A4, HIGH);
  seg_dig = 1;

 //Turn on ALL the digits
 seg_onesD = seven_seg_hex[digit % 10][0];
 seg_onesB = seven_seg_hex[digit % 10][1];
 seg_onesC = seven_seg_hex[digit % 10][2];
 seg_tensD = seven_seg_hex[digit / 10][0];
 seg_tensB = seven_seg_hex[digit / 10][1];
 seg_tensC = seven_seg_hex[digit / 10][2];
}

//Inits the pins for the seven segment display. Must be called in setup()
void seg_init(){
   pinMode(A4, OUTPUT); //Digit Two  
   pinMode(A5, OUTPUT); //Digit One
   pinMode(5, OUTPUT); //Segment A
   pinMode(6, OUTPUT); //Segment B
   pinMode(7, OUTPUT); //Segment C
   pinMode(8, OUTPUT); //Segment D
   pinMode(9, OUTPUT); //Segment E
   pinMode(A3, OUTPUT); //Segment F
   pinMode(A2, OUTPUT); //Segment G
   
   //Turn off the digits
    
   digitalWrite(5, HIGH); //A
   digitalWrite(6, HIGH); //B
   digitalWrite(7, HIGH); //C
   digitalWrite(8, HIGH); //D
   digitalWrite(9, HIGH); //E
   digitalWrite(A3, HIGH); //F
   digitalWrite(A2, HIGH); //G
}
void seg_swap(){
   PORTC = PORTC ^ B00110000; //Toggle the characters
   if(seg_dig){
     PORTD = (PORTD & ~portD_bitmask) | (seg_onesD);
     PORTB = (PORTB & ~portB_bitmask) | (seg_onesB);
     PORTC = (PORTC & ~portC_bitmask) | (seg_onesC);
     seg_dig = 0;
   } else {
     PORTD = (PORTD & ~portD_bitmask) | (seg_tensD);
     PORTB = (PORTB & ~portB_bitmask) | (seg_tensB);
     PORTC = (PORTC & ~portC_bitmask) | (seg_tensC);
     seg_dig = 1;
   }
}

/*
 ***************************************************************
 */
void setup() {                
 seg_init();
 MsTimer2::set(10, seg_swap); // 500ms period
 MsTimer2::start();
}

void derp(){
  int d = 100;
  
  //Turn on Digit One
  digitalWrite(A4, HIGH);
  digitalWrite(A5, LOW);
  
  digitalWrite(5, LOW); //A
  digitalWrite(6, HIGH); //B
  digitalWrite(7, HIGH); //C
  digitalWrite(8, HIGH); //D
  digitalWrite(9, HIGH); //E
  digitalWrite(A3, HIGH); //F
  digitalWrite(A2, HIGH); //G
  
  delay(d);
  
  digitalWrite(A4, LOW);
  digitalWrite(A5, HIGH);
  
  delay(d);
  
  digitalWrite(5, HIGH); //A
  digitalWrite(6, LOW); //B
  
  delay(d);
  
  digitalWrite(6, HIGH); //B
  digitalWrite(7, LOW); //C
  
  delay(d);
  
  digitalWrite(7, HIGH); //C
  digitalWrite(8, LOW); //D
  
  delay(d);
  
  digitalWrite(A4, HIGH);
  digitalWrite(A5, LOW);
  
  delay(d);
  
  digitalWrite(8, HIGH); //D
  digitalWrite(9, LOW); //E
  
  delay(d);
  
  digitalWrite(9, HIGH); //E
  digitalWrite(A3, LOW); //F
  
  delay(d);
}
