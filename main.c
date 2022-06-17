#include "ucos_ii.h"
#include <stdio.h>
#include "usercode/rm.h"


int main() {
	OSInit();
	RMStart();
	OSStart();
	return 0;
}