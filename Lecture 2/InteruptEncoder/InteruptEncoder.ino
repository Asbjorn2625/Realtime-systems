int cnt=0;
void setup() {
pinMode(W1, INPUT_PULLUP);
pinMode(W2, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(W1), Count, RISING);
}
void Count() {
if (digitalRead(W2) == HIGH):
cnt++;
else:
cnt--;
}
void loop() {
}
