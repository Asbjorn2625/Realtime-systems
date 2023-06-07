#include <Arduino_FreeRTOS.h>

// Constants
const int MESSAGE_SIZE = 10;

// Global variables
char producerMessage[MESSAGE_SIZE];
char consumerReply[MESSAGE_SIZE];

// Task handles
TaskHandle_t producerTaskHandle;
TaskHandle_t consumerTaskHandle;

// Function prototypes
void producerTask(void* pvParameters);
void consumerTask(void* pvParameters);

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Create the producer task
  xTaskCreate(producerTask, "Producer", 128, NULL, 2, &producerTaskHandle);

  // Create the consumer task
  xTaskCreate(consumerTask, "Consumer", 128, NULL, 1, &consumerTaskHandle);

  // Start the FreeRTOS scheduler
  vTaskStartScheduler();
}

void loop() {
  // Empty loop, tasks are scheduled by the FreeRTOS scheduler
}

void producerTask(void* pvParameters) {
  while (1) {
    // Generate a message to send
    snprintf(producerMessage, MESSAGE_SIZE, "Hello!");

    // Send the message to the consumer
    Serial.println("Producer: Sending message");
    xTaskNotify(consumerTaskHandle, (uint32_t)producerMessage, eSetValueWithOverwrite);

    // Wait for the reply from the consumer
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    Serial.print("Producer: Received reply: ");
    Serial.println(consumerReply);

    // Delay before sending the next message
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void consumerTask(void* pvParameters) {
  while (1) {
    // Wait for the message from the producer
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    Serial.print("Consumer: Received message: ");
    Serial.println((char*)pvParameters);

    // Process the message (e.g., change it to uppercase)
    for (int i = 0; i < MESSAGE_SIZE; i++) {
      consumerReply[i] = toupper(((char*)pvParameters)[i]);
    }

    // Send the reply to the producer
    xTaskNotify(producerTaskHandle, (uint32_t)consumerReply, eSetValueWithOverwrite);
  }
}
