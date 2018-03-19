/**
 * @file TP2.c
 *
 * @section desc File description
 *
 *
 * @section copyright Copyright
 *
 *
 * @section infos File informations
 *
 * $Date$ mar. févr. 20 10:42:32 CET 2018
 * $Rev$
 * $Author$ 16009566
 * $URL$ /private/student/6/66/16009566/ITR/TP2
 */

#include "tpl_os.h"
#include "nxt_motors.h"
#include "ecrobot_interface.h" 
#include "ecrobot_private.h"
#include <stdio.h>
#include <stdlib.h>

int mutexDist=1;
int distance=51;

int ordre=3;
#define RECULER 1
#define TOURNER 2
#define AVANCER 3
/*Differents ordre pour le robot :
				0 : En attente d'ordre
				1 : Reculer
				2 : Tourner
				3 : Avancer*/
int compteur=0;
int periode=1;
int gauche=1;

FUNC(int, OS_APPL_CODE) main(void)
{
    StartOS(OSDEFAULTAPPMODE);
    ShutdownOS(E_OK);
    return 0;
}

DeclareTask(Detection_contact);
DeclareTask(Detection_distance);
DeclareTask(Navigation);

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

TASK(Detection_contact)
{
	int contact1 = ecrobot_get_touch_sensor(NXT_PORT_S1);
	int contact2 = ecrobot_get_touch_sensor(NXT_PORT_S3);
	
    if (contact1 || contact2){
/*    	P(mutex);*/
		gauche = contact1;
		GetResource(mutexDist);
    	distance = 0;
    	ReleaseResource(mutexDist);
/*    	V(mutex);*/
	}
	TerminateTask();
}

TASK(Detection_distance)
{
/*	P(mutex);*/
	GetResource(mutexDist);
	distance = ecrobot_get_sonar_sensor(NXT_PORT_S2);
	if (distance == 0){
		distance = 1;
	}
	ReleaseResource(mutexDist);
/*	V(mutex);*/
    TerminateTask();
}

TASK(Navigation)
{
	if (ordre == AVANCER){
/*		P(mutex);*/
		GetResource(mutexDist);
		if (distance == 0){
			ordre = RECULER;
			compteur = 0;
		}else if (distance <= 50){
			ordre = TOURNER;
			compteur = 0;
		}
		ReleaseResource(mutexDist);
/*    	V(mutex);*/
	}

    if(ordre==RECULER){
    	//Reculer
    	nxt_motor_set_speed(NXT_PORT_A, -75, 0);
    	nxt_motor_set_speed(NXT_PORT_B, -75, 0);
    	compteur++;
    	if (compteur == 5){
    		ordre = TOURNER;
    	}
    }else if(distance<=50){
    	if (gauche) {
			//Tourner (Droite)
			nxt_motor_set_speed(NXT_PORT_A, 100, 0);
			nxt_motor_set_speed(NXT_PORT_B, 75, 0);
		}else{
			//Tourner (Gauche)
			nxt_motor_set_speed(NXT_PORT_A, 75, 0);
			nxt_motor_set_speed(NXT_PORT_B, 100, 0);
		}
		
    }else{
    	if (periode%10 == 0){
    		//Tourner
    		int random = (rand() % 200) - 100;
			nxt_motor_set_speed(NXT_PORT_A, random, 0);
			nxt_motor_set_speed(NXT_PORT_B, - random, 0);
		
    		
    	}else{
    		//Aller tout droit
    		nxt_motor_set_speed(NXT_PORT_A, 75, 0);
    		nxt_motor_set_speed(NXT_PORT_B, 75, 0);
    	}
    }
	
    periode = (periode+1)%10;
    TerminateTask();
}

void PreTaskHook() {
    int time = systick_get_ms();
    display_goto_xy(0,0);
    display_int(time, 0);
    display_goto_xy(0,1);
    display_int(distance, 0);
    display_string("      "); //Effacer le reste de la ligne
    display_update();
}

void PostTaskHook() {

}

void StartupHook() {
	ecrobot_init_sonar_sensor(NXT_PORT_S2);
	int time = systick_get_ms();
    display_goto_xy(0,0);
    display_int(time, 0);
    display_goto_xy(0,1);
    display_int(distance, 0);
    display_update();
}

/* End of file TP2.c */
