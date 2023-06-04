void setup() {
Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);

PORTB = B00000000;



}

void loop() {
  // put your main code here, to run repeatedly:
delay(100);  
set_pin(&PORTB, 3);
delay(100);
reset_pin(&PORTB,3);
}


void reset_pin(byte* Port, int reset){
  byte and_mask = ~(1 << reset);
  *Port &= and_mask;
}

void set_pin(byte* Port, int set){
  byte or_mask = (1 << set);
  *Port |= or_mask;
}