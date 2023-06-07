#define RESOLUTION 8
#define MAX_VALUE 255
#define SINUS 1  // Change to 0 if you want a ramp
#define SINUS_FREQUENCY 50
#define READ_FREQUENCY 250

int outputs[RESOLUTION] = {0, 1, 2, 3, 4, 5, 6, 7};
int vals[RESOLUTION];

unsigned long lastMillis = 0;

// the frequency of the sine wave
float sineWaveIncrement = (SINUS_FREQUENCY * 360) / 1000.0; // calculates how much to increment i per millisecond

// the frequency of reading analog value
unsigned long lastAnalogReadMillis = 0;
unsigned long analogReadPeriod = 1000 / READ_FREQUENCY;

void setup() {
  Serial.begin(921600);
  for (int i = 0; i < RESOLUTION; i++)
    pinMode(outputs[i], OUTPUT);
}

// Writing a value into the DAC
void writeValue(int value) {
  for (int u = 0; u < RESOLUTION; u++) {
    vals[u] = (value >> u) & 1;
    digitalWrite(outputs[u], vals[u]);
  }
}

void loop() {
  // For creating a ramp
  if (SINUS != 1) {
    for (int i = 0; i < MAX_VALUE; i++) {
      writeValue(i);
      readAnalogWithFrequency();
    }
  } else {
    float valSin = 0;
    // For creating a sinus wave
    for (float i = 0; i < 360; ) {  // looping the value of i from 0 to 360 degree
      valSin = ((sin(i * DEG_TO_RAD) + 1) * MAX_VALUE) / 2;  // creating the sinwave from i, 255 is the highest value in 8 bit (0-255)
      writeValue(valSin);

      // Wait for the specified frequency
      while (1) {
        unsigned long currentMillis = millis();
        if (currentMillis - lastMillis >= 1) {
          lastMillis = currentMillis;
          i += sineWaveIncrement; // adjust the increment of i based on the frequency
          break;
        }
        readAnalogWithFrequency();
      }
      if(i >= 360) i -= 360; // reset i to keep it within 0 to 360 degree
    }
  }
}

// Reading analog value with specified frequency
void readAnalogWithFrequency() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastAnalogReadMillis >= analogReadPeriod) {
    lastAnalogReadMillis = currentMillis;
    Serial.println(analogRead(A0));
  }
}

