#include "mazeblaze2.h"
/* 1 - west , 2 - north , 3 - east , 4 - south */

int actiavate_left_counter =0;
int actiavate_right_counter =0;
int Turn ;
int prev_Turn ;
int prev_lsa1_counter = 0 ; 
int prev_lsa3_counter = 0 ; 
bool straight_possible = false ;

void app_main(void)
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
            Turn = circular_defn(-2) ;
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
            Turn = circular_defn(0) ;
            set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 75) ;//Increase PWM since we have to go straight 
            set_motor_speed(MOTOR_A_1 , MOTOR_FORWARD , 75) ;
        }
        else if (actiavate_right_counter > 0 && lsa_reading[6] == 1)
        {
            Turn = circular_defn(+2) ;
            do
            {
                set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ;
                set_motor_speed(MOTOR_A_1 , MOTOR_BACKWARD , 65) ;
            }
            while (lsa_reading[1] == lsa_reading[2] == lsa_reading[3] == 1) ;
        } 
        else
        {
            Turn = circular_defn(-4) ;
            set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ;//Increase PWM since we have to go straight 
            set_motor_speed(MOTOR_A_1 , MOTOR_BACKWARD , 65) ;
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
        
    }
}

//In LFR , we are limiting our use to directions 1 , 3 , 5 ,7 and we plan to get angle via encoders only 
//For DFS , we will use all 8 directions
int circular_defn(int change_in_dir)
{
    /*The use of this function is have a circular defination from 1 to 8 
    1 - W , 2 - NW , 3 - N , 4 - NE , 5 - E , 6 - SE , 7 - S , 8 - SW 
    The difference of 4 stands for u turn i.e. dead end . this will help us to delete that path from char array 
    subtracting 4 from 1 will give 5 according tio this function */
    int temp = Turn ;
    Turn = prev_Turn + change_in_dir ; //for LFR , change_in_dir = -2 means left
    prev_Turn = temp ; 

    if (Turn <= 0)
    {
        Turn = Turn + 8 ;
        /*fpr eg . if you were moving in west i.e. 1 and then you were to take left . As per LFR , your new direction is 1 - 2 = -1 ;
        now -1 + 8 is 7 which is 7 . Now using LFR you are facing lsouth which is also 7*/
    }
    if (Turn <= 0)
    {
        Turn = Turn - 8 ;
        /*fpr eg . if you were moving in south i.e. 7 and then you were to take right . As per LFR , your new direction is 7 + 2 = 9 ;
        now 9 - 8 is 1 which is 1 . Now using LFR you are facing west which is also 1*/
    }

}