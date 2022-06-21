#pragma once
/* RM�㷨ʵ�� */
//���������ȼ�
#define MAX_PRIORITY 4
#define MIN(x,y) (x<=y ? x : y)

#define NULL (void*)0
#define RM_MAX_TASKS OS_MAX_TASKS
#define STACK_SIZE 1024

typedef unsigned int uint32;
typedef unsigned char uint8;

#define STACK_SIZE 1024

#define True 1
#define False 0

typedef struct rm_tcb {
	struct rm_tcb* next;
	struct rm_tcb* prev;

	uint8 task_id;

	//��λ��Ϊʱ��Ƭ
	uint32 period;
	uint32 compute_time;

	//��ǰ������ʣ����Ҫִ�е�ʱ��
	uint32 remain_time;
	uint32 next_period;
	uint8 prio;
} RMTask;


RMTask* RMCur;

void RMStart();

/*
* ��ʼ�����ݽṹ
*/
void EdfInit();

/*
* ��tcb��ӵ��ȴ������У�����next_period��С��������
*/
void InsertWaitQueue(RMTask* tcb);


/*
* ��ȡ�ȴ�������next_period��С������
* ����Ϊ��ʱ����NULL
*/
RMTask* PeekWaitQueue();


/*
* ɾ�������صȴ�������next_period��С������
* ����Ϊ��ʱ����NULL
*/
RMTask* PopWaitQueue();

/**
* ���뵽����������,����abs_dead_line��С��������
*/
void InsertRdyQueue(RMTask* tcb);

/*
* ��ȡ������������ߵ�����
* ����Ϊ��ʱ����NULL
*/
RMTask* PeekRdyQueue();

/*
* �Ӿ���������ɾ�����ȼ���ߵ������������������У�����NULL
*/
RMTask* PopRdyQueue();

/**
* ����Ӧ���ȼ���������Ӻ��Ƴ�������
*/
void TaskSuspend(uint8 prio);
void TaskResume(uint8 prio);


/*
* ����Edf���ȵ�����
*/

void CreateRMTask(void (*task)(void*), uint32 ct, uint32 period);

void TraceRMStart(RMTask* task);

void TraceTaskSwitch(RMTask* prev_task, RMTask* next_task);
