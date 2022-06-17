#include "rm.h"

#include <stdio.h>
#include <math.h>
#include "rm.h"
#include "ucos_ii.h"

//由偏移量获取某个结构体中的属性
#define GetAttribute(ptr,type,offset) *((type*)((uint8*)(ptr)+offset))
//获取结构体中某个属性的偏移量
#define GetOffset(type,name) ( (uint32)(&((type*)NULL)->name))

#define EachTasks(Var)	\
	for(RMTask* Var = RMTaskList;Var != NULL; Var = Var->next)

#define EachTasksUntil(Var,End)	\
	for(RMTask* Var = RMTaskList;Var != NULL && Var != End; Var = Var->next)

static OS_STK TaskStk[RM_MAX_TASKS][STACK_SIZE];

//用来暂时储存tcb，用于检验是否可调度
RMTask* RMTaskList;	//按照优先级排列的双向链表
RMTask* RMTaskListTail;	//尾节点

RMTask* RMCur;	//当前运行的任务
RMTask  RMTbl[RM_MAX_TASKS];

//按照next_period排序
RMTask* RMWaitQueue[RM_MAX_TASKS + 1];
//按照period排序
RMTask* RMRdyQueue[RM_MAX_TASKS + 1];

static INT32U task_count = 0;

static uint32 wait_task_count = 0;

static uint32 rdy_task_count = 0;

static void EmptyTask() {
	while (True) {

	}
}

static void Init() {
	task_count = 0;
	wait_task_count = 0;
	rdy_task_count = 0;

	RMCur = NULL;
	RMTaskList = NULL;
	RMTaskListTail = NULL;
	
	for (int i = 0; i < RM_MAX_TASKS; i++) {
		OS_MemClr((INT8U*)(RMTbl + i), sizeof(RMTask));
		RMWaitQueue[i] = NULL;
		RMRdyQueue[i] = NULL;
	}
}

/**
* 响应时间方法来验证是否可用RM算法调度
* @return Boolean 是否可用RM算法调度
*/
static BOOLEAN VerifyResponseTime() {
	RMTask* task = RMTaskList;
	while (task != NULL) {
		INT32U R = 0;	//R0
		INT32U I = 0;
		EachTasksUntil(t,task) {
			I += t->compute_time;
		}
		
		do {
			R = I + task->compute_time;
			if (R > task->period) {
				return False;
			}
			I = 0;
			EachTasksUntil(t,task) {
				I += ceil((double)R / (t->period)) * t->compute_time;
			}
		} while (I + task->compute_time > R);
		task = task->next;
	}
	return True;
}

//校验能否采用RM算法调度
static BOOLEAN VerifySchedulability() {
	//compute HBound
	RMTask* head = RMTaskList;
	double ub = 0;
	double hb = 1;
	while (head != NULL) {
		ub += head->compute_time / (double)head->period;
		hb *= head->compute_time / (double)head->period + 1;
		head = head->next;
	}
	if (ub > 1) {
		return False;
	}
	
	return ub <= 2 ||  VerifyResponseTime();
}


static void InsertList(RMTask* new_tcb) {
	RMTask* node = NULL;
	//根据优先级插入到链表对应位置
	if (RMTaskList == NULL) {
		RMTaskList = RMTaskListTail = new_tcb;
	}
	else {
		node = RMTaskList;
		while (node != NULL && new_tcb->period > node->period) {
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

void TaskSuspend(uint8 prio) {
	if (OSTCBPrioTbl[prio] == NULL)
		return;
	OS_ENTER_CRITICAL()
		OS_TCB* tcb = OSTCBPrioTbl[prio];
	uint8 y = tcb->OSTCBY;
	OSRdyTbl[y] &= (OS_PRIO)~tcb->OSTCBBitX;                   /* Make task not ready                 */
	if (OSRdyTbl[y] == 0u) {
		OSRdyGrp &= (OS_PRIO)~tcb->OSTCBBitY;
	}
	tcb->OSTCBStat |= OS_STAT_SUSPEND;
	OS_EXIT_CRITICAL()
}

void TaskResume(uint8 prio) {
	if (OSTCBPrioTbl[prio] == NULL)
		return;
	OS_ENTER_CRITICAL()
		OS_TCB* tcb = OSTCBPrioTbl[prio];
	uint8 y = tcb->OSTCBY;
	OSRdyTbl[y] |= (OS_PRIO)tcb->OSTCBBitX;                   /* Make task not ready                 */
	OSRdyGrp |= (OS_PRIO)tcb->OSTCBBitY;
	tcb->OSTCBStat &= (uint8)~(uint8)OS_STAT_SUSPEND;
	OS_EXIT_CRITICAL()
}


//从队列中弹出元素
static RMTask* PopQueue(RMTask** queue, uint32* count, uint32 offset) {
	if (*count == 0)
		return NULL;
	RMTask** result = queue[1];
	queue[1] = queue[*count];
	queue[*count] = NULL;
	(*count)--;
	if (*count > 0) {
		int i = 1;
		int child = i * 2;
		RMTask* temp = NULL;
		while (child <= *count) {
			if (child + 1 <= *count && GetAttribute(queue[child], uint32, offset) > GetAttribute(queue[child + 1], uint32, offset)) {
				child = child + 1;
			}
			if (GetAttribute(queue[i], uint32, offset) <= GetAttribute(queue[child], uint32, offset)) {
				break;
			}
			temp = queue[i];
			queue[i] = queue[child];
			queue[child] = temp;
			i = child;
			child *= 2;
		}
	}
	return result;
}

static void InsertQueue(RMTask** queue, RMTask* tcb, uint32* count, uint32 offset) {
	queue[++(*count)] = tcb;
	//adjust
	for (int i = *count; i >= 2; i /= 2) {
		if (GetAttribute(queue[i / 2], uint32, offset) <= GetAttribute(tcb, uint32, offset)) {
			break;
		}
		queue[i] = queue[i / 2];
		queue[i / 2] = tcb;
	}
}

void InsertWaitQueue(RMTask* tcb) {
	InsertQueue(RMWaitQueue, tcb, &wait_task_count, GetOffset(RMTask, next_period));
}

RMTask* PeekWaitQueue() {
	return RMWaitQueue[1];
}

RMTask* PopWaitQueue() {
	if (wait_task_count == 0)
		return NULL;
	return PopQueue(RMWaitQueue, &wait_task_count, GetOffset(RMTask, next_period));
}

void InsertRdyQueue(RMTask* tcb) {
	InsertQueue(RMRdyQueue, tcb, &rdy_task_count, GetOffset(RMTask, period));
}

RMTask* PeekRdyQueue() {
	return RMRdyQueue[1];
}

RMTask* PopRdyQueue() {
	if (rdy_task_count == 0)
		return NULL;
	return PopQueue(RMRdyQueue, &rdy_task_count, GetOffset(RMTask, period));
}

/**
* 从控制台读取任务，如果读取失败会重新进行尝试读取
*/
static void ReadTask() {
	INT32U compute_time;
	INT32U period;
	RMTask* node;
	uint32 count = 0;
	//存在多层循环的问题，直接使用goto了
read_label:
	printf("请输入任务数量(最大任务数%d):\n", RM_MAX_TASKS);
	if (scanf("%d", &count) == 0) {
		printf("请输入正确的数字.\n");
		goto read_label;
	}
	if (count <= 0 || count > RM_MAX_TASKS) {
		printf("任务数量输入错误\n");
		goto read_label;
	}
	for (int i = 0; i < count; i++) {
		printf("请输入第%d个任务的计算时间和周期时间：\n", i);
		if (scanf("%d %d", &compute_time, &period) == 0) {
			printf("任务属性读取错误\n");
			goto read_label;
		}
		if (compute_time > period) {
			printf("任务计算时间必须小于周期.\n");
			goto read_label;
		}
		CreateRMTask(EmptyTask,compute_time, period);
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
			EachTasks(task) {
				OSTaskDel(task->prio);
			}
			printf("任务集不可使用RM算法调度\n");
		}
	}
}

void CreateRMTask(void (*task)(void*), uint32 ct, uint32 period) {
	uint8 prio = MAX_PRIORITY + task_count;
	RMTask* new_tcb = RMTbl + task_count;
	new_tcb->task_id = task_count;
	new_tcb->compute_time = ct;
	new_tcb->period = period;

	new_tcb->remain_time = ct;
	new_tcb->next_period = period;
	new_tcb->prio = prio;
	InsertList(new_tcb);
	InsertRdyQueue(new_tcb);
	OSTaskCreate(task,NULL,TaskStk[task_count] + STACK_SIZE - 1, prio);
	TaskSuspend(prio);
	task_count++;
}

void TraceRMStart(RMTask* task) {
	if (task == NULL) {
		printf("系统无任务可运行\n");
	}
	else {
		printf("时间点0，初始任务:%d\n", task->task_id);
	}
}

void TraceTaskSwitch(RMTask* prev_task, RMTask* next_task) {
	if (prev_task == next_task)
		return;
	if (prev_task == NULL) {
		printf("时间点：%d,切换任务:空任务->%d\n", OSTime, next_task->task_id);
	}
	else if (next_task == NULL) {
		printf("时间点：%d,任务%d运行结束:当前无任务运行\n", OSTime, prev_task->task_id);
	}
	else {
		printf("时间点：%d,切换任务:%d->%d\n", OSTime, prev_task->task_id, next_task->task_id);
	}
}