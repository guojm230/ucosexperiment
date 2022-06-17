#include "ucos_ii.h"
#include <stdio.h>
#include "usercode/rm.h"

INT8U stack[1024];

int main() {
	OSInit();
	OSTaskCreate(RMStart,NULL,stack+1023,3);
	OSStart();
	return 0;
}