byte start_byte = 2;
byte end_byte = 3;
byte type = 25;
byte ReceiverID_Low = 9;
byte ReceiverID_Middle = 12;
byte ReceiverID_High = 16;
byte Transducerlevel = 29;
byte quietTime = 10;
byte masters_in_range = 1;
byte *RSSI;
byte *TXID_low;
byte *TXID_middle;
byte *TXID_high;
unsigned int length;
byte checksum_low;
byte checksum_high;


void setup() {
  RSSI = new byte[masters_in_range];
  TXID_low = new byte[masters_in_range];
  TXID_middle = new byte[masters_in_range];
  TXID_high = new byte[masters_in_range];

  for (int i = 0; i < masters_in_range; ++i) {
    RSSI[i] = random(0, 255);
    TXID_low[i] = random(0, 255);
    TXID_middle[i] = random(0, 255);
    TXID_high[i] = random(0, 255);
  }

  length = 11 + masters_in_range * 4;

  // Calculate the checksum
  int checksum = 0;
  checksum = start_byte + type + end_byte + length + ReceiverID_Low + ReceiverID_Middle + ReceiverID_High + Transducerlevel + quietTime + masters_in_range;
  for (int i = 0; i < masters_in_range; ++i) {
    checksum += RSSI[i] + TXID_low[i] + TXID_middle[i] + TXID_high[i];
  }
  // split the checksum into higher and lower bytes
  checksum_high = highByte(checksum);
  checksum_low = lowByte(checksum);

  // Initialize the serial communication
  Serial.begin(9600);
}

void loop() {
  static int iteration = 0;  // Keeps track of the current iteration
  
  if (Serial.available() > 0) {
    int incomingByte = Serial.read();
    if (incomingByte > 1) {
      // Send the variable corresponding to the current iteration
      switch (iteration) {
        case 0:
          Serial.println(start_byte);
          break;
        case 1:
          Serial.println(length);
          break;
        case 2:
          Serial.println(type);
          break;
        case 3:
          Serial.println(ReceiverID_Low);
          break;
        case 4:
          Serial.println(ReceiverID_Middle);
          break;
        case 5:
          Serial.println(ReceiverID_High);
          break;
        case 6:
          Serial.println(Transducerlevel);
          break;
        case 7:
          Serial.println(quietTime);
          break;
        case 8:
          Serial.println(masters_in_range);
          break;
        default:
          if (iteration >= 9 && iteration < (9 + masters_in_range)) {
              Serial.println(RSSI[iteration - 9]);
          } else if (iteration >= (9 + masters_in_range) && iteration < (9 + 2 * masters_in_range)) {
              Serial.println(TXID_low[iteration - (9 + masters_in_range)]);
          } else if (iteration >= (9 + 2 * masters_in_range) && iteration < (9 + 3 * masters_in_range)) {
              Serial.println(TXID_middle[iteration - (9 + 2 * masters_in_range)]);
          } else if (iteration >= (9 + 3 * masters_in_range) && iteration < (9 + 4 * masters_in_range)) {
              Serial.println(TXID_high[iteration - (9 + 3 * masters_in_range)]);
          } else if (iteration == (9 + 4 * masters_in_range)) {
              Serial.println(checksum_low);
          } else if (iteration == (9 + 4 * masters_in_range + 1)) {
              Serial.println(checksum_high);
          } else if (iteration == (9 + 4 * masters_in_range + 2)) {
              Serial.println(end_byte);
          }
          break;
      }

      
      iteration++;  // Increment the iteration counter
      
      // Reset the iteration counter when all variables have been sent
      if (iteration > (23 + masters_in_range)) {
        iteration = 0;
      }
    }
  }
}
