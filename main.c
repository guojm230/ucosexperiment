#include "ucos_ii.h"
#include <stdio.h>
#include "usercode/edf.h"

void EmptyTask() {
	while (1) {}
}

int main() {
	OSInit();
	uint32 task_count = 0;
	uint32 ct;
	uint32 dead_line;
	uint32 period;
input_task:
	EdfInit();
	printf("请输入任务数量:\n");
	scanf("%d", &task_count);
	for (int i = 0; i < task_count; i++) {
		printf("请输入第%d个任务的属性：(计算时间 相对截止时间 周期)\n",i);
		scanf("%d %d %d",&ct,&dead_line,&period);
		CreateEdfTask(EmptyTask,ct,dead_line, period);
	}
	if (!VerifySchedulability()) {
		printf("不可用EDF调度,请重新输入\n");
		goto input_task;
	}
	OSStart();
	return 0;
}