/*
   Example of a FreeRTOS mutex
   https://www.freertos.org/Real-time-embedded-RTOS-mutexes.html
*/

// Include Arduino FreeRTOS library
#include <Arduino_FreeRTOS.h>


// Include mutex support
#include <semphr.h>

/*
   Declaring a global variable of type SemaphoreHandle_t

*/
SemaphoreHandle_t mutex;

unsigned long globalCount = 0;
unsigned long distanceTraveled= 0;
int calib = 12

void setup() {

  Serial.begin(9600);

  /**
       Create a mutex.
       https://www.freertos.org/CreateMutex.html
  */
  mutex = xSemaphoreCreateMutex();
  if (mutex != NULL) {
    Serial.println("Mutex created");
  }

  /**
     Create tasks
  */

  xTaskCreate(cumDistance, "Cummulative Distance", 128, 1000, 0, NULL);
  xTaskCreate(Counter, "Cummulative Distance", 128, 1000, 0, NULL);

}

void loop() {}

void cumDistance(void *pvParameters)
{
  for (;;)
  {
    
    DistanceTraveled = globalcount*calib
    globalcount = 0
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void Counter(void *pvParameters)
{
  for (;;)
  {
    globalCount++
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}