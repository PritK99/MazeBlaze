#include "mazeblaze2.h"
/* 1 - west , 2 - north , 3 - east , 4 - south */

int actiavate_left_counter =0;
int actiavate_right_counter =0;
char Turn ;
int prev_lsa1_counter = 0 ; 
int prev_lsa3_counter = 0 ; 
bool straight_possible = false ;

void turning()
{
    while(1)
    {
        get_raw_lsa() ;

        /* This part detects if there is a node and if there is , it reduces the speed to pwm 65 and works on counters*/
        if (lsa_reading[0] == 1) /*A path in left detected once */
        {
            if (lsa_readings[1] != prev_lsa1_counter)
            {
                actiavate_left_counter ++ ;
                prev_lsa1_counter = lsa_reading[0] ;
            }
            set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ;//Reduce pwm to 65 to slow down to bot 
            set_motor_speed(MOTOR_A_1 , MOTOR_FORWARD , 65) ;
        }
        if (lsa_reading[4] == 1) /*A path in right detected first*/
        {
            if (lsa_readings[3] != prev_lsa3_counter)
            {
                actiavate_right_counter ++ ;
                prev_lsa1_counter = lsa_reading[3] ;
            }
            set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ;//Reduce pwm to 65 to slow down to bot 
            set_motor_speed(MOTOR_A_1 , MOTOR_FORWARD , 65) ;
        }
        if( (actiavate_left_counter > 0 )&& (actiavate_right_counter > 0 )&& ( lsa_readings[1] = lsa_reading[2] == lsa_reading[3] == 1 )) /*straight does exsist even though left or right counter are incremented*/
        {
            straight_possible = true ;
        }

        /*This part of the code takes action on the basis of the node detected*/
        if (actiavate_left_counter > 0 && lsa_reading[5] == 1)
        {
            Turn = 1 ;
            do
            {
                set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ; 
                set_motor_speed(MOTOR_A_1 , MOTOR_BACKWARD , 65) ;
                if (lsa_readings[3] != prev_lsa3_counter)
                    {
                        actiavate_right_counter = actiavate_left_counter ; 
                        prev_lsa1_counter = lsa_reading[3] ;
                    }
            }
            while (lsa_reading[1] == lsa_reading[2] == lsa_reading[3] == 1 && actiavate_left_counter == 1) ;
        }
        else if ( straight_possible )
        {
            set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 75) ;//Increase PWM since we have to go straight 
            set_motor_speed(MOTOR_A_1 , MOTOR_FORWARD , 75) ;
        }
        else if (actiavate_right_counter > 0 && lsa_reading[6] == 1)
        {
            Turn = 3 ;
            do
            {
                set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ;
                set_motor_speed(MOTOR_A_1 , MOTOR_BACKWARD , 65) ;
            }
            while (lsa_reading[1] == lsa_reading[2] == lsa_reading[3] == 1) ;
        } 
        else
        {
            set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ;//Increase PWM since we have to go straight 
            set_motor_speed(MOTOR_A_1 , MOTOR_BACKWARD , 65) ;
        }        
        
    }
}