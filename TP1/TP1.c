/**
 * @file TP1.c
 *
 * @section desc File description
 *
 *
 * @section copyright Copyright
 *
 *
 * @section infos File informations
 *
 * $Date$ mar. févr.  6 10:31:31 CET 2018
 * $Rev$
 * $Author$ 16009566
 * $URL$ /private/student/6/66/16009566/ITR/TP1
 */

#include "tpl_os.h"
#include "nxt_motors.h"
#include "ecrobot_interface.h" 
#include "ecrobot_private.h"

FUNC(int, OS_APPL_CODE) main(void)
{   
    StartOS(OSDEFAULTAPPMODE);
    ShutdownOS(E_OK);
    return 0;
}

DeclareTask(task0);
DeclareTask(task1);
DeclareTask(task2);
DeclareTask(task3);
/*DeclareTask(task4);*/

ISR(isr_button_start)
{
    ecrobot_status_monitor("isr_button_start");
    
}

ISR(isr_button_stop)
{
    ShutdownOS(E_OK);
}

ISR(isr_button_left)
{
    ecrobot_status_monitor("isr_button_left");

}

ISR(isr_button_right)
{
    ecrobot_status_monitor("isr_button_right");

}

TASK(task0) {
    display_string("Test Task0\n");
    display_update();
    systick_wait_ms(10000);

    
    ActivateTask(task1);
    TerminateTask();
}

TASK(task1) {
    int time = systick_get_ms();
    display_string("Task1 : ");
    display_int(time, 0);
    display_string("\n");
    display_update();
    
    TerminateTask();
}

TASK(task2) {
    display_string("bonjour je suis task2\n");
    display_update();
    
    ChainTask(task3);
}

TASK(task3) {
    int time = systick_get_ms();
    display_string("Task3 : ");
    display_int(time, 0);
    display_string("\n");
    display_update();
    
    TerminateTask();
}

/*TASK(task4) {*/
/*	EventMaskType mask;*/
/*	display_clear(1);*/
/*	GetEvent(isr_button_right, &mask);*/
/*	WaitEvent(mask);*/
/*	*/
/*    display_string("Task4 : Event passe");*/
/*    display_string("\n");*/
/*    display_update();*/
/*    */
/*    TerminateTask();*/
/*}*/

void PreTaskHook() {
    
    TaskType tmp;
    GetTaskID(&tmp);
/*    display_string("Task");*/
/*    display_int(tmp, 0);*/
/*    display_string(" : PreTaskHook\n");*/
}

void PostTaskHook() {
    
    TaskType tmp;
    GetTaskID(&tmp);
/*    display_string("Task");*/
/*    display_int(tmp, 0);*/
/*    display_string(" : PostTaskHook\n");*/

}

/* End of file TP1.c */
