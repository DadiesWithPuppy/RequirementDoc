/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*                          (c) Copyright 2009-2011; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                          APPLICATION CODE
*
*                                          Microsoft Windows
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : JJL
*                 FGK
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>


/*
*********************************************************************************************************
*                                             LOCAL DEFINES
*********************************************************************************************************
*/
#define TASK_1_PRIO 1u
#define TASK_2_PRIO 2u


/*
*********************************************************************************************************
*                                            LOCAL VARIABLES
*********************************************************************************************************
*/

static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];
static  CPU_STK  Task1Stk[APP_TASK_START_STK_SIZE];
static  CPU_STK  Task2Stk[APP_TASK_START_STK_SIZE];
static  CPU_STK  Task3Stk[APP_TASK_START_STK_SIZE];


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart(void  *p_arg);
static void MyTask(void *p_arg);

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

int  main (void)
{
    OSInit();                                                   /* Init uC/OS-II.                                       */

    OSTaskCreateExt((void(*)(void *))AppTaskStart,              /* Create the start task                                */
        (void          *) 0,
        (OS_STK        *)&AppTaskStartStk[APP_TASK_START_STK_SIZE - 1],
        (INT8U          ) APP_TASK_START_PRIO,
        (INT16U         ) APP_TASK_START_PRIO,
        (OS_STK        *)&AppTaskStartStk[0],
        (INT32U         ) APP_TASK_START_STK_SIZE,
        (void          *) 0,
        (INT16U         )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR),0,0);

	//1
	OSTaskCreateExt((void(*)(void *))MyTask,              /* Create the start task                                */
		(void          *)0,
		(OS_STK        *)&Task1Stk[APP_TASK_START_STK_SIZE - 1],
		(INT8U)TASK_1_PRIO,
		(INT16U)TASK_1_PRIO,
		(OS_STK        *)&Task1Stk[0],
		(INT32U)APP_TASK_START_STK_SIZE,
		(void          *)0,
		(INT16U)(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR),
		3, 1);

	//2
	OSTaskCreateExt((void(*)(void *))MyTask,              /* Create the start task                                */
		(void          *)0,
		(OS_STK        *)&Task2Stk[APP_TASK_START_STK_SIZE - 1],
		(INT8U)TASK_2_PRIO,
		(INT16U)TASK_2_PRIO,
		(OS_STK        *)&Task2Stk[0],
		(INT32U)APP_TASK_START_STK_SIZE,
		(void          *)0,
		(INT16U)(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR),
		5, 3);

    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II).  */
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Argument(s) : p_arg       is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Note(s)     : (1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    OS_ERR  err;


   (void)p_arg;

    BSP_Init();                                                 /* Initialize BSP functions                             */
    CPU_Init();                                                 /* Initialize uC/CPU services                           */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

    APP_TRACE_DBG(("uCOS-II is Running...\n\r"));

    while (DEF_ON) {                                            /* Task body, always written as an infinite loop.       */
        OSTimeDlyHMSM(0, 0, 1, 0);

        //APP_TRACE_DBG(("Time: %d\n\r", OSTimeGet(&err)));
		//APP_TRACE("test %d \n",123); 
    }
}


static void MyTask(void *p_arg) {
	int start, end;
	int toDelay;
	start = 0;

	int T = OSTCBCur->p; //任务执行周期
	int c = OSTCBCur->c; //任务执行时间

	while (1) {
		//延迟任务不执行
		while (OSTCBCur->compTime > 0) {

		}
		end = OSTimeGet();

		toDelay = T - (end - start);//延迟时间=周期-(end-start)
		start = start + T; // next start time

		//剩余执行时间
		OSTCBCur->compTime = c; // reset the counter (c ticks for computation)
		//DDL=周期+start
		OSTCBCur->ddl = T + start;

		OSTimeDly(toDelay); // delay and wait (T-C) times
	}
}