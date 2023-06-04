#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <semphr.h>
/*
// Function prototypes
static void MyTask1(void* pvParameters);
static void MyTask2(void* pvParameters);

void setup() {
  // Initialize the Serial Monitor with 9600 baud rate
  Serial.begin(9600);
  Serial.println(F("In Setup function"));
  
  // Set the digital pins 8 to 9 as digital output pins
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  // Create two tasks with labels Task1 and Task2 and assign the priority as 1 and 2 respectively.
  xTaskCreate(MyTask1, "Task1", 100, NULL, 1, NULL);
  xTaskCreate(MyTask2, "Task2", 100, NULL, 2, NULL);
}

void loop() {
  // There is no instruction in the loop section of the code.
  // Because each task executes on interrupt after specified time
}

// Task 1
static void MyTask1(void* pvParameters) {
  while(1) { 
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW); 
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

// Task 2
static void MyTask2(void* pvParameters) {
  while(1) {
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH); 
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}
*/
/*
#define QUEUE_SIZE 5
#define PRODUCER_PERIOD_MS 20

void producerTask(void* parameters);
void consumerTask(void* parameters);

// Declare the message queue
QueueHandle_t msgQueue;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  while (!Serial) {}

  // Create the message queue
  msgQueue = xQueueCreate(QUEUE_SIZE, sizeof(int));

  // Check if message queue creation was successful
  if (msgQueue == NULL) {
    Serial.println("Failed to create message queue!");
    while (1) {}
  }

  // Create the producer task with higher priority
  xTaskCreate(producerTask, "Producer", 128, NULL, 2, NULL);

  // Create the consumer task with lower priority
  xTaskCreate(consumerTask, "Consumer", 128, NULL, 1, NULL);
}

void loop() {
  // Empty loop
}

void producerTask(void* parameters) {
  int message = 0;

  while (1) {
    // Send a message to the queue
    xQueueSend(msgQueue, &message, 0);

    // Increment the message for the next iteration
    message++;

    // Wait for the specified period
    vTaskDelay(PRODUCER_PERIOD_MS / portTICK_PERIOD_MS);
  }
}

void consumerTask(void* parameters) {
  int message;

  while (1) {
    // Wait until a message is available in the queue
    xQueueReceive(msgQueue, &message, portMAX_DELAY);

    // Process the message (in this case, just print it to serial)
    Serial.print("Received message: ");
    Serial.println(message);
  }
}
*/

SemaphoreHandle_t xSerialSemaphore;

// Function prototypes
static void MyTask1(void* pvParameters);
static void MyTask2(void* pvParameters);

void setup() {
  // Initialize the Serial Monitor with 9600 baud rate
  Serial.begin(9600);
  Serial.println(F("In Setup function"));
  
  // Set the digital pins 8 to 9 as digital output pins
  
  // Create a semaphore for Serial print
  xSerialSemaphore = xSemaphoreCreateMutex();
  
  // Create two tasks with labels Task1 and Task2 and assign the priority as 1 and 2 respectively.
  xTaskCreate(MyTask1, "Task1", 100, NULL, 1, NULL);
  xTaskCreate(MyTask2, "Task2", 100, NULL, 1, NULL);
}

void loop() {}

// Task 1
static void MyTask1(void* pvParameters) {
  while(1) { 
   for (int i = 1; i < 50; i++){
    xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
    Serial.println("a");
    xSemaphoreGive(xSerialSemaphore);
   }
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}

// Task 2
static void MyTask2(void* pvParameters) {
  while(1) { 
   for (int i = 0; i < 50; i++){
    xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
    Serial.println("b");
    xSemaphoreGive(xSerialSemaphore);
   }
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}


