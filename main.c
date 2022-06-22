#include "ucos_ii.h"
#include "usercode/INCLUDES.H"
#include <stdio.h>

#define  TASK_STK_SIZE               512       /* Size of each task's stacks (# of WORDs)            */
#define TaskStart_Prio	1
#define Task1_Prio		2





OS_STK  TaskStk[OS_MAX_TASKS][TASK_STK_SIZE];    // Tasks stacks
HANDLE mainhandle;		//���߳̾��
CONTEXT Context;		//���߳��л�������
BOOLEAN FlagEn = 1;		//����һ��ȫ�ֱ�������Ϊ�Ƿ�ʱ�ӵ��ȵı�־

void VCInit(void)
{
	HANDLE cp, ct;
	Context.ContextFlags = CONTEXT_CONTROL;
	cp = GetCurrentProcess();	//�õ���ǰ���̾��
	ct = GetCurrentThread();	//�õ���ǰ�߳�α���
	DuplicateHandle(cp, ct, cp, &mainhandle, 0, TRUE, 2);	//α���ת��,�õ��߳�����

}

int main() {
	INT32U Experiment;
	VCInit();
	printf("0.û���û�����\n");
	printf("1.��һ�����ӣ�һ���û�����\n");
	printf("2.�ڶ������ӣ�����������CPU��������\n");
	printf("3.���������ӣ�����Ĺ���ͻָ�\n");
	printf("4.���ĸ����ӣ��ź�������\n");
	printf("5.��������ӣ������ź�������\n");
	printf("6.���������ӣ��¼���־��\n");
	printf("7.���߸����ӣ���Ϣ����\n");
	printf("8.�ڰ˸����ӣ���Ϣ����\n");
	printf("9.�ھŸ����ӣ��ڴ����\n");
	printf("10.��ʮ�����ӣ��ź������ȼ���ת\n");
	printf("11.��ʮһ�����ӣ����������ȼ��̳�\n");
	printf("12.��ʮ�������ӣ��¼���־�����\n");
	printf("���������ѡ������:\n");
	scanf("%d", &Experiment);
	if ((Experiment < 0) || (Experiment > 12))
	{
		printf("��Ч������!");
		return(1);
	}
	OSInit();
	switch (Experiment)
	{
	case 1://һ����������
		OSTaskCreate(FirstTask, 0, &TaskStk[5][TASK_STK_SIZE - 1], 5);
		break;
	case 2://����������CPU
		OSTaskCreate(E2_task1, 0, &TaskStk[5][TASK_STK_SIZE - 1], 5);
		OSTaskCreate(E2_task2, 0, &TaskStk[6][TASK_STK_SIZE - 1], 6);
		break;
	case 3://����Ĺ���ͻָ�
		OSTaskCreate(E3_task0, 0, &TaskStk[5][TASK_STK_SIZE - 1], 5);
		OSTaskCreate(E3_task1, 0, &TaskStk[6][TASK_STK_SIZE - 1], 6);
		OSTaskCreate(E3_task2, 0, &TaskStk[7][TASK_STK_SIZE - 1], 7);
		break;
	case 4://�ź�����������
		OSTaskCreate(UserTaskSemA, 0, &TaskStk[5][TASK_STK_SIZE - 1], 7);
		OSTaskCreate(UserTaskSemB, 0, &TaskStk[6][TASK_STK_SIZE - 1], 6);
		OSTaskCreate(UserTaskSemC, 0, &TaskStk[7][TASK_STK_SIZE - 1], 5);
		break;
	case 5://�����ź�����������
		OSTaskCreate(TaskMutex1, 0, &TaskStk[6][TASK_STK_SIZE - 1], 6);
		OSTaskCreate(TaskMutex2, 0, &TaskStk[7][TASK_STK_SIZE - 1], 50);
		OSTaskCreate(TaskPrint, 0, &TaskStk[8][TASK_STK_SIZE - 1], 30);
		break;
	case 6://ʱ���־���������
		OSTaskCreate(TaskDataProcess, 0, &TaskStk[5][TASK_STK_SIZE - 1], 5);
		OSTaskCreate(TaskIO1, 0, &TaskStk[6][TASK_STK_SIZE - 1], 6);
		OSTaskCreate(TaskIO2, 0, &TaskStk[7][TASK_STK_SIZE - 1], 7);
		OSTaskCreate(TaskIO3, 0, &TaskStk[8][TASK_STK_SIZE - 1], 8);
		OSTaskCreate(TaskIO4, 0, &TaskStk[9][TASK_STK_SIZE - 1], 9);
		break;
	case 7://��Ϣ����
		OSTaskCreate(TaskMessageSen, 0, &TaskStk[6][TASK_STK_SIZE - 1], 6);
		OSTaskCreate(TaskMessageRec, 0, &TaskStk[7][TASK_STK_SIZE - 1], 7);
		break;
	case 8://��Ϣ����
		OSTaskCreate(TaskQSen, 0, &TaskStk[7][TASK_STK_SIZE - 1], 5);
		OSTaskCreate(TaskQRec, 0, &TaskStk[8][TASK_STK_SIZE - 1], 6);
		OSTaskCreate(TaskQRec, 0, &TaskStk[9][TASK_STK_SIZE - 1], 7);
		break;
	case 9://�ڴ����
		OSTaskCreate(TaskM, 0, &TaskStk[8][TASK_STK_SIZE - 1], 6);
		break;
	case 10: //���ȼ���ת
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