#include "rm.h"

#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <assert.h>
#include "rm.h"

#define EachTasks(Var)	\
	for(RMTask* Var = RMTaskList;Var != NULL; Var = Var->next)

#define EachTasksUntil(Var,End)	\
	for(RMTask* Var = RMTaskList;Var != NULL && Var != End; Var = Var->next)

static OS_STK  TaskStk[OS_MAX_TASKS][STACK_SIZE];

RMTask* RMTaskList;	//�������ȼ����е�˫��ѭ������
RMTask* RMTaskListTail;	//β�ڵ�
RMTask* RMTaskCur;	//��ǰ���е�����
RMTask  RMTCBTbl[RM_MAX_TASKS];

static INT32U time_count = 0;

static INT32U task_count = 0;

static INT32U major_cycle = 0;

static void EmptyTask() {
	while(True){}
}

static void Init() {
	RMTaskCur = NULL;
	RMTaskList = NULL;
	RMTaskListTail = NULL;
	for (int i = 0; i < RM_MAX_TASKS; i++) {
		OS_MemClr((INT8U*)(RMTCBTbl + i), sizeof(RMTask));
	}
}

/**
* ��Ӧʱ�䷽������֤�Ƿ����RM�㷨����
* @return Boolean �Ƿ����RM�㷨����
*/
static BOOLEAN VerifyResponseTime() {
	RMTask* task = RMTaskList;
	while (task != NULL) {
		INT32U R = 0;	//R0
		INT32U I = 0;
		EachTasksUntil(t,task) {
			I += t->computation_time;
		}
		
		do {
			R = I + task->computation_time;
			if (R > task->period) {
				return False;
			}
			I = 0;
			EachTasksUntil(t,task) {
				I += ceil((double)R / (t->period)) * t->computation_time;
			}
		} while (I + task->computation_time > R);
		task = task->next;
	}
	return True;
}

//У���ܷ����RM�㷨����
static BOOLEAN VerifySchedulability() {
	//compute HBound
	RMTask* head = RMTaskList;
	double ub = 0;
	double hb = 1;
	while (head != NULL) {
		ub += head->computation_time / (double)head->period;
		hb *= head->computation_time / (double)head->period + 1;
		head = head->next;
	}
	if (ub > 1) {
		return False;
	}
	
	return ub <= 2 ||  VerifyResponseTime();
}

static void AddNewTask(INT32U task_id,INT32U computation_time,INT32U period) {
	RMTask* node = NULL;
	RMTask* new_tcb = RMTCBTbl + task_id;
	new_tcb->task_id = task_id;
	new_tcb->computation_time = computation_time;
	new_tcb->period = period;
	new_tcb->remain_time = computation_time;
	//�������ȼ����뵽�����Ӧλ��
	if (RMTaskList == NULL) {
		RMTaskList = RMTaskListTail = new_tcb;
	}
	else {
		node = RMTaskList;
		while (node != NULL && period > node->period) {
			node = node->next;
		}
		if (node == NULL) {
			new_tcb->prev = RMTaskListTail;
			RMTaskListTail->next = new_tcb;
			RMTaskListTail = new_tcb;
		}
		else {
			new_tcb->next = node;
			new_tcb->prev = node->prev;
			node->prev = new_tcb;
			if (new_tcb->prev == NULL) {
				RMTaskList = new_tcb;
			}
			else {
				new_tcb->prev->next = new_tcb;
			}
		}
	}
}

/**
* �ӿ���̨��ȡ���������ȡʧ�ܻ����½��г��Զ�ȡ
*/
static void ReadTask() {
	INT32U computation_time;
	INT32U period;
	RMTask* node;
	//���ڶ��ѭ�������⣬ֱ��ʹ��goto��
read_label:
	printf("��������������(���������%d):\n", RM_MAX_TASKS);
	if (scanf("%d", &task_count) == 0) {
		printf("��������ȷ������.\n");
		goto read_label;
	}
	if (task_count <= 0 || task_count > RM_MAX_TASKS) {
		printf("���������������\n");
		goto read_label;
	}
	for (int i = 0; i < task_count; i++) {
		printf("�������%d������ļ���ʱ�������ʱ�䣺\n", i);
		if (scanf("%d %d", &computation_time, &period) == 0) {
			printf("�������Զ�ȡ����\n");
			goto read_label;
		}
		if (computation_time > period) {
			printf("�������ʱ�����С������.\n");
			goto read_label;
		}
		AddNewTask(i, computation_time, period);
	}
}

/**
* ÿ��ʱ��Ƭ����һ��
* ȱ�㣬����Ƶ����������
*/
static void ExecuteOnce(INT32U current_time) {
	RMTask* prev_task = RMTaskCur;
	if (RMTaskCur != NULL) {
		RMTaskCur->remain_time--;
		RMTaskCur = NULL;
	}
	//Ѱ�ҳ���һ��Ҫִ�е�����
	EachTasks(head) {
		if (head->remain_time > 0 && RMTaskCur == NULL) {
			RMTaskCur = head;
		}

		if(time_count%head->period == 0) {
			head->remain_time = head->computation_time;
			if (RMTaskCur == NULL)
				RMTaskCur = head;
		}
	}
	if (prev_task == RMTaskCur)
		return;
	if (RMTaskCur == NULL) {
		printf("ʱ��%d,��ǰ����������\n",time_count);
	}
	else if(prev_task != NULL) {
		printf("ʱ��%d���л�����:%d->%d\n",time_count,prev_task->task_id, RMTaskCur->task_id);
	}
	else {
		printf("ʱ��%d���л�����:������->%d\n", time_count, RMTaskCur->task_id);
	}
}



void RMStart() {
	while (True) {
		Init();
		ReadTask();
		if (VerifySchedulability()) {
			break;
		}
		else {
			printf("���񼯲���ʹ��RM�㷨����\n");
		}
	}

	while (True) {
		ExecuteOnce(time_count);
		OSTimeDly(1);
		time_count++;
	}
}