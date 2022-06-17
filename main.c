#include "ucos_ii.h"
#include <stdio.h>

#define STACK_SIZE  1024

OS_STK Stack[3][STACK_SIZE];

OS_EVENT* mutex1 = NULL;
OS_EVENT* mutex2 = NULL;

INT8U error;

static void Task1() {
	//INT8U error = 0;
	OSMutexPend(mutex1,0,&error);
	OSTimeDly(1);
	OSMutexPend(mutex2, 0, &error);
	while (OS_TRUE) {}
}

static void Task2() {
	//INT8U error = 0;
	OSMutexPend(mutex2, 0, &error);
	OSTimeDly(1);
	OSMutexPend(mutex1, 0, &error);
	while(OS_TRUE){}
}

static void Task3() {
	while (OS_TRUE) {
		printf("“—æ≠À¿À¯\n");
		OSTimeDly(100);
	}
}

int main() {
	OSInit();
	mutex1 = OSMutexCreate(2, &error);
	mutex2 = OSMutexCreate(3, &error);
	OSTaskCreate(Task1,NULL,Stack[0]+STACK_SIZE-1, 2);
	OSTaskCreate(Task2, NULL, Stack[1] + STACK_SIZE - 1, 3);
	OSTaskCreate(Task3, NULL, Stack[2] + STACK_SIZE - 1, 4);

	OSStart();
	return 0;
}