#include "ucos_ii.h"
#include <stdio.h>
#include "usercode/edf.h"

void EmptyTask() {
	while (1) {}
}

int main() {
	OSInit();
	EdfInit();
	uint32 task_count = 0;
	uint32 ct;
	uint32 dead_line;
	uint32 period;
	printf("��������������:\n");
	scanf("%d", &task_count);
	for (int i = 0; i < task_count; i++) {
		printf("�������%d����������ԣ�(����ʱ�� ��Խ�ֹʱ�� ����)\n",i);
		scanf("%d %d %d",&ct,&dead_line,&period);
		CreateEdfTask(EmptyTask,ct,dead_line, period);
	}
	VerifySchedulability();
	OSStart();
	return 0;
}