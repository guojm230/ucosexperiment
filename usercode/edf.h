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

	//绝对截止时间
	uint32 abs_dead_line;
	//本次周期内剩余时间
	uint32 remain_time;
	//下个周期的时间点
	uint32 next_period;
	//对应UCOS任务的优先级
	uint8 prio;
} EDFTcb;

extern EDFTcb* EDFCur;

/*
* 初始化数据结构
*/
void EdfInit();


/*
* 将tcb添加到等待队列中，按照next_period由小到大排列
*/
void InsertWaitQueue(EDFTcb* tcb);


/*
* 获取等待队列中next_period最小的任务
* 队列为空时返回NULL
*/
EDFTcb* PeekWaitQueue();


/*
* 删除并返回等待队列中next_period最小的任务
* 队列为空时返回NULL
*/
EDFTcb* PopWaitQueue();

/**
* 插入到就绪队列中,按照abs_dead_line由小到大排列
*/
void InsertRdyQueue(EDFTcb* tcb);

/*
* 获取就绪队列中最高的任务
* 队列为空时返回NULL
*/
EDFTcb* PeekRdyQueue();

/*
* 从就绪队列中删除优先级最高的任务，如果无任务可运行，返回NULL
*/
EDFTcb* PopRdyQueue();

/**
* 将对应优先级的任务添加和移除就绪表
*/
void TaskSuspend(uint8 prio);
void TaskResume(uint8 prio);


/*
* 创建Edf调度的任务
*/
void CreateEdfTask(void (*task)(void*), uint32 ct, uint32 dead_line, uint32 period);

void TraceEdfStart(EDFTcb* task);

void TraceEdfTaskSwitch(EDFTcb* prev_task, EDFTcb* next_task);