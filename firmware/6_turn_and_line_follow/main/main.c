/*for now , this code deals with LFR only in dry run */
#include "mazeblaze2.h"
/*This method involves tuning kp , ki ,kd physically

some obseravations :-
1) When both motors are in MOTOR_FORWARD , it takes left turn . 
2) When A_0 is FORWARD and A_1 is BACKWARD , it goes staright etc
*/

/*Dirction referance :-
1 - W
2 - NW
3 - N
4 - NE 
5 - E
6 - SE 
7 - S
8 - SW */

#define GOOD_DUTY_CYCLE 55
#define MIN_DUTY_CYCLE 70
#define MAX_DUTY_CYCLE 85
#define LEFT_DETECTION_DUTY_CYCLE 60 

#define NO_OF_NODES 150

int Turn ;
float error=0, prev_error=0, difference, cumulative_error, correction; 
float left_duty_cycle = 0, right_duty_cycle = 0;
int prev_Turn = 3 ; //starts from north
//we dont need a right sensor prev reading since we are using left follow
bool is_end = false ;

int dry_run[NO_OF_NODES] = {0} ;; //To be filled during dry run
/* Dry_run will hold only 4 types of values , i.e. 1 for West , 3 for North , 5 for East , 7 for South */
int final_run[NO_OF_NODES] = {3 , 0}; //after removing redundant values from dry run and the first value is hardcoded as 3 since it north as start always
int skip_array[NO_OF_NODES] ; //this array is only employed for multiple left turn situation
int turn_index = 0 ;   //This contains index for turns taken
int final_run_index = 1 ; //final_run[0] is already 3 which stands for north 
int prev_in_final_run;
int left_counter = 0 ;
int prev_lsa0_value = 0 ;

void circular_defn(int change_in_dir) ;
float bound(float val, float min, float max) ;
void calculate_correction() ;
void calculate_error() ;
void get_shortest_path() ;
void led_blink_at_end() ;

void turn_task(void *arg)
{
    while(1)
    {

        if (lsa_reading[0] == 1000 && lsa_reading[1] == 1000) /*A path in left detected once */
        {
            circular_defn(-2) ;     //for turning left , we do -2 , at whichever angle it maybe 

            left_counter ++ ;

            /*get all the paths possible in left*/
            for (int i = 0 ; i < 5000 ; i++) /*5000 is a randome value choosen on trial . our aim is that bot goes atleast 2 cm forward*/
            {
                set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , LEFT_DETECTION_DUTY_CYCLE ) ;
                set_motor_speed(MOTOR_A_1 , MOTOR_FORWARD , LEFT_DETECTION_DUTY_CYCLE ) ;
                if (lsa_reading[0] != prev_lsa0_value) /*records change in values i.e. increments if there is any difference between 2 values*/
                {
                    left_counter ++ ;
                    prev_lsa0_value = lsa_reading[0] ;
                }
            }
            /*keep turning left untill you find the leftmost path*/
            do
            {
                /*both motors forward means the bot will go to left*/
                set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , GOOD_DUTY_CYCLE ) ;
                set_motor_speed(MOTOR_A_1 , MOTOR_FORWARD , GOOD_DUTY_CYCLE ) ;

                if (lsa_reading[0] != prev_lsa0_value) /*records change in values i.e. increments if there is any difference between 2 values*/
                {
                    left_counter -- ;
                    prev_lsa0_value = lsa_reading[0] ;
                }
            }
            while (( lsa_reading[1] == 1000) && ( lsa_reading[2] == 1000 ) && ( lsa_reading[3] == 1000 ) && left_counter <= 2  ) ;
            
            
        }
        if( ( lsa_reading[1] == 1000) && ( lsa_reading[2] == 1000 ) && ( lsa_reading[3] == 1000 )) /*straight does exist even though left or right counter are incremented*/
        {
            circular_defn(0) ;      //no change in direction
            set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , GOOD_DUTY_CYCLE) ;//Increase PWM since we have to go straight 
            set_motor_speed(MOTOR_A_1 , MOTOR_BACKWARD , GOOD_DUTY_CYCLE) ;
        }
        if (lsa_reading[4] == 1000) /*A path in right detected first*/
        {
            circular_defn(+2) ;     //for turning right , we do +2 
            do
            {
                set_motor_speed(MOTOR_A_0 , MOTOR_BACKWARD , GOOD_DUTY_CYCLE) ;
                set_motor_speed(MOTOR_A_1 , MOTOR_BACKWARD , GOOD_DUTY_CYCLE) ;
            }
            while ( (lsa_reading[1] == 1000) && ( lsa_reading[2] == 1000 ) && ( lsa_reading[3] == 1000 )) ;
        }
        else if ( (lsa_reading[0] == 0) && ( lsa_reading[1] == 0 ) && ( lsa_reading[2] == 0 ) && (lsa_reading[3] == 0) && ( lsa_reading[4] == 0 ) )
        {
            circular_defn(-4) ;     //for turning right , we do +4 or -4

            do
            {
                /*both motors forward means the bot will go to left*/
                set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , GOOD_DUTY_CYCLE) ;
                set_motor_speed(MOTOR_A_1 , MOTOR_FORWARD , GOOD_DUTY_CYCLE) ;
            }
            while (( lsa_reading[1] == 1000) && ( lsa_reading[2] == 1000 ) && ( lsa_reading[3] == 1000 ) ) ;
        } 
        else if ((lsa_reading[0] == 1000) && ( lsa_reading[1] == 1000 ) && ( lsa_reading[2] == 1000 ) && (lsa_reading[3] == 1000) && ( lsa_reading[4] == 1000 ))
        {
            set_motor_speed(MOTOR_A_0 , MOTOR_STOP , GOOD_DUTY_CYCLE) ;
            set_motor_speed(MOTOR_A_1 , MOTOR_STOP , GOOD_DUTY_CYCLE) ;
            led_blink_at_end() ;
        }
        else //This is a safety case  where the bot will move round and round in search of line 
        {
            do
            {
                /*both motors forward means the bot will go to left*/
                set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , GOOD_DUTY_CYCLE ) ;
                set_motor_speed(MOTOR_A_1 , MOTOR_FORWARD , GOOD_DUTY_CYCLE ) ;
            }
            while (( lsa_reading[1] == 1000) && ( lsa_reading[2] == 1000 ) && ( lsa_reading[3] == 1000 ) ) ;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);      
        
    }
    vTaskDelete(NULL);
}

void line_follow_task(void *arg)
{

    while (1)
    {
        get_raw_lsa();

        calculate_error();
        calculate_correction();

        left_duty_cycle = bound((GOOD_DUTY_CYCLE - correction), MIN_DUTY_CYCLE, MAX_DUTY_CYCLE);
        right_duty_cycle = bound((GOOD_DUTY_CYCLE + correction), MIN_DUTY_CYCLE, MAX_DUTY_CYCLE);
   
        set_motor_speed(MOTOR_A_0, MOTOR_FORWARD, left_duty_cycle);    /*turns to left or right depending upon sign of error . If error is +ve , whch means bot is more inclined towards left , right_duty_cycle > left_duty_cycle which implies bot will take right turn and vice versa*/
        set_motor_speed(MOTOR_A_1, MOTOR_FORWARD, right_duty_cycle);

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}
// end of task

void app_main()
{
    ESP_ERROR_CHECK(enable_lsa());
    ESP_ERROR_CHECK(enable_motor_driver());

    xTaskCreate(&turn_task, "turn_task", 4096, NULL, 1, NULL); // creating a task to start line following
    xTaskCreate(&line_follow_task, "line_follow_task", 4096, NULL, 1, NULL); // creating a task to start line following
}
// end of main

void calculate_error()
{
    /*
    possible cases of errors :-
    1) All the front sensor read black - this means the bot is entirely out of path
    2) if lsa_reading [1] != lsa_reading [2] != lsa_reading [3] != 1 - this means bot is slightly out of path

    error to left of line is +ve while right of line is -ve ----->  +ve |  | -ve
    */
    if (lsa_reading[0] == 0 && lsa_reading[1] == 0 && lsa_reading[2] == 0 && lsa_reading[3] == 0 && lsa_reading[4] == 0)
    {
        /*All sensore read black*/
        if (prev_error >= 0) // we use prev_error to extract the sign to give to error
        {
            error = 2.5; // arbitrary value
        }
        else
        {
            error = -2.5; // arbitrary value
        }
    }
    else if (lsa_reading[1] == 0 && lsa_reading[3] == 1000)
    {
        /* turn right to nullify */
        error = -1; // arbitrary value
    }
    else if (lsa_reading[1] == 1000 && lsa_reading[3] == 0)
    {
        /* turn left to nullify */
        error = 1; // arbitrary value
    }
    else if (lsa_reading[1] == 1000 && lsa_reading[2] == 1000 && lsa_reading[3] == 1000 && lsa_reading[0] == 0 && lsa_reading[4] == 0)
    {
        /* no error since the bot is on line */
        error = 0;
    }
    else
    {
        error = 0; // this case is for safety
    }
}
// end of function

void calculate_correction()
{
    error = error * 10;              // we need the error correction in range 0-100 so that we can send it directly as duty cycle parameter
    difference = error - prev_error; // used for calcuating kd
    cumulative_error += error;       // used for calculating ki

    cumulative_error = bound(cumulative_error, -30, 30); // bounding cumulative_error to avoid the issue of cumulative_error being very large

    float kp , ki , kd ;
    kp = 3 ;
    kd = 1 ;
    ki = 0 ; 

    correction = kp* error + ki*cumulative_error + kd*difference; 

    prev_error = error; // update error
}
// end of function

float bound(float val, float min, float max) //To bound a certain value in range MAX to MIN 
{
    if (val > max)
        val = max;
    else if (val < min)
        val = min;
    return val;
}
//end of function

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

    dry_run[turn_index++] = Turn ;
}
//end of function

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
}
//end of function

void led_blink_at_end() /*The led is supposed to glow at the end of maze*/
{
    is_end = true ;

    set_motor_speed(MOTOR_A_0 , MOTOR_STOP , 60) ;
    set_motor_speed(MOTOR_A_0 , MOTOR_STOP , 60) ;

    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << END_LED);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    get_shortest_path() ; //once we have reached end , we start to calculate the shortest path

    for (int i = 0 ; i < 1000 ; i++)
    {
    gpio_set_level(END_LED, 1);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    }

}
//end of function