#include <krnl.h>

const int STKSZ = 200;

#define LED1 8
#define LED2 9

struct k_t *pt1,  // pointer to hold reference
  *pt2;           // to taskdescriptor for t1 and t2

char stak1[STKSZ], stak2[STKSZ];

void alg01() {}  // for time being empty

char dataBufForMsgQ[11*10];

struct k_msg_t *msgQ; // Msg queue struct

int loopNr1 = 0;
int loopNr2 = 0;

void aktuer01() {
  Serial.print(",");
  Serial.print("aktuer1:");
  Serial.println(loopNr1);
  loopNr1 = loopNr1 + 1;
}

void aktuer02() {
  Serial.print("aktuer2:");
  Serial.println(loopNr2);
  loopNr2 = loopNr2 + 1;
}

// Assignment 1 tasks
void t1(void) {

  while (1) {
    k_sleep(50);
    k_eat_msec(5000);
    //aktuer01();
    k_breakout();
  }
}

void t2() {
  while (1) {
    digitalWrite(12, !digitalRead(13));
    //Serial.println(!digitalRead(13));
    //aktuer02();
    k_sleep(100);
  }
}

// Assignment 3 tasks
void producerTask(void) {
  char res;
  char message[11] ="Hello world";
  
  while (1) {
    for (int i = 0; i < 11; i++){
      res = k_send(msgQ, &message[i]);
    }
    k_sleep(2000);
    //aktuer01();
    k_breakout();
  }
}

void consumerTask() {
  char res;
  char temp;
  char message[11];
  int lostM;  
  int pos = 0;
  while (1) {
    digitalWrite(12, !digitalRead(13));
    res = k_receive(msgQ,  &temp, 0, &lostM);
      // Successfully received a message
      message[pos] = temp;
      pos++;

      if (pos == 11) {
        // Received entire message
        Serial.println(message);
        pos = 0; // Reset position for next message
      }    

    k_clear_msg_Q(msgQ);
    //aktuer02();
    //k_sleep(100);

  }
}  

void setup() {

  Serial.begin(115200);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);  

  // init krnl
  k_init(2, 0, 1);
  //  length of message v, byte size v, 
  msgQ = k_crt_send_Q (11, sizeof(char),  dataBufForMsgQ);


  //               |------------ function used for body code for task
  //               |  |--------- priority (lower number= higher prio
  //               |  |   |----- staksize for array s1
  //                         |-- array used for stak
  pt1 = k_crt_task(producerTask, 5, stak1, STKSZ);
  pt2 = k_crt_task(consumerTask, 10, stak2, STKSZ);


  // NB-1 remember an Arduino has only 2-8 kByte RAM
  // NB-2 remember that stak is used in function calls for
  //  - return address
  //  - registers stakked
  //  - local variabels in a function
  //  So having 200 Bytes of stak excludes a local variable like ...
  //    int arr[400];
  // krnl call k_unused_stak returns size of unused stak
  // Both task has same priority so krnl will shift between the
  // tasks every 10 milli second (speed set in k_start)

  k_start();  // start kernel with tick speed 1 milli seconds
}

void loop() {
  /* loop will never be called */
}



extern "C" {

	void k_breakout() // called every task shift from dispatcher
	{
		if (pRun->nr == 0)  // 0 is dummy task - the eater of excessive CPU when all user tasks are idling
		{
			PORTB = PORTB | B11011111;  // led13 (bit 5) on let the rest be untouched
		}
		else {
			PORTB = PORTB & B00100000;  // led13 off uno
		}
	}

}

