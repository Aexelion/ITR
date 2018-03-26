/**
 * @file Wall-e_Tournament.c
 *
 * @section desc File description
 *
 *
 * @section copyright Copyright
 *
 *
 * @section infos File informations
 *
 * $Date$ mar. mars 20 10:33:44 CET 2018
 * $Rev$
 * $Author$ 16009566
 * $URL$ /private/student/6/66/16009566/ITR/Wall-e_Tournament
 */

#include "tpl_os.h"
#include "nxt_motors.h"
#include "ecrobot_interface.h" 
#include "ecrobot_private.h"
#include <stdio.h>
#include <stdlib.h>

#define SEUIL 15
#define SEUIL2 30

#define RECULER 1
#define TOURNER 2
#define AVANCER 3
#define POSITION 4
#define STOP 5

#define Capteur_Contact_Gauche NXT_PORT_S3
#define Capteur_Contact_Droit NXT_PORT_S1
#define Capteur_Distance NXT_PORT_S2

#define Moteur_Gauche NXT_PORT_B
#define Moteur_Droit NXT_PORT_A

#define VITESSE 70


int mutexDist=1;
int distance=1;

int ordre=AVANCER;
int compteur_Recule=0;
int compteur_Position=0;
int compteur_Tourner=0;
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
	int contact1 = ecrobot_get_touch_sensor(Capteur_Contact_Gauche);
	int contact2 = ecrobot_get_touch_sensor(Capteur_Contact_Droit);
	
    if (contact1 || contact2){
/*    	P(mutex);*/
		gauche = contact1;
		GetResource(mutexDist);
    	distance = 0;
    	ReleaseResource(mutexDist);
/*    	V(mutex);*/
		if (ordre == AVANCER){
			ordre = RECULER;
		}
	}
	TerminateTask();
}

TASK(Detection_distance)
{
/*	P(mutex);*/
	GetResource(mutexDist);
	distance = ecrobot_get_sonar_sensor(Capteur_Distance);
	if (distance <= 0){
		distance = 1;
	}
	ReleaseResource(mutexDist);
/*	V(mutex);*/
	if (ordre == AVANCER) {
		if (distance <= SEUIL){
			ordre = POSITION;
		}
	}
	if (ordre == TOURNER){
		if (distance >= SEUIL2){
			ordre = AVANCER;
		}
	}
    TerminateTask();
}

TASK(Navigation)
{
	if (ordre == AVANCER)  {
    	display_goto_xy(0,0);
		display_string("J'avance        ");
    	display_update();
		nxt_motor_set_speed(Moteur_Gauche, VITESSE, 0);
		nxt_motor_set_speed(Moteur_Droit, VITESSE, 0);
	}else if (ordre == RECULER) {
		display_goto_xy(0,0);
		display_string("Je recule       ");
		display_update();
		nxt_motor_set_speed(Moteur_Gauche, -VITESSE, 0);
		nxt_motor_set_speed(Moteur_Droit, -VITESSE, 0);
		compteur_Recule++;
		if (compteur_Recule >= 5){
			compteur_Recule = 0;
			compteur_Tourner = 0;
			ordre = TOURNER;
		}
	}else if (ordre == TOURNER) {
		display_goto_xy(0,0);
		display_string("Je tourne       ");
		if (gauche){
			display_string(" a droite      ");
			display_update();
			nxt_motor_set_speed(Moteur_Gauche, VITESSE, 0);
			nxt_motor_set_speed(Moteur_Droit, -VITESSE, 0);
		}else{
			display_string(" a gauche       ");
			display_update();
			nxt_motor_set_speed(Moteur_Gauche, -VITESSE, 0);
			nxt_motor_set_speed(Moteur_Droit, VITESSE, 0);
		}
		compteur_Tourner++;
		if (compteur_Tourner >= 20) {
			ordre = STOP;
		}
	}else if (ordre == POSITION) {
		display_goto_xy(0,0);
		display_string("Je me positionne");
		if (gauche){
			display_string(" a droite      ");
			display_update();
			nxt_motor_set_speed(Moteur_Gauche, VITESSE, 0);
			nxt_motor_set_speed(Moteur_Droit, -VITESSE, 0);
		}else{
			display_string(" a gauche      ");
			display_update();
			nxt_motor_set_speed(Moteur_Gauche, -VITESSE, 0);
			nxt_motor_set_speed(Moteur_Droit, VITESSE, 0);
		}
		compteur_Position++;
		if (compteur_Position >= 10){
			compteur_Position = 0;
			compteur_Tourner = 0;
			if (gauche){gauche = 0;}
			else{gauche = 1;}
			ordre = TOURNER;
		}
	}else if(ordre == STOP) {
		display_goto_xy(0,0);
		display_string("Je suis arrive");
		display_update();
		nxt_motor_set_speed(Moteur_Gauche, 0, 0);
		nxt_motor_set_speed(Moteur_Droit, 0, 0);
	}
}

void PreTaskHook() {
    display_goto_xy(0,1);
    display_int(distance, 0);
    display_string("      "); //Effacer le reste de la ligne
    display_update();
}

void PostTaskHook() {

}

void StartupHook() {
	ecrobot_init_sonar_sensor(Capteur_Distance);
    display_goto_xy(0,1);
    display_int(distance, 0);
    display_update();
}

/* End of file Wall-e_Tournament.c */
