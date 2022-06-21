#include "edf.h"
#include "ucos_ii.h"
//由偏移量获取某个结构体中的属性
#define GetAttribute(ptr,type,offset) *((type*)((uint8*)(ptr)+offset))
//获取结构体中某个属性的偏移量
#define GetOffset(type,name) ( (uint32)(&((type*)NULL)->name))

#define STACK_SIZE 1024

EDFTcb* EDFFreeList;
EDFTcb* EDFCur;

EDFTcb* EDFWaitQueue[EDF_MAX_TASKS+1];
EDFTcb* EDFRdyQueue[EDF_MAX_TASKS+1];

EDFTcb EDFTbl[EDF_MAX_TASKS];

uint8 Stacks[EDF_MAX_TASKS][1024];

static int task_count = 0;

static int wait_task_count = 0;
static int rdy_task_count = 0;

void EdfInit() {
	//clear
	EDFCur = NULL;
	for (int i = 0; i < EDF_MAX_TASKS; i++) {
		OS_MemClr(EDFTbl + i, sizeof(EDFTcb));
		EDFWaitQueue[i] = NULL;
		EDFRdyQueue[i] = NULL;
	}
	//set
	EDFFreeList = EDFTbl;
	for (int i = 0; i < EDF_MAX_TASKS - 1; i++) {
		EDFTbl[i].next = EDFTbl + i + 1;
	}
}

//寻找可用的优先级
static int findPrio() {
	static int priority = OS_N_SYS_TASKS + 1;
	for (; priority < OS_LOWEST_PRIO + 1;priority++) {
		if (OSTCBPrioTbl[priority] == NULL)
			return priority;
	}
	return -1;
}

//从队列中弹出元素
static EDFTcb* PopQueue(EDFTcb** queue,uint32* count,uint32 offset) {
	if (*count == 0)
		return NULL;
	EDFTcb** result = queue[1];
	queue[1] = queue[*count];
	queue[*count] = NULL;
	(*count)--;
	if (*count > 0) {
		int i = 1;
		int child = i * 2;
		EDFTcb* temp = NULL;
		while (child <= *count) {
			if (child + 1 <= *count && GetAttribute(queue[child],uint32,offset) > GetAttribute(queue[child + 1],uint32,offset)) {
				child = child + 1;
			}
			if (GetAttribute(queue[i], uint32, offset) <= GetAttribute(queue[child],uint32,offset)) {
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

static void InsertQueue(EDFTcb** queue,EDFTcb* tcb, uint32* count, uint32 offset) {
	queue[++(*count)] = tcb;
	//adjust
	for (int i = *count; i >= 2; i /= 2) {
		if (GetAttribute(queue[i / 2],uint32,offset) <= GetAttribute(tcb,uint32,offset)) {
			break;
		}
		queue[i] = queue[i / 2];
		queue[i / 2] = tcb;
	}
}

void InsertWaitQueue(EDFTcb* tcb) {
	InsertQueue(EDFWaitQueue, tcb, &wait_task_count, GetOffset(EDFTcb,next_period));
}

EDFTcb* PeekWaitQueue() {
	return EDFWaitQueue[1];
}

EDFTcb* PopWaitQueue() {
	if (wait_task_count == 0)
		return NULL;
	return PopQueue(EDFWaitQueue,&wait_task_count,GetOffset(EDFTcb,next_period));
}

void InsertRdyQueue(EDFTcb* tcb) {
	InsertQueue(EDFRdyQueue,tcb,&rdy_task_count,GetOffset(EDFTcb,abs_dead_line));
}

EDFTcb* PeekRdyQueue() {
	return EDFRdyQueue[1];
}

EDFTcb* PopRdyQueue() {
	if (rdy_task_count == 0)
		return NULL;
	return PopQueue(EDFRdyQueue, &rdy_task_count, GetOffset(EDFTcb, abs_dead_line));
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


void CreateEdfTask(void (*task)(void*), uint32 ct, uint32 dead_line, uint32 period) {
	if (EDFFreeList == NULL) {
		printf("error:EDF任务已达最大任务数量");
		return;
	}

	int prio = findPrio();
	if (prio == -1) {
		printf("error:找不到可用优先级");
		return;
	}

	EDFTcb* tcb = EDFFreeList;
	EDFFreeList = EDFFreeList->next;

	tcb->task_id = task_count;
	tcb->dead_line = dead_line;
	tcb->period = period;
	tcb->compute_time = ct;
	tcb->remain_time = ct;
	tcb->prio = prio;

	//第一次截止时间
	tcb->abs_dead_line = OSTime + dead_line;
	tcb->prev = tcb->next = NULL;
	tcb->next_period = OSTime + period;

	InsertRdyQueue(tcb);
	OSTaskCreateExt(task, NULL, Stacks[task_count] + STACK_SIZE - 1, prio, task_count, Stacks[task_count], STACK_SIZE, tcb, NULL);
	task_count++;
	TaskSuspend(prio);
}


void TraceEdfStart(EDFTcb* task) {
	if (task == NULL) {
		printf("系统无任务可运行\n");
	}
	else {
		printf("时间点0，初始任务:%d\n",task->task_id);
	}
}

void TraceEdfTaskSwitch(EDFTcb* prev_task, EDFTcb* next_task) {
	if (prev_task == next_task)
		return;
	if (prev_task == NULL) {
		printf("时间点：%d,切换任务:空任务->%d\n", OSTime, next_task->task_id);
	} else if (next_task == NULL) {
		printf("时间点：%d,任务%d运行结束:当前无任务运行\n", OSTime, prev_task->task_id);
	}
	else {
		printf("时间点：%d,切换任务:%d->%d\n", OSTime,prev_task->task_id, next_task->task_id);
	}
}