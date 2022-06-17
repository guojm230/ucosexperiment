#include "ucos_ii.h"
#include "usercode/INCLUDES.H"
#include <stdio.h>

#define  TASK_STK_SIZE               512       /* Size of each task's stacks (# of WORDs)            */
#define TaskStart_Prio	1
#define Task1_Prio		2





OS_STK  TaskStk[OS_MAX_TASKS][TASK_STK_SIZE];    // Tasks stacks
HANDLE mainhandle;		//主线程句柄
CONTEXT Context;		//主线程切换上下文
BOOLEAN FlagEn = 1;		//增加一个全局变量，做为是否时钟调度的标志

void VCInit(void)
{
	HANDLE cp, ct;
	Context.ContextFlags = CONTEXT_CONTROL;
	cp = GetCurrentProcess();	//得到当前进程句柄
	ct = GetCurrentThread();	//得到当前线程伪句柄
	DuplicateHandle(cp, ct, cp, &mainhandle, 0, TRUE, 2);	//伪句柄转换,得到线程真句柄

}

int main() {
	INT32U Experiment;
	VCInit();
	printf("0.没有用户任务\n");
	printf("1.第一个例子，一个用户任务\n");
	printf("2.第二个例子，两个任务共享CPU交替运行\n");
	printf("3.第三个例子，任务的挂起和恢复\n");
	printf("4.第四个例子，信号量管理\n");
	printf("5.第五个例子，互斥信号量管理\n");
	printf("6.第六个例子，事件标志组\n");
	printf("7.第七个例子，消息邮箱\n");
	printf("8.第八个例子，消息队列\n");
	printf("9.第九个例子，内存管理\n");
	printf("10.第十个例子，信号量优先级翻转\n");
	printf("11.第十一个例子，互斥量优先级继承\n");
	printf("12.第十二个例子，事件标志组测试\n");
	printf("请输入序号选择例子:\n");
	scanf("%d", &Experiment);
	if ((Experiment < 0) || (Experiment > 12))
	{
		printf("无效的输入!");
		return(1);
	}
	OSInit();
	switch (Experiment)
	{
	case 1://一个任务运行
		OSTaskCreate(FirstTask, 0, &TaskStk[5][TASK_STK_SIZE - 1], 5);
		break;
	case 2://两个任务共享CPU
		OSTaskCreate(E2_task1, 0, &TaskStk[5][TASK_STK_SIZE - 1], 5);
		OSTaskCreate(E2_task2, 0, &TaskStk[6][TASK_STK_SIZE - 1], 6);
		break;
	case 3://任务的挂起和恢复
		OSTaskCreate(E3_task0, 0, &TaskStk[5][TASK_STK_SIZE - 1], 5);
		OSTaskCreate(E3_task1, 0, &TaskStk[6][TASK_STK_SIZE - 1], 6);
		OSTaskCreate(E3_task2, 0, &TaskStk[7][TASK_STK_SIZE - 1], 7);
		break;
	case 4://信号量管理例程
		OSTaskCreate(UserTaskSemA, 0, &TaskStk[5][TASK_STK_SIZE - 1], 7);
		OSTaskCreate(UserTaskSemB, 0, &TaskStk[6][TASK_STK_SIZE - 1], 6);
		OSTaskCreate(UserTaskSemC, 0, &TaskStk[7][TASK_STK_SIZE - 1], 5);
		break;
	case 5://互斥信号量管理例程
		OSTaskCreate(TaskMutex1, 0, &TaskStk[6][TASK_STK_SIZE - 1], 6);
		OSTaskCreate(TaskMutex2, 0, &TaskStk[7][TASK_STK_SIZE - 1], 50);
		OSTaskCreate(TaskPrint, 0, &TaskStk[8][TASK_STK_SIZE - 1], 30);
		break;
	case 6://时间标志组管理例程
		OSTaskCreate(TaskDataProcess, 0, &TaskStk[5][TASK_STK_SIZE - 1], 5);
		OSTaskCreate(TaskIO1, 0, &TaskStk[6][TASK_STK_SIZE - 1], 6);
		OSTaskCreate(TaskIO2, 0, &TaskStk[7][TASK_STK_SIZE - 1], 7);
		OSTaskCreate(TaskIO3, 0, &TaskStk[8][TASK_STK_SIZE - 1], 8);
		OSTaskCreate(TaskIO4, 0, &TaskStk[9][TASK_STK_SIZE - 1], 9);
		break;
	case 7://消息邮箱
		OSTaskCreate(TaskMessageSen, 0, &TaskStk[6][TASK_STK_SIZE - 1], 6);
		OSTaskCreate(TaskMessageRec, 0, &TaskStk[7][TASK_STK_SIZE - 1], 7);
		break;
	case 8://消息队列
		OSTaskCreate(TaskQSen, 0, &TaskStk[7][TASK_STK_SIZE - 1], 5);
		OSTaskCreate(TaskQRec, 0, &TaskStk[8][TASK_STK_SIZE - 1], 6);
		OSTaskCreate(TaskQRec, 0, &TaskStk[9][TASK_STK_SIZE - 1], 7);
		break;
	case 9://内存管理
		OSTaskCreate(TaskM, 0, &TaskStk[8][TASK_STK_SIZE - 1], 6);
		break;
	case 10: //优先级翻转
		OSTaskCreate(MySemTaskA, 0, &TaskStk[10][TASK_STK_SIZE - 1], 5);
		OSTaskCreate(MySemTaskB, 0, &TaskStk[7][TASK_STK_SIZE - 1], 7);
		OSTaskCreate(MySemTaskC, 0, &TaskStk[5][TASK_STK_SIZE - 1], 10);
		OSTaskCreate(MyTaskD, 0, &TaskStk[8][TASK_STK_SIZE - 1], 8);
		break;
	case 11: //Mutex
		OSTaskCreate(MyMutexTaskA, 0, &TaskStk[10][TASK_STK_SIZE - 1], 5);
		OSTaskCreate(MyMutexTaskB, 0, &TaskStk[7][TASK_STK_SIZE - 1], 7);
		OSTaskCreate(MyMutexTaskC, 0, &TaskStk[5][TASK_STK_SIZE - 1], 10);
		OSTaskCreate(MyTaskD, 0, &TaskStk[8][TASK_STK_SIZE - 1], 8);
		break;
	case 12:
		OSTaskCreate(FlagsTaskA,NULL, &TaskStk[10][TASK_STK_SIZE - 1],10);
		OSTaskCreate(FlagsTaskB, NULL, &TaskStk[9][TASK_STK_SIZE - 1],9);
		OSTaskCreate(FlagsTaskC, NULL, &TaskStk[8][TASK_STK_SIZE - 1], 8);
		OSTaskCreate(FlagsTaskD, NULL, &TaskStk[7][TASK_STK_SIZE - 1], 7);
		OSTaskCreate(FlagsTaskE, NULL, &TaskStk[6][TASK_STK_SIZE - 1], 6);
		break;
	default:
		;
	}
	OSStart();
	return 0;
}