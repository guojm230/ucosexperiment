#pragma once
/* RM算法实现 */
//调度器优先级
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

	//单位均为时间片
	uint32 period;
	uint32 compute_time;

	//当前周期内剩余需要执行的时间
	uint32 remain_time;
	uint32 next_period;
	uint8 prio;
} RMTask;


RMTask* RMCur;

void RMStart();

/*
* 初始化数据结构
*/
void EdfInit();

/*
* 将tcb添加到等待队列中，按照next_period由小到大排列
*/
void InsertWaitQueue(RMTask* tcb);


/*
* 获取等待队列中next_period最小的任务
* 队列为空时返回NULL
*/
RMTask* PeekWaitQueue();


/*
* 删除并返回等待队列中next_period最小的任务
* 队列为空时返回NULL
*/
RMTask* PopWaitQueue();

/**
* 插入到就绪队列中,按照abs_dead_line由小到大排列
*/
void InsertRdyQueue(RMTask* tcb);

/*
* 获取就绪队列中最高的任务
* 队列为空时返回NULL
*/
RMTask* PeekRdyQueue();

/*
* 从就绪队列中删除优先级最高的任务，如果无任务可运行，返回NULL
*/
RMTask* PopRdyQueue();

/**
* 将对应优先级的任务添加和移除就绪表
*/
void TaskSuspend(uint8 prio);
void TaskResume(uint8 prio);


/*
* 创建Edf调度的任务
*/

void CreateRMTask(void (*task)(void*), uint32 ct, uint32 period);

void TraceRMStart(RMTask* task);

void TraceTaskSwitch(RMTask* prev_task, RMTask* next_task);
