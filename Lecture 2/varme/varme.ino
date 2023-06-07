#include <LiquidCrystal.h>
int ThermistorPin = 0;
int Vo;
float R1 = 10000;
float logR2, R2, T;
float A = 3.354016E-03;
float B = 2.569850E-04;
float C = 2.620131E-06;
float D = 6.383091E-08;

void setup() {
Serial.begin(9600);
}

void loop() {

  Vo = analogRead(ThermistorPin);
  Serial.println(Vo);

R2 = R1* (1023.0 / (float)Vo - 1.0);
logR2 = log(R2/R1);
  T = 1/(A+ B*logR2 + C*logR2*logR2 + D*logR2*logR2*logR2); 

  T = T - 273.15; 

  Serial.print("Temperature: "); 
  Serial.print(T);
  Serial.println(" C"); 

  delay(1000);
}
