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

RMTask* RMTaskList;	//按照优先级排列的双向循环链表
RMTask* RMTaskListTail;	//尾节点
RMTask* RMTaskCur;	//当前运行的任务
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
* 响应时间方法来验证是否可用RM算法调度
* @return Boolean 是否可用RM算法调度
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

//校验能否采用RM算法调度
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
	//根据优先级插入到链表对应位置
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
* 从控制台读取任务，如果读取失败会重新进行尝试读取
*/
static void ReadTask() {
	INT32U computation_time;
	INT32U period;
	RMTask* node;
	//存在多层循环的问题，直接使用goto了
read_label:
	printf("请输入任务数量(最大任务数%d):\n", RM_MAX_TASKS);
	if (scanf("%d", &task_count) == 0) {
		printf("请输入正确的数字.\n");
		goto read_label;
	}
	if (task_count <= 0 || task_count > RM_MAX_TASKS) {
		printf("任务数量输入错误\n");
		goto read_label;
	}
	for (int i = 0; i < task_count; i++) {
		printf("请输入第%d个任务的计算时间和周期时间：\n", i);
		if (scanf("%d %d", &computation_time, &period) == 0) {
			printf("任务属性读取错误\n");
			goto read_label;
		}
		if (computation_time > period) {
			printf("任务计算时间必须小于周期.\n");
			goto read_label;
		}
		AddNewTask(i, computation_time, period);
	}
}

/**
* 每个时间片运行一次
* 缺点，调度频繁，开销大
*/
static void ExecuteOnce(INT32U current_time) {
	RMTask* prev_task = RMTaskCur;
	if (RMTaskCur != NULL) {
		RMTaskCur->remain_time--;
		RMTaskCur = NULL;
	}
	//寻找出下一个要执行的任务
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
		printf("时间%d,当前无任务运行\n",time_count);
	}
	else if(prev_task != NULL) {
		printf("时间%d，切换任务:%d->%d\n",time_count,prev_task->task_id, RMTaskCur->task_id);
	}
	else {
		printf("时间%d，切换任务:空任务->%d\n", time_count, RMTaskCur->task_id);
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
			printf("任务集不可使用RM算法调度\n");
		}
	}

	while (True) {
		ExecuteOnce(time_count);
		OSTimeDly(1);
		time_count++;
	}
}