/*Understanding the flow of code :-
Here we create two tasks , namely line follow task and turn task simultanously .
The line follow task starts by getting the raw LSA values and then calculating the correction if any . Then it feeds this correction to motor by passing argument to set_motor_speed() . It also executes turning when a node is spotted .
The front LSA is used to note that a path exists and the back sensors are used to detect when to take sharp turn */

/*Dirction referance :-
1 - W
2 - NW
3 - N
4 - NE 
5 - E
6 - SE 
7 - S
8 - SW */

#include <stdio.h>
#include "mazeblaze2.h"
#include "line_following.h"
#include "tuning_http_server.h"

#define GOOD_DUTY_CYCLE 67
#define MIN_DUTY_CYCLE 55
#define MAX_DUTY_CYCLE 80
#define NO_OF_NODES 150

int actiavate_left_counter =0;
int actiavate_right_counter =0;
int Turn ;
float error=0, prev_error=0, difference, cumulative_error, correction; 
float left_duty_cycle = 0, right_duty_cycle = 0;
int prev_Turn ;
int prev_lsa1_counter = 0 ; 
int prev_lsa3_counter = 0 ; 
bool straight_possible = false ;

int dry_run[NO_OF_NODES] = {0} ;; //To be filled during dry run
/* Dry_run will hold only 4 types of values , i.e. 1 for West , 3 for North , 5 for East , 7 for South */
int final_run[50] = {0}; //after removing redundant values from dry run
int degree[50] ; //This contains angles taken at node 
int degree_index = 0 ; //This contains index for degree array
int turn_index = 0 ;   //This contains index for turns taken
float angle ;

void circular_defn(int change_in_dir) ;
float bound(float val, float min, float max) ;
void calculate_correction() ;
void calculate_error() ;
void confine_angle () ;
void get_shortest_path() ;

void turn_task(void *arg)
{
    while(1)
    {
        get_raw_lsa() ;

        /* This part detects if there is a node and if there is , it reduces the speed to pwm 65 and works on counters*/
        if (lsa_reading[0] == 1) /*A path in left detected once */
        {
            if (lsa_reading[1] != prev_lsa1_counter)
            {
                actiavate_left_counter ++ ;
                prev_lsa1_counter = lsa_reading[0] ;
            }
            set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ;//Reduce pwm to 65 to slow down to bot 
            set_motor_speed(MOTOR_A_1 , MOTOR_FORWARD , 65) ;
        }
        if (lsa_reading[4] == 1) /*A path in right detected first*/
        {
            if (lsa_reading[3] != prev_lsa3_counter)
            {
                actiavate_right_counter ++ ;
                prev_lsa1_counter = lsa_reading[3] ;
            }
            set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ;//Reduce pwm to 65 to slow down to bot 
            set_motor_speed(MOTOR_A_1 , MOTOR_FORWARD , 65) ;
        }
        if( (actiavate_left_counter > 0 )&& (actiavate_right_counter > 0 )&& ( lsa_reading[1] == 1) && ( lsa_reading[2] == 1 ) && ( lsa_reading[3] == 1 )) /*straight does exsist even though left or right counter are incremented*/
        {
            straight_possible = true ;
        }

        /*This part of the code takes action on the basis of the node detected*/
        if (actiavate_left_counter > 0 && lsa_reading[5] == 1)
        {
            circular_defn(-2) ;     //for turning left , we do -2 
            do
            {
                set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ; 
                set_motor_speed(MOTOR_A_1 , MOTOR_BACKWARD , 65) ;
                if (lsa_reading[3] != prev_lsa3_counter)
                    {
                        actiavate_right_counter = actiavate_left_counter ; 
                        prev_lsa1_counter = lsa_reading[3] ;
                    }
            }
            while (( lsa_reading[1] == 1) && ( lsa_reading[2] == 1 ) && ( lsa_reading[3] == 1 ) && actiavate_left_counter == 1) ;
        }
        else if ( straight_possible )
        {
            circular_defn(0) ;      //no change in direction
            set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 75) ;//Increase PWM since we have to go straight 
            set_motor_speed(MOTOR_A_1 , MOTOR_FORWARD , 75) ;
        }
        else if (actiavate_right_counter > 0 && lsa_reading[6] == 1)
        {
            circular_defn(+2) ;     //for turning right , we do +2 
            do
            {
                set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ;
                set_motor_speed(MOTOR_A_1 , MOTOR_BACKWARD , 65) ;
            }
            while ( (lsa_reading[1] == 1) && ( lsa_reading[2] == 1 ) && ( lsa_reading[3] == 1 )) ;
        } 
        else
        {
            circular_defn(-4) ;
            set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ;//Increase PWM since we have to go straight 
            set_motor_speed(MOTOR_A_1 , MOTOR_BACKWARD , 65) ;
            do
            {
                set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 65) ; 
                set_motor_speed(MOTOR_A_1 , MOTOR_BACKWARD , 65) ;
                if (lsa_reading[3] != prev_lsa3_counter)
                    {
                        actiavate_right_counter = actiavate_left_counter ; 
                        prev_lsa1_counter = lsa_reading[3] ;
                    }
            }
            while (( lsa_reading[1] == 1) && ( lsa_reading[2] == 1 ) && ( lsa_reading[3] == 1 ) && actiavate_left_counter == 1) ;
        }        
        
    }
    vTaskDelete(NULL);
}

void line_follow_task(void* arg)
{
    ESP_ERROR_CHECK(enable_lsa()) ;
    ESP_ERROR_CHECK(enable_motor_driver()) ;
    
    while(1)
    {
        get_raw_lsa() ;

        calculate_error();
        calculate_correction();

        left_duty_cycle = bound((GOOD_DUTY_CYCLE - correction), MIN_DUTY_CYCLE , MAX_DUTY_CYCLE);
        right_duty_cycle = bound((GOOD_DUTY_CYCLE + correction), MIN_DUTY_CYCLE , MAX_DUTY_CYCLE);

        set_motor_speed(MOTOR_A_0, MOTOR_FORWARD, left_duty_cycle);
        set_motor_speed(MOTOR_A_1, MOTOR_FORWARD, right_duty_cycle);

        /*prints on terminal*/
        ESP_LOGI("debug","left_duty_cycle:  %f    ::  right_duty_cycle :  %f  :: error :  %f  correction  :  %f  \n",left_duty_cycle, right_duty_cycle, error, correction);
        ESP_LOGI("error_correction", "KP: %f ::  KI: %f  :: KD: %f", read_pid_const().kp, read_pid_const().ki, read_pid_const().kd);

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

void app_main()
{
    xTaskCreate(&turn_task, "line_follow_task", 4096, NULL, 1, NULL);    //creating a task to start line following
    xTaskCreate(&line_follow_task, "line_follow_task", 4096, NULL, 1, NULL);    //creating a task to start line following
    start_tuning_http_server();    
}

/********************************Function Definations here**************************************/

/*
possible cases of errors :-
1) All the front sensor read black
2) if lsa_reading [1] != lsa_reading [2] != lsa_reading [3]

error to right of line is positive while left of line is negative
*/
void calculate_error()
{
    if (lsa_reading[1] == 0 && lsa_reading[2] == 0 && lsa_reading[3] == 0 )
    {

        if(prev_error > 0) //we use prev_error to extract the sign to give to error
        {
            error = 2.5;    //arbitrary value
        }
        else 
        {
            error = -2.5;   //arbitrary value
        }
    }
    else if (lsa_reading[1] == 0 && lsa_reading[3] == 1000)
    {
        /* turn right to nullify */
        error = 1 ;     //arbitrary value
    }
    else if (lsa_reading[1] == 1000 && lsa_reading[3] == 0)
    {
        /* turn left to nullify */
        error = -1 ;    //arbitrary value
    }
    else if (lsa_reading[1] == 1000 && lsa_reading[2] == 1000 && lsa_reading[3] == 1000)
    {
        /* no error */
        error = 0 ;     //arbitrary value
    }
    else
    { 
        error = prev_error ; //this case is for safety
    }   
}

void calculate_correction()
{
    error = error*10;  // we need the error correction in range 0-100 so that we can send it directly as duty cycle paramete
    difference = error - prev_error;    //used for calcuating kd 
    cumulative_error += error;     //used for calculating ki

    cumulative_error = bound(cumulative_error, -30, 30); //bounding cumulative_error to avoid the issue of cumulative_error being very large

    correction = read_pid_const().kp*error + read_pid_const().ki*cumulative_error + read_pid_const().kd*difference; //defined in http_server.c

    prev_error = error; //update error
}

void circular_defn(int change_in_dir)
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

    dry_run[turn_index ++] = Turn ; //puts the new value of turn in dry_run and then increments the index 
}

float bound(float val, float min, float max) //To bound a certain value in range MAX to MIN 
{
    if (val > max)
        val = max;
    else if (val < min)
        val = min;
    return val;
}

void confine_angle ()
//an error of 15 degrees can be tolerated
{
    if (fabs(angle-45) <= 15  ) 
    {
        degree[degree_index ++ ] = 45 ; //puts the new value of turn in degree_index and then increments the index 
    }
    if (fabs(angle-90) <= 15  ) 
    {
        degree[degree_index ++ ] = 90 ;
    }
    if (fabs(angle-135) <= 15  ) 
    {
        degree[degree_index ++ ] = 135 ;
    }
    if (fabs(angle-180) <= 15  ) 
    {
        degree[degree_index ++ ] = 135 ;
    }
}

void get_shortest_path()
{
    int prev_index = 0;
    int prev_value = dry_run[prev_index];

    for (int i = 0; i < NO_OF_NODES ; i++)
        if ( i == 0 )   //there is no value to compare yet
        {
            continue ;
        }
        else if (dry_run[i] == 0)	// 0 refers to the unfilled cells or the cells with redundancy removed 
        {
            continue;
        }
        /*path is redundant or not depends upon whether there is a dead end or not i.e. the difference between two consecutive values in dry_run have difference of 4 , but also if the angle accounts to 180 degree */
        else if (fabs(prev_value - dry_run[i]) == 4)
        {
            dry_run[i] = 0;
            dry_run[prev_index] = 0;	//we shift our previous index one behind after making the element as 0
            if ( prev_index != 0 ) //if redundant values are spotted in first two values itself
            {
                prev_index -- ;
            }
            prev_value = dry_run[prev_index];	//we get the previous value as per the previous index since it is already shifted
        }
        else		//This condition means no redundant path was discovered hence we increment the prev_index
        {
            prev_value = dry_run[i];
            do  /*this is because prev_index is incremented till it overcomes all the redundant 0's it had created */
            {
            prev_index++;
            }
            while (dry_run[prev_index] == 0) ;
        }

    int length_of_path = 0 ; // j is the index counter for final run

    for (int i = 0 ; i < NO_OF_NODES ; i++)
    {
        if (dry_run[i] == 0 )   //all 0's are redundant and hence are not included in node
        {
            continue;
        }
        else 
        {
            final_run[length_of_path] = dry_run[i] ;
            length_of_path++ ;
        }
    }

    /*printing the balues to confirm readings
    for (int i = 0 ; i < length_of_path ; i++ )
    {
        printf("%d " , final_run[i]) ;
    }*/
}