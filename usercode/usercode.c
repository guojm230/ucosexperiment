/*	 
*	usercode.c
*	the user task for ucosII
*	
*
*	
*	Last modified:	 2012-04-25 
*   writen by¬����
*   for book:Ƕ��ʽʵʱ����ϵͳ��COS������ʵ��
*   2011-2012�ڵ��ӿƼ���ѧ
*   
*   
*/

#include   "INCLUDES.h"

extern BOOLEAN FlagEn;

int a,b,c;
char * buffer;
/*ʵ��ʹ�õĴ���1*/
int FirstTask(void *pParam)
{
    int i,j=0;
	OSTimeDly(100);//��Ӱ��ͳ�������ʼ��ʱ��ȡ���Ŀ��м���ֵ
		printf("welcome to embeded system\n");
		printf("welcome to ucos\n");
		printf("This is a simple task for experiment 1!\n");

	for(;;)
	{
        for(i=0;i<99999999;i++);
		printf("������ʱ��j=%d\n",j++);
	}
	return(0); 
}
/*ʵ��ʹ�õĴ���2*/
void E2_task1(void *pParam)
{
    int i=0;
	OSTimeDly(100);
    printf("�������������У�E2task1,���ȼ�%d\n",OSPrioCur);
	for(;;)
	{
		printf("���ȼ�Ϊ%d���������%d\n",OSPrioCur,i++);
		OSTimeDly(100);
	}
}

void E2_task2(void *pParam)
{
    int i=0;
	OSTimeDly(100);
    printf("�������������У�E2task2,���ȼ�%d\n",OSPrioCur);
	for(;;)
	{
		printf("���ȼ�Ϊ%d���������%d\n",OSPrioCur,i++);
		OSTimeDly(100);
	}
}

/*ʵ��ʹ�õĴ���3*/
void E3_task0(void *pParam)
{
    int i=0;
	OSTimeDly(100);
	printf("����0���ȼ�%d,�������ͻָ��ɼ���������ݴ�������\n",OSPrioCur);
	for(;;)
	{
		printf("����0��������1,�ָ�����2������ʱ��1��\n");
        OSTaskSuspend(6);
        OSTaskResume(7);
		OSTimeDly(100);

		printf("����0��������2,�ָ�����1������ʱ��1��\n");
		OSTaskSuspend(7);
        OSTaskResume(6);
		OSTimeDly(100);
	}
}

void E3_task1(void *pParam)
{
    int i=0;
	OSTimeDly(100);
    printf("����1���ȼ�%d\n",OSPrioCur);
	for(;;)
	{
		printf("����1���%d\n",i++);
		OSTimeDly(20);
	}
}
void E3_task2(void *pParam)
{
    int i=100;
	OSTimeDly(100);
    printf("����2���ȼ�%d\n",OSPrioCur);
	for(;;)
	{
		printf("����2���%d\n",i++);
		OSTimeDly(30);
	}
}
/*end ʵ��ʹ�õĴ���3*/


int add1(int *p1,int *p2)
{
	int a,b,c;
	a=*p1;
	b=*p2;
	OSTimeDly(OS_TICKS_PER_SEC);
	c=a+b;
	return(c);
}

int add2(int p1,int p2)
{
	//int a,b;
	a=p1;
	b=p2;
	OSTimeDly(OS_TICKS_PER_SEC);
	c=a+b;
	return(c);
}

void usertask(void *pParam)
{
	//INT8U i=0;
	 // for(;;){
	//	printf("%d\n",i);
	//	i++;
	//	OSTimeDly(100);
	//}
	INT8U i=(INT8U)pParam;
	//printf("���ȼ�Ϊ%d�������������ʱ%d��\n",OSPrioCur,OSPrioCur);

	for(;;)
	{
		printf("���ȼ�Ϊ%d���������%d\n",OSPrioCur,i++);
		OSTimeDly(100);
	}
}
void usertask1(void *pParam)
{
    int sum=1;
	for(;;){
		//printf("\ntask%d call add2(1,2)\n",1);
		//sum=add2(1,2);
		OSTimeDly(OS_TICKS_PER_SEC);
        printf("task5 after sleep,��������ȴ���%d,FlagEn=%d\n",sum++,FlagEn)  ;
		//printf("\ntask%d call add2(1,2) solution is %d\n",1,sum);
	}//for(;;)
}
void usertask2(void *pParam)
{

    int sum=1;
	for(;;){
		//printf("\n\rtask%d call add2(100,200)\n",2);
		
		OSTimeDly(OS_TICKS_PER_SEC);
		//c=a+b;//sum=add2(100,200);
		printf("task6 after sleep,��������ȴ���%d,FlagEn=%d\n",sum++,FlagEn) ;//printf("\ntask%d call add2(100,200) solution is %d\n",2,sum);
	}//for(;;)
}

void UsrCouPri()
{
	int crt;
	crt=0;
    for(;;){
	//	sum=add2(100,200);	

    }
}

//�ź������������
void UserTaskSemA(void *pParam)
{
    /*����SemA�����ź�����Ȼ�������Է�����ԴR*/
	/*�����ź���*/
	INT8U     *perr;
	INT8U err;
	//INT8U i;
	OS_SEM_DATA mySemData;
	err=0;
	perr=&err;
	MyEventSem=OSSemCreate(2);
    if (MyEventSem==(OS_EVENT *)0)
	{
		printf("����A�����ź���ʧ�ܣ�\n");
		OSTaskDel(OS_PRIO_SELF);
	    return;
	}
	OSSemQuery(MyEventSem,&mySemData); 
    printf("ʱ��:%d, ����A�����ź�������ǰ�ź���ֵ=%d\n",OSTimeGet(),mySemData.OSCnt);
	while(1)
	{
	    OSSemQuery(MyEventSem,&mySemData); 
		printf("ʱ��:%d,����A��ʼ�����ź�������ǰ�ź���ֵ=%d\n",OSTimeGet(),mySemData.OSCnt);
		OSSemPend(MyEventSem,0,perr);
		if (err!=OS_ERR_NONE)
		{
			 printf("����A�����ź���ʧ��\n");
			 printf("�����%d\n",err);
			 continue;
		}
        OSSemQuery(MyEventSem,&mySemData); 
		printf("ʱ��:%d,����A����ź�������ǰ�ź���ֵ=%d������A��ʼ��R����\n",OSTimeGet(),mySemData.OSCnt);
   		OSTimeDly(1000); /*ģ�������Դ,��Ҫ10�룬1000��ʱ�����*/
        printf("ʱ��:%d������A������Դ�������ύ�ź�����\n",OSTimeGet());
        OSSemPost(MyEventSem);
        OSSemQuery(MyEventSem,&mySemData); 
		printf("ʱ��:%d,����A�ύ�ź�����ɣ���ǰ�ź���ֵ=%d,����A����ʱ����1000���\n",OSTimeGet(),mySemData.OSCnt);
        OSTimeDly(1000);
	}
}

void UserTaskSemB(void *pParam)
{
    /*����SemA�����ź�����Ȼ�������Է�����ԴR*/
	INT8U     *perr;
	INT8U err;
	OS_SEM_DATA mySemData;
	err=0;
	perr=&err;
	printf("ʱ��:%d,����B��ʼ��ʱ300��ʱ�����",OSTimeGet());
    OSTimeDly(300);/*����B����ʱ3��*/
	if (MyEventSem==(OS_EVENT *)0)
	{
		printf("����A�����ź���ʧ�ܣ�\n");
		OSTaskDel(OS_PRIO_SELF);
	    return;
	}
	while(1)
	{
	    OSSemQuery(MyEventSem,&mySemData); 
		printf("ʱ��:%d,����B��ʼ�����ź�������ǰ�ź���ֵ=%d\n",OSTimeGet(),mySemData.OSCnt);
		OSSemPend(MyEventSem,0,perr);
		if (err!=OS_ERR_NONE)
		{
			 printf("����B�����ź���ʧ��\n");
			 printf("�����%d\n",err);
			 continue;
		}
        OSSemQuery(MyEventSem,&mySemData); 
		printf("ʱ��:%d,����B����ź�������ǰ�ź���ֵ=%d������B��ʼ��R����,��1000��ʱ�����\n",OSTimeGet(),mySemData.OSCnt);
   		OSTimeDly(1000); /*ģ�������Դ,��Ҫ10�룬1000��ʱ�����*/
        printf("ʱ��:%d������B������Դ�������ύ�ź�����\n",OSTimeGet());
        OSSemPost(MyEventSem);
        OSSemQuery(MyEventSem,&mySemData); 
		printf("ʱ��:%d,����B�ύ�ź�����ɣ���ǰ�ź���ֵ=%d,����B����ʱ����1000���\n",OSTimeGet(),mySemData.OSCnt);
        OSTimeDly(1000);
	}
}


void UserTaskSemC(void *pParam)
{
    /*����SemA�����ź�����Ȼ�������Է�����ԴR*/
	INT8U     *perr;
	INT8U err;
	OS_SEM_DATA mySemData;
	err=0;
	perr=&err;
    printf("ʱ��:%d,����C��ʼ��ʱ400��ʱ�����",OSTimeGet());
    OSTimeDly(400);/*����C����ʱ4��*/
	if (MyEventSem==(OS_EVENT *)0)
	{
		printf("����A�����ź���ʧ�ܣ�\n");
		OSTaskDel(OS_PRIO_SELF);
	    return;
	}
	while(1)
	{
	    OSSemQuery(MyEventSem,&mySemData); 
		printf("ʱ��:%d,����C��ʼ�����ź�������ǰ�ź���ֵ=%d\n",OSTimeGet(),mySemData.OSCnt);
		OSSemPend(MyEventSem,0,perr);
		if (err!=OS_ERR_NONE)
		{
			 printf("����C�����ź���ʧ��\n");
			 printf("�����%d\n",err);
			 continue;
		}
        OSSemQuery(MyEventSem,&mySemData); 
		printf("ʱ��:%d,����C����ź�������ǰ�ź���ֵ=%d������C��ʼ��R����,��1000��ʱ�����\n",OSTimeGet(),mySemData.OSCnt);
   		OSTimeDly(1000); /*ģ�������Դ,��Ҫ10�룬1000��ʱ�����*/
        printf("ʱ��:%d������C������Դ�������ύ�ź�����\n",OSTimeGet());
        OSSemPost(MyEventSem);
        printf("ʱ��:%d,����C�ύ�ź�����ɣ���ǰ�ź���ֵ=%d,����C����ʱ����1000���\n",OSTimeGet(),mySemData.OSCnt);
        OSTimeDly(1000);
	}
}

//�¼���־�������
INT8U IO[4][10];
OS_FLAG_GRP  * pFlagGroupDataProcess;
void TaskDataProcess(void *pParam)
{
      INT8U     *perr;
	  INT8U err,i;   
      INT16U SUM;
      OS_FLAGS processflag,retflag;
      err=OS_ERR_NONE;	  
      perr=&err;
	  processflag=0x0F;
	  /*�����¼���־��,�¼���־��ʼֵ0��û���¼�����*/
      pFlagGroupDataProcess=OSFlagCreate(0,perr); 
      /*ʡ���˼���Ƿ񴴽��ɹ�*/
	  while(1)
	  {
		  printf("ʱ��:%d������TaskDataProcess��ʼ�����¼���־-----------��\n",OSTimeGet());
		  retflag=OSFlagPend (pFlagGroupDataProcess,
                      processflag,
                      OS_FLAG_WAIT_SET_ALL+OS_FLAG_CONSUME,
                      0,
                      perr);  
		  
		  if (retflag==processflag)
		  {
				SUM=0;
			    printf("ʱ��:%d������TaskDataProcess�����¼���־�ɹ�����ʼ�������ݣ�\n",OSTimeGet());
                for (i=0;i<10;i++)
				{
					SUM+=IO[0][i]+IO[1][i]+IO[2][i]+IO[3][i];
				}
                printf("ʱ��:%d������TaskDataProcess����������ɣ����Ϊ%d:\n",OSTimeGet(),SUM);
		  }	  
	  }
}
void TaskIO1(void *pParam)
{
      INT8U     *perr;
	  INT8U err,i;   
	  
	  OS_FLAGS rdyflag;
	  //OS_FLAG_GRP  * pFlagGroup;
	  err=OS_ERR_NONE;
	  perr=&err;	
	  rdyflag=0;

	  while(1)
	  {
		  OSTimeDly(100);/*��ʱ1��*/ 
          for (i=0;i<10;i++) /*ģ���ȡ���ݵĹ���*/
		  {
				IO[0][i]=1;
		  }
          printf("ʱ��:%d������TaskIO1��ȡIO���ݺ�׼���ύ�¼�����ǰ�¼���־λ:%d\n",OSTimeGet(),rdyflag);
		  rdyflag=OSFlagPost (pFlagGroupDataProcess,
                      0x01,
                      OS_FLAG_SET,
                     perr);            /*�ύ�¼���־����λ�¼���־�������һλλ0*/
		  printf("ʱ��:%d������TaskIO1��ȡIO���ݺ��ύ�¼�����ǰ�¼���־λ:%d\n",OSTimeGet(),rdyflag);
	  }
}
void TaskIO2(void *pParam)
{
      INT8U     *perr;
	  INT8U err,i;   
 	  OS_FLAGS rdyflag;
	  perr=&err;
	  err=OS_ERR_NONE;
	  rdyflag=0;
	  while(1)
	  {
		  OSTimeDly(100);/*��ʱ1��*/ 
          for (i=0;i<10;i++) /*ģ���ȡ���ݵĹ���*/
		  {
				IO[1][i]=2;
		  }
          printf("ʱ��:%d������TaskIO2��ȡIO���ݺ�׼���ύ�¼�����ǰ�¼���־λ:%d\n",OSTimeGet(),rdyflag);
		  rdyflag=OSFlagPost (pFlagGroupDataProcess,
                      0x02,
                      OS_FLAG_SET,
                     perr);            /*�ύ�¼���־����λ�¼���־����λ1*/
		  printf("ʱ��:%d������TaskIO2��ȡIO���ݺ��ύ�¼�����ǰ�¼���־λ:%d\n",OSTimeGet(),rdyflag);
	  }
}
void TaskIO3(void *pParam)
{
      INT8U     *perr;
	  INT8U err,i;   
 	  OS_FLAGS rdyflag;
	  perr=&err;
	  err=OS_ERR_NONE;
	  rdyflag=0;
	  while(1)
	  {
		  OSTimeDly(100);/*��ʱ1��*/ 
          for (i=0;i<10;i++) /*ģ���ȡ���ݵĹ���*/
		  {
				IO[2][i]=3;
		  }
          printf("ʱ��:%d������TaskIO3��ȡIO���ݺ�׼���ύ�¼�����ǰ�¼���־λ��%d\n",OSTimeGet(),rdyflag);
		  rdyflag=OSFlagPost (pFlagGroupDataProcess,
                      0x04,
                      OS_FLAG_SET,
                     perr);            /*�ύ�¼���־����λ�¼���־����λ2*/
		  printf("ʱ��:%d������TaskIO3��ȡIO���ݺ��ύ�¼�����ǰ�¼���־λ��%d\n",OSTimeGet(),rdyflag);
	  }
}
void TaskIO4(void *pParam)
{
      INT8U     *perr;
	  INT8U err,i;   
 	  OS_FLAGS rdyflag;
	  perr=&err;
	  err=OS_ERR_NONE;
	  rdyflag=0;
	  while(1)
	  {
		  OSTimeDly(100);/*��ʱ1��*/ 
          for (i=0;i<10;i++) /*ģ���ȡ���ݵĹ���*/
		  {
				IO[3][i]=3;
		  }
          printf("ʱ��:%d������TaskIO4��ȡIO���ݺ�׼���ύ�¼�����ǰ�¼���־λ��%d\n",OSTimeGet(),rdyflag);
		  rdyflag=OSFlagPost (pFlagGroupDataProcess,
                      0x08,
                      OS_FLAG_SET,
                     perr);            /*�ύ�¼���־����λ�¼���־����λ3*/
		  printf("ʱ��:%d������TaskIO4��ȡIO���ݺ��ύ�¼�����ǰ�¼���־λ��%d\n",OSTimeGet(),rdyflag);
	  }
}

/*Mutex���ӳ���ʹ�����ȼ���ת*/
OS_EVENT  *myMutex;
void TaskMutex1(void *pParam)
{
      INT8U     *perr;
	  INT8U err,i;  
	  INT32U j;
	  perr=&err;
	  err=OS_ERR_NONE;


      myMutex=OSMutexCreate(3,perr);/*���������ź��������ȼ��̳����ȼ�PIPΪ9*/
	  if (myMutex==(OS_EVENT  *)0)   /*����Ƿ񴴽��ɹ�*/
	  {
			printf("ʱ��:%d�������ȼ�����TaskMutex1���������ź���ʧ��,ʧ�ܺ�%d:\n",OSTimeGet(),*perr);
		    OSTaskDel(OS_PRIO_SELF); /*���ɹ���ɾ��������*/
			return;
	  }
      printf("ʱ��:%d�������ȼ�����TaskMutex1���������ź����ɹ�.\n",OSTimeGet());
      OSTimeDly(100);/*��ʱ1��*/ 
      printf("ʱ��:%d�������ȼ�����TaskMutex1���󻥳��ź���.\n",OSTimeGet());
      OSMutexPend(myMutex,0,perr);/*�ȴ������ź���*/
	  printf("ʱ��:%d�������ȼ�����TaskMutex1��û����ź���.\n",OSTimeGet());
	  if (*perr == OS_ERR_NONE)
	  {
		  
		  for(i=1;i<=5;i++)
		  {
			  printf("ʱ��%d:�����ȼ�����TaskMutex1�򴮿��������%d\n",OSTimeGet(),i);  /*ģ�����IO*/	 
			  for (j=1;j<=9999999;j++);	 /*ģ���������*/  			  
		  }
	  }
	  else
	  {
		  printf("ʱ��:%d�������ȼ�����TaskMutex1�����ź���ʧ��,ʧ�ܺ�%d:\n",OSTimeGet(),*perr);		
	  }
	  OSMutexPost(myMutex);
	  for(i=1;i<=5;i++)
	  {
		  
		  printf("ʱ��%d:�����ȼ�����TaskMutex1ִ���ύ�ź�����ִ����������%d\n",OSTimeGet(),i);	/*ģ�����IO*/	   
		  for (j=1;j<=99999999;j++);	 /*��ʱ,��ʾ�ڲ�������*/	
	  }
	  printf("�����ȼ�����TaskMutex1�������У�ɾ���Լ�\n",OSTimeGet(),*perr);	
	  OSTaskDel(OS_PRIO_SELF); /*ɾ��������*/
	  return;

}
void TaskMutex2(void *pParam)
{
	  INT8U 	*perr;
	  INT8U err,i;	 
	  INT32U j;
	  perr=&err;
	  err=OS_ERR_NONE;
	  if (myMutex==(OS_EVENT  *)0)	 /*����Ƿ��б������Ļ����ź���*/
	  {
			printf("ʱ��:%d�������ź���δ����");
			OSTaskDel(OS_PRIO_SELF); /*ɾ��������*/
			return;
	  }
	  OSTimeDly(90);/*��ʱ����1��*/ 
	  printf("ʱ��:%d�������ȼ�����TaskMutex2���󻥳��ź���\n",OSTimeGet());
	  OSMutexPend(myMutex,0,perr);/*�ȴ������ź���*/
	  printf("ʱ��:%d������TaskMutex2��û����ź���\n",OSTimeGet());
	  if (*perr == OS_ERR_NONE)
	  {
		  printf("ʱ��:%d�������ȼ�����TaskMutex2��û����ź���\n",OSTimeGet());
		  for(i=1;i<=5;i++)
		  {
			  
			  printf("ʱ��%d:�����ȼ�TaskMutex2�򴮿��������%d\n",OSTimeGet(),i);  /*ģ�����IO*/     
			  for (j=1;j<=99999999;j++);   /*ģ���������*/    
		  }
	  }
	  else
	  {
		  printf("ʱ��:%d�������ȼ�����TaskMutex2�����ź���ʧ��,ʧ�ܺ�:\n",OSTimeGet(),*perr);     
	  }
	  OSMutexPost(myMutex);
	  for(i=1;i<=5;i++)
	  {		  
		  printf("ʱ��%d:�����ȼ�TaskMutex2ִ���ύ�ź�����ִ����������%d\n",OSTimeGet(),i);  /*ģ�����IO*/     
		  for (j=1;j<=99999999;j++);   /*��ʱ,��ʾ�ڲ�������*/    
	  }
	  printf("�����ȼ�����TaskMutex2�������У�ɾ���Լ�\n",OSTimeGet(),*perr);   
	  OSTaskDel(OS_PRIO_SELF); /*ɾ��������*/
	  return;
}

void TaskPrint(void *pParam)
{
      INT8U     *perr;
	  INT8U err,i;   
	  INT32U j;
	  perr=&err;
	  err=OS_ERR_NONE;
      i=0;
      OSTimeDly(95);
	  for(i=1;i<=5;i++)
	  {
		  printf("ʱ��%d:�����ȼ�����TaskPrint�������У���ӡ����%d\n",OSTimeGet(),i++);  /*ģ�����IO*/     
		  for (j=1;j<=99999999;j++);   /*ģ����д�ӡ����*/  	
	  }
	  printf("�����ȼ�����TaskPrint�������У�ɾ���Լ�\n");
	  OSTaskDel(OS_PRIO_SELF); /*ɾ��������*/
}
//��Ϣ���������
OS_EVENT  *myMBox;
void TaskMessageSen(void *pParam)
{
      INT8U     *perr;
	  INT8U err;//,i;  
	  //INT32U j;
	  INT32U scount;
	  	  int a[3];
	  perr=&err;
	  err=OS_ERR_NONE;

      scount=1;

	  a[1]=5;
	  a[3]=6;
      myMBox=OSMboxCreate(&scount);/*�������䣬*/
	  if (myMBox==(OS_EVENT  *)0)   /*����Ƿ񴴽��ɹ�*/
	  {
			printf("ʱ��:%d�� TaskMessageSen��������ʧ��\n",OSTimeGet());
		    OSTaskDel(OS_PRIO_SELF); /*���ɹ���ɾ��������*/
			return;
	  }
	  printf("ʱ��:%d�� TaskMessageSen��������ɹ�\n",OSTimeGet());
	  while(1)
	  {
		  OSTimeDly(100);/*��ʱ1��*/ 
		  scount++;
          printf("ʱ��:%d������TTaskMessageSen׼������Ϣ����ϢΪ%d\n",OSTimeGet(),scount);
		  OSMboxPost(myMBox,&scount); /*����Ϣ*/
	  }
}
void TaskMessageRec(void *pParam)
{
      INT8U     *perr;
	  INT8U err;//,i;  
	  INT32U * prcount;
	  perr=&err;
	  err=OS_ERR_NONE;

	  if (myMBox==(OS_EVENT  *)0)   /*��������Ƿ����*/
	  {
			printf("ʱ��:%d������TaskMessageRec�ж����䲻����!\n",OSTimeGet());
		    OSTaskDel(OS_PRIO_SELF); /*���ɹ���ɾ��������*/
			return;
	  }
	  
	  while(1)
	  { 
          prcount=(INT32U * )OSMboxPend(myMBox,0,perr); /*������Ϣ�������Ϣ�����ھ�����*/
		  if (*perr==OS_ERR_NONE)
			printf("ʱ��:%d������TaskMessageRec������ϢΪ%d\n",OSTimeGet(),*prcount);
		  else
            printf("ʱ��:%d������TaskMessageRec�ȴ��쳣�����������:%d\n",*perr);
	  }
}

//��Ϣ���е�����
OS_EVENT  *myQ;
void TaskQSen(void *pParam)
{
      INT8U     *perr;
	  INT8U err,i;  
	  INT8U scount;
	  OS_Q_DATA myQData; /*��Ϣ��������*/ 
	  void * myq[6]; /*��Ϣ����*/
	  INT8U mymessage[256];
	  perr=&err;
	  err=OS_ERR_NONE;    
      scount=0;

      myQ=OSQCreate(myq,6);/*������Ϣ���У��ݻ�Ϊ6*/
	  if (myQ==(OS_EVENT  *)0)   /*����Ƿ񴴽��ɹ�*/
	  {
			printf("ʱ��:%d�� TaskQSen������Ϣ����ʧ��\n",OSTimeGet());
		    OSTaskDel(OS_PRIO_SELF); /*���ɹ���ɾ��������*/
			return;
	  }
	  printf("ʱ��:%d�� TaskQSen������Ϣ���гɹ�\n",OSTimeGet());
      for (i=0;i<=254;i++)
		 mymessage[i]=i;
      mymessage[255]=i;
	  while(1)
	  {
		  OSTimeDly(100);/*��ʱ1��*/ 
		  
          printf("ʱ��:%d������TTaskQSen׼������Ϣ����ϢΪ%d\n",OSTimeGet(),mymessage[scount]);
		  err=OSQPost(myQ,&mymessage[scount]); /*����Ϣ*/
		  switch (err) {
				case OS_ERR_Q_FULL:
                    printf("ʱ��:%d������TTaskQSen����Ϣʧ�ܣ���Ϣ��������\n",OSTimeGet());
					break;
                case OS_ERR_NONE:
                    printf("ʱ��:%d������TTaskQSen����Ϣ�ɹ�\n",OSTimeGet());
					break;
				default:
                    printf("ʱ��:%d������TTaskQSen����Ϣʧ�ܣ�����ţ�%d\n",OSTimeGet(),err);
		  }
		  OSQQuery(myQ,&myQData);
		  printf("ʱ��:%d����ǰ��������Ϣ������%d\n",OSTimeGet(),myQData.OSNMsgs);
		  scount++;
	  }
}

void TaskQRec(void *pParam)
{
      INT8U     *perr;
	  INT8U err;  
	  INT8U rcount;
	  INT8U rec;
	  OS_Q_DATA myQData; /*��Ϣ��������*/ 
	  //INT8U mymessage[256];
	  perr=&err;
	  err=OS_ERR_NONE;    
      rcount=0;

     
	  if (myQ==(OS_EVENT  *)0)   /*�����Ϣ�����Ƿ����*/
	  {
			printf("ʱ��:%d�� TaskQRec�ж���Ϣ���в�����\n",OSTimeGet());
		    OSTaskDel(OS_PRIO_SELF); /*���ɹ���ɾ��������*/
			return;
	  }
	  while(1)
	  {
		  OSTimeDly(200);/*��ʱ1��*/ 
          printf("ʱ��:%d������TaskQRec��ʼ�ȴ���Ϣ\n",OSTimeGet());
		  rec=(unsigned char)*(INT32U *)OSQPend(myQ,0,perr); /*����Ϣ*/
		  switch (err) {
                case OS_ERR_NONE:
                    printf("ʱ��:%d������TaskQRec���յ���Ϣ%d\n",OSTimeGet(),rec);
					break;
				default:
                    printf("ʱ��:%d������TTaskMessageSen����Ϣʧ�ܣ�����ţ�%d\n",OSTimeGet(),err);
		  }
		  OSQQuery(myQ,&myQData);
		  printf("ʱ��:%d����ǰ��������Ϣ������%d\n",OSTimeGet(),myQData.OSNMsgs);
	  }
}

//�ڴ���������

void TaskM(void *pParam)
{
      
	  INT8U *perr;
	  INT8U err,i;
      OS_MEM  *pmyMem;     //MCB���ַ
      INT8U myMem[3][20]; //�������ڴ����
	  void    *pblk[10];   //�ڴ���ַ����
	  BOOLEAN require;
	  OS_MEM_DATA mem_data;//���ڲ�ѯ�ڴ����Ϣ
	  err=OS_ERR_NONE; 	
	  perr=&err;
      require=1;
      pmyMem=OSMemCreate(myMem,3,20,perr);/*�����ڴ������10����, ÿ����20���ֽ�*/
	  if (pmyMem==(OS_MEM  *)0)   /*����Ƿ񴴽��ɹ�*/
	  {
			printf("ʱ��:%d�� TaskM�����ڴ����ʧ��\n",OSTimeGet());
		    OSTaskDel(OS_PRIO_SELF); /*���ɹ���ɾ��������*/
			return;
	  }
	  printf("ʱ��:%d�� TaskM�����ڴ�����ɹ�������10����, ÿ����20���ֽ�\n",OSTimeGet());
	  i=0;
	  while(1)
	  {
		  if (i>5) 
		  { 
			  i=0;
			  require=!require;
		  }
          //printf("ʱ��:%d��i=%d\n",OSTimeGet(),i);
		  OSTimeDly(100);/*��ʱ1��*/ 
		  if (require)
		  { 
			printf("ʱ��:%d������TaskM׼������һ����->",OSTimeGet());
			pblk[i++]=OSMemGet(pmyMem,perr); /*�����ڴ��*/
			switch (err) {
				case OS_ERR_MEM_NO_FREE_BLKS:
                    printf("ʱ��:%d������TaskM�������ڴ��ʧ�ܣ����������޿����ڴ�飡\n",OSTimeGet());
					break;
                case OS_ERR_NONE:
                    printf("ʱ��:%d������TaskM����ڴ��\n",OSTimeGet());
					break;
				default:
                    printf("ʱ��:%d������TaskM�������ڴ��ʧ�ܣ�����ţ�%d\n",OSTimeGet(),err);
			}
		  }
		  else
		  {
		  	printf("ʱ��:%d������TaskM׼���ͷ�һ����->",OSTimeGet());
			err=OSMemPut(pmyMem,pblk[i++]); /*�����ڴ��*/
			switch (err) {
				case OS_ERR_MEM_FULL:
                    printf("ʱ��:%d������TaskM�������ڴ��ʧ�ܣ�����������\n",OSTimeGet());
					break;
                case OS_ERR_MEM_INVALID_PBLK:
                    printf("ʱ��:%d������TaskM���ͷ��ڴ��ʧ�ܣ��ͷ���Ч���ڴ�飡\n",OSTimeGet());
					break;
                case OS_ERR_NONE:
                    printf("ʱ��:%d������TaskM�ɹ��ͷ��ڴ��\n",OSTimeGet());
					break;
				default:
                    printf("ʱ��:%d������TaskM�ͷ��ڴ��ʧ�ܣ�����ţ�%d\n",OSTimeGet(),err);
			}
		  
		  }
		  OSMemQuery(pmyMem,&mem_data);       
		  printf("ʱ��:%d����ǰ�����е������ڴ��������%d\n",OSTimeGet(),mem_data.OSNUsed);
		  printf("ʱ��:%d����ǰ�����еĿ����ڴ��������%d\n",OSTimeGet(),mem_data.OSNFree);
	  }
}


/*
* �ź������ȼ���ת
*/
// usercode.c
OS_EVENT* sem;

//��ȡ�ź���
static void AcquireSem(const char* name, INT8U prio, OS_SEM_DATA* pSemData, INT8U* perr) {
	OSSemQuery(sem, pSemData);
	if (pSemData->OSCnt == 0) {
		printf("ʱ��:%d, %s[���ȼ�%d]��ʼ�����ȴ��ź�����\n",OSTimeGet(),name,prio);
	}
	OSSemPend(sem, 0, perr);
	if (*perr != OS_ERR_NONE) {
		printf("%s�����ź���ʧ��\n", name);
		printf("�����%d\n", *perr);
		return;
	}
	OSSemQuery(sem, pSemData);
	printf("ʱ��:%d,%s[���ȼ�%d]��ȡ�ź����ɹ�����ǰ�ź���ֵ=%d\n",OSTimeGet(),name,prio,pSemData->OSCnt);
	printf("ʱ��:%d,%s[���ȼ�%d]����ִ������\n", OSTimeGet(), name, prio);
	for (int i = 0; i < 100000000; i++) {
		for (int j = 0; j < 999999999; j++);
	}
}

void MySemTaskA(void* p) {
	INT8U err = 0;
	OS_SEM_DATA semData;
	sem = OSSemCreate(1);
	if (sem == (OS_EVENT*)0) {
		printf("����A�����ź���ʧ�ܣ�\n");
		OSTaskDel(OS_PRIO_SELF);
		return;
	}
	OSSemQuery(sem, &semData);
	printf("ʱ��:%d,����A�����ź�������ǰ�ź���ֵ=%d\n", OSTimeGet(), semData.OSCnt);
	OSTimeDly(100);
	AcquireSem("����A", 5, &semData, &err);
	OSTaskDel(OS_PRIO_SELF);
}

void MySemTaskB(void* p) {
	INT8U err = 0;
	OS_SEM_DATA semData;
	OSTimeDly(100);
	AcquireSem("����B", 7, &semData, &err);
	OSTaskDel(OS_PRIO_SELF);
}

void MySemTaskC(void* p) {
	INT8U err = 0;
	OS_SEM_DATA semData;
	printf("ʱ��:%d,����C��ʼ\n", OSTimeGet());
	AcquireSem("����C", 10, &semData, &err);
	OSTaskDel(OS_PRIO_SELF);
}

void MyTaskD(void* p) {
	OSTimeDly(100);
	for (;;) {
		for (INT32U j = 0; j < 999999999; ++j);
		printf("ʱ��: %d, ����D[���ȼ�8]����ִ��\n", OSTimeGet());
	}
	OSTaskDel(OS_PRIO_SELF);
}

/*
* Mutex
*/
/*
* �ź������ȼ���ת
*/
// usercode.c
OS_EVENT* mutex;

//��ȡ�ź���
static void AcquireMutex(const char* name, INT8U prio, INT8U* perr) {
	OS_MUTEX_DATA data;
	OSMutexQuery(mutex, &data);
	if (!data.OSValue) {
		printf("ʱ��:%d, %s[���ȼ�%d]��ʼ�����ȴ���������\n", OSTimeGet(), name, prio);
	}
	OSMutexPend(mutex, 0, perr);
	if (*perr != OS_ERR_NONE) {
		printf("%s�����ź���ʧ��\n", name);
		printf("�����%d\n", *perr);
		return;
	}
	printf("ʱ��:%d,%s[���ȼ�%d]��ȡ�������ɹ�!\n", OSTimeGet(), name, prio);
	printf("ʱ��:%d,%s[���ȼ�%d]����ִ������\n", OSTimeGet(), name, prio);
	for (int i = 0; i < 100000000; i++) {
		for (int j = 0; j < 999999999; j++);
	}
}

void MyMutexTaskA(void* p) {
	INT8U err = 0;
	mutex = OSMutexCreate(2,&err);
	if (mutex == (OS_EVENT*)0) {
		printf("����A�����ź���ʧ�ܣ�\n");
		OSTaskDel(OS_PRIO_SELF);
		return;
	}
	printf("ʱ��:%d,����A������������\n", OSTimeGet());
	OSTimeDly(100);
	AcquireMutex("����A", 5, &err);
	OSTaskDel(OS_PRIO_SELF);
}

void MyMutexTaskB(void* p) {
	INT8U err = 0;
	OSTimeDly(100);
	AcquireMutex("����B", 7, &err);
	OSTaskDel(OS_PRIO_SELF);
}

void MyMutexTaskC(void* p) {
	INT8U err = 0;
	printf("ʱ��:%d,����C��ʼ\n", OSTimeGet());
	AcquireMutex("����C", 10, &err);
	OSTaskDel(OS_PRIO_SELF);
}

OS_FLAG_GRP* pgrp;

OS_FLAGS TASK_A_BIT = 0x1;
OS_FLAGS TASK_B_BIT = 0x1<<1;
OS_FLAGS TASK_C_BIT = 0x1<<2;
OS_FLAGS TASK_D_BIT = 0x1<<3;
OS_FLAGS TASK_E_BIT = 0x1<<4;


void FlagsTaskA() {
	INT32U error=0;
	printf("����A��ʼ����\n");
	OSTimeDly(100);
	OSFlagPost(pgrp,TASK_A_BIT<<4 | TASK_A_BIT,OS_FLAG_SET,&error);
	if (error != OS_ERR_NONE) {
		printf("����A���ñ�־λʧ��\n");
	}
	OSTaskDel(OS_PRIO_SELF);
}

void FlagsTaskB() {
	INT32U error=0;
	printf("����B��ʼ����\n");
	OSTimeDly(100);
	OSFlagPost(pgrp, TASK_B_BIT << 4 | TASK_B_BIT, OS_FLAG_SET, &error);
	if (error != OS_ERR_NONE) {
		printf("����B���ñ�־λʧ��\n");
	}
	OSTaskDel(OS_PRIO_SELF);
}

void FlagsTaskC() {
	INT32U error=0;
	OSFlagPend(pgrp,TASK_A_BIT | TASK_B_BIT,OS_FLAG_WAIT_SET_ALL,0,&error);
	if (error != OS_ERR_NONE) {
		printf("����C�ȴ�ʧ��,error:%d\n",error);
	}
	printf("����C��ʼ����\n");
	OSFlagPost(pgrp, TASK_C_BIT << 4 | TASK_C_BIT, OS_FLAG_SET, &error);
	if (error != OS_ERR_NONE) {
		printf("����C���ñ�־λʧ��,error:%d\n",error);
	}
	OSTaskDel(OS_PRIO_SELF);
}

void FlagsTaskD() {
	INT32U error=0;
	OSFlagPend(pgrp, TASK_C_BIT, OS_FLAG_WAIT_SET_ALL, 0, &error);
	if (error != OS_ERR_NONE) {
		printf("����D�ȴ�ʧ��,error:%d\n", error);
	}
	printf("����D��ʼ����\n");
	OSFlagPost(pgrp, TASK_D_BIT << 4 | TASK_D_BIT, OS_FLAG_SET, &error);
	if (error != OS_ERR_NONE) {
		printf("����D���ñ�־λʧ��,error:%d\n", error);
	}
	OSTaskDel(OS_PRIO_SELF);
}

void FlagsTaskE() {
	INT32U error = 0;
	pgrp = OSFlagCreate(0, &error);
	if (error != OS_ERR_NONE) {
		printf("�����¼���־��ʧ��\n");
		OSTaskDel(OS_PRIO_SELF);
		return;
	}
	INT32U flags = 0;
	//�ȴ��κ��������
	while (TRUE) {
		OSFlagPend(pgrp, (TASK_A_BIT | TASK_B_BIT | TASK_C_BIT | TASK_D_BIT)<<4, OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME, 0, &error);
		if ((pgrp->OSFlagFlags & TASK_A_BIT) && !(flags & TASK_A_BIT)) {
			printf("ͳ����������A�Ѿ����\n");
			flags |= TASK_A_BIT;
		}
		if ((pgrp->OSFlagFlags & TASK_B_BIT) && !(flags & TASK_B_BIT)) {
			printf("ͳ����������B�Ѿ����\n");
			flags |= TASK_B_BIT;
		}
		if ((pgrp->OSFlagFlags & TASK_C_BIT) && !(flags & TASK_C_BIT)) {
			printf("ͳ����������C�Ѿ����\n");
			flags |= TASK_C_BIT;
		}
		if ((pgrp->OSFlagFlags & TASK_D_BIT) && !(flags & TASK_D_BIT)) {
			printf("ͳ����������D�Ѿ����\n");
			flags |= TASK_D_BIT;
		}
		if (flags == (TASK_A_BIT | TASK_B_BIT | TASK_C_BIT | TASK_D_BIT)) {
			printf("����������ɣ�����E����\n");
			break;
		}
	}
	OSTaskDel(OS_PRIO_SELF);
}