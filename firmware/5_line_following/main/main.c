#include <stdio.h>
#include "mazeblaze2.h"

int actiavate_left_counter =0;
int actiavate_right_counter =0;
char Turn ;

while(1)
{
    get_raw_lsa() ;

    if (lsa_reading[0] == 1)
    {
        actiavate_left_counter ++ ;
        set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ;//Reduce pwm to 65 to slow down to bot 
        set_motor_speed(MOTOR_A_1 , MOTOR_FORWARD , 65) ;
    }
    if (lsa_reading[4] == 1)
    {
        activate_right_counter ++ ;
        set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ;//Reduce pwm to 65 to slow down to bot 
        set_motor_speed(MOTOR_A_1 , MOTOR_FORWARD , 65) ;
    }

    if (actiavate_left_counter > 0 && lsa_reading[5] == 1)
    {
        Turn = 'L' ;
        do
        {
            set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ;
            set_motor_speed(MOTOR_A_1 , MOTOR_BACKWARD , 65) ;
        }
        while (lsa_reading[1] == lsa_reading[2] == lsa_reading[3] == 1) ;
    }
    else if (lsa_reading[1] == lsa_reading[2] == lsa_reading[3] == 1 && )
    if (actiavate_right_counter > 0 && lsa_reading[6] == 1)
    {
        Turn = 'R' ;
        do
        {
            set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ;
            set_motor_speed(MOTOR_A_1 , MOTOR_BACKWARD , 65) ;
        }
        while (lsa_reading[1] == lsa_reading[2] == lsa_reading[3] == 1) ;
    }         
    
}
