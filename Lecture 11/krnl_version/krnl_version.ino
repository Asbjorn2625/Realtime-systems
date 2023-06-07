//220216
#include <krnl.h>

struct k_t *p1, *p2 , *s1, *s2;
unsigned long starttime1, starttime2;

void task1()
{	
	k_set_sem_timer(s1, 500);
	
	while (1) {
    k_wait(s1, 600);
    unsigned long current_time1 = millis();
    Serial.print("Task1 -> Time since last run: ");
    Serial.println(current_time1-starttime1);
		k_eat_msec(200);
    current_time1 = millis();
    Serial.print("Task1 -> Time to finish run: ");
    Serial.println(current_time1-starttime1);
    starttime1 = millis();
	}
}


void task2()
{
  k_set_sem_timer(s2, 800);
	while (1) {
    k_wait(s2, 500);
    unsigned long current_time2 = millis();
    Serial.print("Task2 -> Time since last run: ");
    Serial.println(current_time2-starttime2);
		k_eat_msec(200);
    current_time2 = millis();
    Serial.print("Task2 -> Time to finish run: ");
    Serial.println(current_time2-starttime2);
    starttime2 = millis();
	}
}


#define STK 110
char a1[STK], a2[STK];

void setup()
{
	int res;
	Serial.begin(9600);
	
	k_init(2, 2, 0); // init with space for two tasks
	
	// priority low number higher priority than higher number
	p1 = k_crt_task(task1, 10, a1, STK);
	p2 = k_crt_task(task2, 11, a2, STK);
	
	s1 = k_crt_sem(1, 1); // crt sem
  s2 = k_crt_sem(1, 1); // crt sem
	
  starttime1 = millis();
  starttime2 = millis();

	res = k_start(); // 1 milli sec tick speed
	// you will never return from k_start
	Serial.print("ups an error occured: "); Serial.println(res);
	while (1) ;
}

// TRY TO SET PRIORITIES EQUAL
// TRY TO SET task2 higher thatn t1 (priority) stil at right time in task1 ?

void loop() {}


extern "C" {
	void k_breakout() // called every task shift from dispatcher
	{
		PORTB = (1 << pRun->nr);
		// on MEGA use PORTA
	}
}
