#pragma once
#include "ucos_ii.h"
/* RM算法实现 */
//调度器优先级
#define DISPATCHER_PRIORITY 4
#define MIN(x,y) (x<=y ? x : y)
//最大支持的任务数量
#define RM_MAX_TASKS OS_MAX_TASKS

#define STACK_SIZE 1024

#define True 1
#define False 0

typedef struct rm_tcb {
	struct rm_tcb* next;
	struct rm_tcb* prev;

	INT8U task_id;

	//单位均为时间片
	INT32U period;
	INT32U computation_time;

	//当前周期内剩余需要执行的时间
	INT32U remain_time;
} RMTask;

//入口函数
void RMStart();