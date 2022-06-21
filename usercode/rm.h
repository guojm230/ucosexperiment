#pragma once
#include "ucos_ii.h"
/* RM�㷨ʵ�� */
//���������ȼ�
#define DISPATCHER_PRIORITY 4
#define MIN(x,y) (x<=y ? x : y)
//���֧�ֵ���������
#define RM_MAX_TASKS OS_MAX_TASKS

#define STACK_SIZE 1024

#define True 1
#define False 0

typedef struct rm_tcb {
	struct rm_tcb* next;
	struct rm_tcb* prev;

	INT8U task_id;

	//��λ��Ϊʱ��Ƭ
	INT32U period;
	INT32U computation_time;

	//��ǰ������ʣ����Ҫִ�е�ʱ��
	INT32U remain_time;
} RMTask;

//��ں���
void RMStart();