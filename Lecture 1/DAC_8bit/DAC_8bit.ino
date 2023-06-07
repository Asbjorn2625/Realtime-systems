#define RESOLUTION 8
#define MAX_VALUE 255
#define FREQUENCY 50
#define SINUS 1  // Change to 0 if you want a ramp

int outputs[RESOLUTION] = {0, 1, 2, 3, 4, 5, 6, 7};
int vals[RESOLUTION];

unsigned long lastMillis = 0;
unsigned long period = 10;  // mili seconds


void setup() {
  Serial.begin(9600);
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
      Serial.println(analogRead(A0));
    }

  } else {
    int valSin = 0;
    // For creating a sinus wave
    for (int i = 0; i < 360; i++) {  //looping the value of i from 0 to 360 degree

      valSin = ((sin(i * DEG_TO_RAD) + 1) * MAX_VALUE) / 2;  // creating the sinwave from i, 255 is the highest value in 8 bit (0-255)
      writeValue(valSin);

      // Wait for the specified frequency
      while (1) {
        unsigned long currentMillis = millis()-lastMillis;
        if (currentMillis > period) {
          lastMillis = currentMillis;
          Serial.println(analogRead(A0));
          break;
        }
      }
    }
  }
}