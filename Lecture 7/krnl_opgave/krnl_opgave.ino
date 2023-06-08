//220216
#include <krnl.h>

//////////// TWO TASKS WITH BREAKOUT ///////////////
/*
const int STKSZ = 200;

struct k_t *pt1,  // pointer to hold reference
  *pt2;           // to taskdescriptor for t1 and t2

char stak1[STKSZ], stak2[STKSZ];

void t1(void) {
  while (1) {
    k_sleep(10);
    k_eat_msec(5); // you can't really see the LED without this
  }
}



void t2() {
  while (1) {
    k_sleep(50); 
    k_eat_msec(10); // you can't really see the LED without this
  }
}

void setup() {

  Serial.begin(9600);
  pinMode(13, OUTPUT);

  // init krnl so you can create 2 tasks, no semaphores and no message queues
  k_init(2, 0, 0);

  //               |------------ function used for body code for task
  //               |  |--------- priority (lower number= higher prio)
  //               |  |   |----- staksize for array s1
  //                         |-- array used for stak
  pt1 = k_crt_task(t1, 9, stak1, STKSZ);
  pt2 = k_crt_task(t2, 10, stak2, STKSZ);
  k_start();  // start kernel with tick speed 1 milli seconds
}

void loop() {
}



extern "C" {
	void k_breakout() // called every task shift from dispatcher
	{
		if (pRun->nr == 0)  // 0 is dummy task - the eater of excessive CPU when all user tasks are idling
		{
			PORTB = PORTB | B00100000;  // led13 (bit 5) on let the rest be untouched
		}
		else {
			PORTB = PORTB & B11011111;  // led13 off uno
		}
	}

}*/

/*
//////////// MSGQ ///////////////

#include <krnl.h>

#define STK 150
char a1[STK], a2[STK];

struct k_t * pTask1, *pTask2;

struct k_msg_t *msgQ;

int dataBufForMsgQ[10]; // 10 ints..

void task1()  // Producer task
{
	int i = 0;
	
	while (1) {
		
		i++;
		k_send(msgQ, &i);
		
		k_sleep(20);  // Periodically runs every 20ms
		
	}
}

void task2()  // Consumer task
{
	char res;
	int ii, lostM;
	
	while (1) {
		
		res = k_receive(msgQ,  &ii, 0, &lostM);
		
		if (res >= 0) {
			// We've received a message, print it.
			Serial.print("Received: "); 
			Serial.println(ii);
		}
	}
}

void setup() {
	Serial.begin(9600);
	delay(2000);
	
	k_init(2, 0, 1); // 2 tasks, 0 semaphores, 1 message queue 

	msgQ = k_crt_send_Q (10, sizeof(int),  dataBufForMsgQ);  // 10 elements of size
	
	pTask1 = k_crt_task(task1, 10, a1, STK); // Producer task with higher priority
	pTask2 = k_crt_task(task2, 15 , a2, STK); // Consumer task with lower priority
	
	k_start();
	
}

void loop() {}
*/

/*
//////////// PRINT WITHOUT SEMAPHORES ///////////////
// This does not work perfectly as 1 runs before 2 is done writing 'b' -> giving such a print "23b\n 1a"
#include <krnl.h>

const int STKSZ = 200;

struct k_t *pt1, *pt2;

char stak1[STKSZ], stak2[STKSZ];


void t1(void) {
  while (1) {
   // digitalWrite(12, !digitalRead(13));
    k_sleep(30); // Go every 30 ms
    for(int i=0; i<50; i++){
      Serial.print(i+1);
      Serial.println("a");
    }
  }
}



void t2() {
  while (1) {
   // digitalWrite(12, !digitalRead(13));
    k_sleep(30); // Go every 30 ms
    for(int i=0; i<50; i++){
      Serial.print(i+1);
      Serial.println("b");
    }
  }
}

void setup() {

  Serial.begin(9600);
  pinMode(13, OUTPUT);

  // init krnl so you can create 2 tasks, no semaphores and no message queues
  k_init(2, 0, 0);

  //               |------------ function used for body code for task
  //               |  |--------- priority (lower number= higher prio
  //               |  |   |----- staksize for array s1
  //                         |-- array used for stak
  pt1 = k_crt_task(t1, 10, stak1, STKSZ);
  pt2 = k_crt_task(t2, 11, stak2, STKSZ);

  k_start();
}

void loop() {
}
*/

////////////// PRINT WITH SEMAPHORES ///////////////////
// This does not work perfectly as they use too much time to run every 30ms
#include <krnl.h>

const int STKSZ = 200;

struct k_t *pt1, *pt2, *mutSem;

char stak1[STKSZ], stak2[STKSZ];


void t1(void) {
  while (1) {
   // digitalWrite(12, !digitalRead(13));
    k_sleep(30); // Go every 30 ms
    k_wait(mutSem, 0); // wait for semaphore
    for(int i=0; i<50; i++){
      Serial.print(i+1);
      Serial.println("a");
    }
    k_signal(mutSem); // release semaphore
  }
}



void t2() {
  while (1) {
   // digitalWrite(12, !digitalRead(13));
    k_sleep(30); // Go every 30 ms
    k_wait(mutSem, 0); // wait for semaphore
    for(int i=0; i<50; i++){
      Serial.print(i+1);
      Serial.println("b");
    }
    k_signal(mutSem); // release semaphore
  }
}

void setup() {

  Serial.begin(9600);
  pinMode(13, OUTPUT);

  // init krnl so you can create 2 tasks, 1 semaphore and no message queues
  k_init(2, 1, 0);
  
  // create semaphore for mutual exclusion
  mutSem = k_crt_sem(1, 1); // Initial value 1, max value 1

  //               |------------ function used for body code for task
  //               |  |--------- priority (lower number= higher prio
  //               |  |   |----- staksize for array s1
  //                         |-- array used for stak
  pt1 = k_crt_task(t1, 10, stak1, STKSZ);
  pt2 = k_crt_task(t2, 11, stak2, STKSZ);

  k_start();
}

void loop() {
}
