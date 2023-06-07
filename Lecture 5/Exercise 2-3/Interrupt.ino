#define LED_PIN 9
#define BUTTON_PIN 2
int cnt = 0;
unsigned long lastTime = 0;

volatile byte ledState = LOW;
void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), blinkLed, RISING);
}
void loop() {
  Serial.print("CNT:");
  Serial.println(cnt);
}
void blinkLed() {
  unsigned long current_time = millis();
  if(current_time-lastTime >= 50){
    lastTime = current_time;
    ledState = !ledState;
    cnt++;
    digitalWrite(LED_PIN, ledState);
  }
}