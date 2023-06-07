#include <Arduino_FreeRTOS.h>
#include <semphr.h>

SemaphoreHandle_t buttonMutex;
int lastButtonPressed = -1;

const int BUFFER_SIZE = 10;
int buttonPressBuffer[BUFFER_SIZE];
int bufferHead = 0;  // Index of the oldest item in the buffer
int bufferTail = 0;  // Index of the next available position in the buffer

// Define the button pins
const int BUTTON_PIN_1 = 2;
const int BUTTON_PIN_2 = 3;

const int STACK_SIZE = 128;

struct ButtonParams {
  int pin;
  // Add other parameters as needed
};

// Define the button variables
ButtonParams button1Params;
ButtonParams button2Params;

void handleButtonInterrupt() {
  int buttonPin = digitalPinToInterrupt(digitalRead(BUTTON_PIN_1) == LOW ? BUTTON_PIN_1 : BUTTON_PIN_2);

  xSemaphoreTake(buttonMutex, portMAX_DELAY);
  lastButtonPressed = buttonPin;
  buttonPressBuffer[bufferTail] = buttonPin;
  bufferTail = (bufferTail + 1) % BUFFER_SIZE;
  xSemaphoreGive(buttonMutex);
}

void Taskbutton(void* pvParameters) {
  ButtonParams* buttonParams = (ButtonParams*)pvParameters;
  int buttonPin = buttonParams->pin;

  pinMode(buttonPin, INPUT_PULLUP);
  
  for (;;) {
    if (digitalRead(buttonPin) == LOW) {
      xSemaphoreTake(buttonMutex, portMAX_DELAY);
      lastButtonPressed = buttonPin;
      buttonPressBuffer[bufferTail] = buttonPin;
      bufferTail = (bufferTail + 1) % BUFFER_SIZE;
      xSemaphoreGive(buttonMutex);

      // Button is pressed
      // Perform the corresponding action for this button

      // Example action:
      Serial.print("Button ");
      Serial.print(buttonPin);
      Serial.println(" pressed");
    }
    vTaskDelay(10);
  }
}

void processButtonPress(int buttonPin) {
  // Process the button press and remove corresponding items from the buffer
  // Example: Move the elevator to the floor indicated by the button

  // Print the floor reached
  Serial.print("Elevator reached floor ");
  Serial.println(buttonPin);

  // Remove all matching button presses from the buffer
  xSemaphoreTake(buttonMutex, portMAX_DELAY);
  for (int i = bufferHead; i != bufferTail; i = (i + 1) % BUFFER_SIZE) {
    if (buttonPressBuffer[i] == buttonPin) {
      // Remove the item from the buffer
      buttonPressBuffer[i] = -1;
    }
  }
  xSemaphoreGive(buttonMutex);
}

void elevatorTask(void* pvParameters) {
  for (;;) {
    xSemaphoreTake(buttonMutex, portMAX_DELAY);
    if (bufferHead != bufferTail && buttonPressBuffer[bufferHead] != -1) {
      int buttonPin = buttonPressBuffer[bufferHead];
      bufferHead = (bufferHead + 1) % BUFFER_SIZE;
      xSemaphoreGive(buttonMutex);

      // Process the button press
      processButtonPress(buttonPin);
    } else {
      xSemaphoreGive(buttonMutex);
      // No pending button presses, idle or perform other elevator tasks
      vTaskDelay(10);
    }
  }
}

void setup() {
  // Initialize the button mutex
  buttonMutex = xSemaphoreCreateMutex();

  // Initialize the button parameters
  button1Params.pin = BUTTON_PIN_1;
  button2Params.pin = BUTTON_PIN_2;

  // Create the button tasks
  xTaskCreate(Taskbutton, "Button1Task", STACK_SIZE, (void*)&button1Params, 1, NULL);
  xTaskCreate(Taskbutton, "Button2Task", STACK_SIZE, (void*)&button2Params, 1, NULL);

  // Create the elevator task
  xTaskCreate(elevatorTask, "ElevatorTask", STACK_SIZE, NULL, 0, NULL);

  // Attach interrupt handler
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_1), handleButtonInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_2), handleButtonInterrupt, FALLING);

  // Start the FreeRTOS scheduler
  vTaskStartScheduler();
}

void loop() {
  // Empty loop, tasks are managed by the scheduler
}
