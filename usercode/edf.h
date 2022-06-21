#pragma once

#define NULL (void*)0
#define EDF_MAX_TASKS OS_MAX_TASKS

typedef unsigned int uint32;
typedef unsigned char uint8;


typedef struct edf_tcb {
	struct edf_tcb* prev;
	struct edf_tcb* next;

	uint32 task_id;
	uint32 compute_time;
	uint32 period;
	uint32 dead_line;

	//���Խ�ֹʱ��
	uint32 abs_dead_line;
	//����������ʣ��ʱ��
	uint32 remain_time;
	//�¸����ڵ�ʱ���
	uint32 next_period;
	//��ӦUCOS��������ȼ�
	uint8 prio;
} EDFTcb;

extern EDFTcb* EDFCur;

/*
* ��ʼ�����ݽṹ
*/
void EdfInit();


/*
* ��tcb��ӵ��ȴ������У�����next_period��С��������
*/
void InsertWaitQueue(EDFTcb* tcb);


/*
* ��ȡ�ȴ�������next_period��С������
* ����Ϊ��ʱ����NULL
*/
EDFTcb* PeekWaitQueue();


/*
* ɾ�������صȴ�������next_period��С������
* ����Ϊ��ʱ����NULL
*/
EDFTcb* PopWaitQueue();

/**
* ���뵽����������,����abs_dead_line��С��������
*/
void InsertRdyQueue(EDFTcb* tcb);

/*
* ��ȡ������������ߵ�����
* ����Ϊ��ʱ����NULL
*/
EDFTcb* PeekRdyQueue();

/*
* �Ӿ���������ɾ�����ȼ���ߵ������������������У�����NULL
*/
EDFTcb* PopRdyQueue();

/**
* ����Ӧ���ȼ���������Ӻ��Ƴ�������
*/
void TaskSuspend(uint8 prio);
void TaskResume(uint8 prio);


/*
* ����Edf���ȵ�����
*/
void CreateEdfTask(void (*task)(void*), uint32 ct, uint32 dead_line, uint32 period);

void TraceEdfStart(EDFTcb* task);

void TraceEdfTaskSwitch(EDFTcb* prev_task, EDFTcb* next_task);