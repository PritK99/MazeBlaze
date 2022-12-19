#include "mazeblaze2.h"
#include "tuning_http_server.h"
#include "driver/gpio.h"

/*Variables created for path planning testing at 2*/
int pindex = 1;
#define NO_OF_NODES 100
int dry_run[NO_OF_NODES] = {1, 0}; // To be filled during dry run
int final_run[NO_OF_NODES] = {0};  // after removing redundant values from dry run

float error = 0, prev_error = 0, difference, cumulative_error, correction;
float left_duty_cycle = 0, right_duty_cycle = 0;

float kp = 50, ki = 0.01 , kd = 60;

const int weights[5] = {3, 1, 0, -1, -3};

TaskHandle_t taskhandle1 = NULL;
TaskHandle_t taskhandle2 = NULL;
TaskHandle_t taskhandle3 = NULL;

#define BOOT_BUTTON 0

// MOTOR A0 ---> Right
// MOTOR A1 ---> Left

/*This method involves tuning kp , ki ,kd physically*/
#define GOOD_DUTY_CYCLE 80
#define MIN_DUTY_CYCLE 50
#define MAX_DUTY_CYCLE 85
/*variables which help to decide which turns to take*/
void simplify_path()
{
    int prev_index = 0;
    int prev_value = dry_run[prev_index];

    for (int i = 0; i < NO_OF_NODES; i++)
        if (i == 0) // there is no value to compare yet
        {
            continue;
        }
        else if (dry_run[i] == 0) // 0 refers to the unfilled cells or the cells with redundancy removed
        {
            continue;
        }
        /*path is redundant or not depends upon whether there is a dead end or not i.e. the difference between two consecutive values in dry_run have difference of 4 , but also if the angle accounts to 180 degree */
        else if (fabs(prev_value - dry_run[i]) == 2)
        {
            dry_run[i] = 0;
            dry_run[prev_index] = 0; // we shift our previous index one behind after making the element as 0
            if (prev_index != 0)     // if redundant values are spotted in first two values itself
            {
                prev_index--;
            }
            prev_value = dry_run[prev_index]; // we get the previous value as per the previous index since it is already shifted
        }
        else // This condition means no redundant path was discovered hence we increment the prev_index
        {
            prev_value = dry_run[i];
            do /*this is because prev_index is incremented till it overcomes all the redundant 0's it had created */
            {
                prev_index++;
            } while (dry_run[prev_index] == 0);
        }

    int length_of_path = 0; // j is the index counter for final run

    for (int i = 0; i < NO_OF_NODES; i++)
    {
        if (dry_run[i] == 0) // all 0's are redundant and hence are not included in node
        {
            continue;
        }
        else
        {
            final_run[length_of_path] = dry_run[i];
            length_of_path++;
        }
    }

    // printing the balues to confirm readings
    printf("Simplified path : ");
    for (int i = 0; i < length_of_path; i++)
    {
        printf("%d ", final_run[i]);
    }
}

void calculate_error()
{

    int all_black_flag = 1; // assuming initially all black condition
    float weighted_sum = 0, sum = 0;
    float pos = 0;

    for (int i = 0; i < 5; i++)
    {
        if (lsa_reading[i] > BLACK_PATCH)
        {
            all_black_flag = 0;
        }
        weighted_sum += (float)(weights[i]) * (lsa_reading[i]);
        sum = sum + lsa_reading[i];
    }

    if (sum != 0) // sum can never be 0 but just for safety purposes
    {
        pos = weighted_sum / sum; // This will give us the position wrt line. if +ve then bot is facing left and if -ve the bot is facing to right.
    }

    if (all_black_flag == 1) // If all black then we check for previous error to assign current error.
    {
        if (prev_error > 0)
        {
            error = 3;
        }
        else
        {
            error = -3;
        }
    }
    else
    {
        error = pos;
    }
}
// end of function

void calculate_correction()
{
    error = error * 10;              // we need the error correction in range 0-100 so that we can send it directly as duty cycle paramete
    difference = error - prev_error; // used for calcuating kd
    cumulative_error += error;       // used for calculating ki

    cumulative_error = bound(cumulative_error, -30, 30);               // bounding cumulative_error to avoid the issue of cumulative_error being very large
    correction = kp * error + ki * cumulative_error + kd * difference; // defined in http_server.c

    prev_error = error; // update error
}
// end of function

float bound(float val, float min, float max) // To bound a certain value in range MAX to MIN
{
    if (val > max)
        val = max;
    else if (val < min)
        val = min;
    return val;
}
// end of function

// all booleans
bool only_left = false, left = false, right = false, only_right = false, ll = false;

void line_follow_task(void *arg)
{

    while (1)
    {
        get_raw_lsa(); // funtion that updates the lsa readings

        if ((lsa_reading[0] == 1000) && (lsa_reading[1] == 1000) && (lsa_reading[2] == 1000)) // checks left first
        {
            left = 1;
            vTaskDelay(20 / portTICK_PERIOD_MS);
            get_raw_lsa(); // funtion that updates the lsa readings
            if (left == 1 || right == 1)
            {
                if ((lsa_reading[0] == 1000) && (lsa_reading[1] == 1000) && (lsa_reading[2] == 1000)) // checks left first
                {
                    printf("left flag confirmed\n");
                    left = 1;
                }
                else if (lsa_reading[0] == 0 && lsa_reading[3] == 1000 && lsa_reading[2] == 1000 && lsa_reading[4] == 1000)
                {
                    printf("Right flag confirmed\n");
                    right = true;
                }
                else
                {
                    left = 0;
                    right = 0;
                }
            }
        }
        else if (lsa_reading[0] == 0 && lsa_reading[3] == 1000 && lsa_reading[2] == 1000 && lsa_reading[4] == 1000)
        {
            right = true;
            vTaskDelay(20 / portTICK_PERIOD_MS);
            get_raw_lsa(); // funtion that updates the lsa readings
            if (left == 1 || right == 1)
            {
                if ((lsa_reading[0] == 1000) && (lsa_reading[1] == 1000) && (lsa_reading[2] == 1000)) // checks left first
                {
                    printf("left flag confirmed\n");
                    left = 1;
                }
                else if (lsa_reading[0] == 0 && lsa_reading[3] == 1000 && lsa_reading[2] == 1000 && lsa_reading[4] == 1000)
                {
                    printf("Right flag confirmed\n");
                    right = 1;
                }
                else
                {
                    left = 0;
                    right = 0;
                }
            }
        }

        if (left == 1) // checks left first
        {
            // if (lsa_reading[0] == 0 && lsa_reading[1] == 1000 && lsa_reading[3] == 1000 && lsa_reading[2] == 1000 && lsa_reading[4] == 1000)
            // {
            //     while(lsa_reading[0] == 0 && lsa_reading[1] == 1000 && lsa_reading[3] == 1000 && lsa_reading[2] == 1000 && lsa_reading[4] == 1000)
            //     {
            //         get_raw_lsa() ;
            //         set_motor_speed(MOTOR_A_0, MOTOR_STOP, 0);
            //         set_motor_speed(MOTOR_A_1, MOTOR_STOP, 0);
            //     }
            // }
            int counter = 0;
            while (lsa_reading[0] == 1000 && lsa_reading[1] == 1000)
            {
                get_raw_lsa();
                vTaskDelay(10 / portTICK_PERIOD_MS);
                counter++;
                // printf("%d\n", counter);
                if (counter >= 15 && lsa_reading[3] == 1000 && lsa_reading[2] == 1000 && lsa_reading[1] == 1000)
                {
                    while (1)
                    {
                        set_motor_speed(MOTOR_A_0, MOTOR_STOP, 0);
                        set_motor_speed(MOTOR_A_1, MOTOR_STOP, 0);
                        vTaskDelay(10 / portTICK_PERIOD_MS);
                    }
                }
            }
            // printf("\n");

            vTaskDelay(40 / portTICK_PERIOD_MS);

            get_raw_lsa();

            // printf("%d %d %d %d %d\n", lsa_reading[0], lsa_reading[1], lsa_reading[2], lsa_reading[3], lsa_reading[4]);

            if (lsa_reading[1] == 0 && lsa_reading[3] == 0 && lsa_reading[2] == 0)
            {
                printf("ONLY LEFT DETECTED");
                only_left = true;
            }
            else if (lsa_reading[2] == 1000 && (lsa_reading[1] == 1000 || lsa_reading[3] == 1000))
            {
                printf("STR+LEFT DETECTED");
                only_left = false;
            }
        }
        else if (right == 1)
        {
            // printf("Success 101\n");

            while (lsa_reading[4] == 1000 && lsa_reading[3] == 1000)
            {
                get_raw_lsa();
                vTaskDelay(10 / portTICK_PERIOD_MS);
            }

            vTaskDelay(40 / portTICK_PERIOD_MS);
            get_raw_lsa();
            printf("%d %d %d %d %d\n", lsa_reading[0], lsa_reading[1], lsa_reading[2], lsa_reading[3], lsa_reading[4]);

            if ((lsa_reading[0] == 0 && lsa_reading[1] == 0 && lsa_reading[3] == 0 && lsa_reading[2] == 0 && lsa_reading[4] == 0))
            {
                printf("ONLY RIGHT DETECTED");
                only_right = true;
            }
            else if (lsa_reading[2] == 1000 && (lsa_reading[1] == 1000 || lsa_reading[3] == 1000))
            {
                printf("STR+RIGHT DETECTED");
                only_right = false;
            }
        }

        get_raw_lsa();

        if (left == 1)
        {
            // For taking left we always subtract 1
            if (dry_run[pindex - 1] == 1)
            {
                dry_run[pindex] = 4;
            }
            else if (dry_run[pindex - 1] == 2)
            {
                dry_run[pindex] = 1;
            }
            else if (dry_run[pindex - 1] == 3)
            {
                dry_run[pindex] = 2;
            }
            else
            {
                dry_run[pindex] = 3;
            }
            pindex++;

            while (1)
            {
                get_raw_lsa();

                if (lsa_reading[1] == 0)
                {
                    ll = true;
                }

                set_motor_speed(MOTOR_A_0, MOTOR_FORWARD, 75);
                set_motor_speed(MOTOR_A_1, MOTOR_BACKWARD, 75);

                if (lsa_reading[1] == 1000 && ll)
                {
                    // vTaskDelay(80 / portTICK_PERIOD_MS);
                    set_motor_speed(MOTOR_A_0, MOTOR_STOP, 0);
                    set_motor_speed(MOTOR_A_1, MOTOR_STOP, 0);
                    ll = 0;
                    left = 0;
                    only_left = 0;
                    only_right = 0;
                    right = 0;

                    break;
                }
                vTaskDelay(10 / portTICK_PERIOD_MS);
            }
        }
        // This is the condition for dead end and no right and left
        else if ((right == 0) && (left == 0) && lsa_reading[0] == 0 && lsa_reading[1] == 0 && lsa_reading[3] == 0 && lsa_reading[2] == 0 && lsa_reading[4] == 0)
        {
            // For taking dead_end we subtract 2
            if (dry_run[pindex - 1] == 1)
            {
                dry_run[pindex] = 3;
            }
            else if (dry_run[pindex - 1] == 2)
            {
                dry_run[pindex] = 4;
            }
            else if (dry_run[pindex - 1] == 3)
            {
                dry_run[pindex] = 1;
            }
            else
            {
                dry_run[pindex] = 2;
            }
            pindex++;

            while (lsa_reading[2] == 0)
            {
                get_raw_lsa();

                set_motor_speed(MOTOR_A_0, MOTOR_BACKWARD, 75);
                set_motor_speed(MOTOR_A_1, MOTOR_FORWARD, 75);

                if (lsa_reading[2] == 1000)
                {
                    set_motor_speed(MOTOR_A_0, MOTOR_STOP, 0);
                    set_motor_speed(MOTOR_A_1, MOTOR_STOP, 0);

                    ll = 0;
                    left = 0;
                    only_left = 0;
                    only_right = 0;
                    right = 0;

                    break;
                }

                vTaskDelay(10 / portTICK_PERIOD_MS);
            }
        }
        else if (only_right == 1 && right == 1)
        {
            // For taking dead_end we add 1
            if (dry_run[pindex - 1] == 1)
            {
                dry_run[pindex] = 2;
            }
            else if (dry_run[pindex - 1] == 2)
            {
                dry_run[pindex] = 3;
            }
            else if (dry_run[pindex - 1] == 3)
            {
                dry_run[pindex] = 4;
            }
            else
            {
                dry_run[pindex] = 1;
            }
            pindex++;

            while (lsa_reading[2] == 0)
            {
                get_raw_lsa();

                set_motor_speed(MOTOR_A_0, MOTOR_BACKWARD, 75);
                set_motor_speed(MOTOR_A_1, MOTOR_FORWARD, 75);

                // vTaskDelay(100 / portTICK_PERIOD_MS);

                if (lsa_reading[2] == 1000)
                {
                    set_motor_speed(MOTOR_A_0, MOTOR_STOP, 0);
                    set_motor_speed(MOTOR_A_1, MOTOR_STOP, 0);

                    ll = 0;
                    left = 0;
                    only_left = 0;
                    only_right = 0;
                    right = 0;

                    break;
                }

                vTaskDelay(10 / portTICK_PERIOD_MS);
            }
        }
        // // stop condition req
        // if (lsa_reading[0] == 1000 && lsa_reading[1] == 1000 && lsa_reading[3] == 1000 && lsa_reading[2] == 1000 && lsa_reading[4] == 1000)
        // {
        //     printf("ALL WHITE BOX DETECTED");
        //     while (lsa_reading[0] == 1000 && lsa_reading[1] == 1000 && lsa_reading[3] == 1000 && lsa_reading[2] == 1000 && lsa_reading[4] == 1000)
        //     {
        //         get_raw_lsa();
        //         set_motor_speed(MOTOR_A_0, MOTOR_STOP, 0);
        //         set_motor_speed(MOTOR_A_1, MOTOR_STOP, 0);
        //     }
        // }

        calculate_error();
        calculate_correction();

        left_duty_cycle = bound((GOOD_DUTY_CYCLE - correction), MIN_DUTY_CYCLE, MAX_DUTY_CYCLE);
        right_duty_cycle = bound((GOOD_DUTY_CYCLE + correction), MIN_DUTY_CYCLE, MAX_DUTY_CYCLE);

        set_motor_speed(MOTOR_A_1, MOTOR_FORWARD, left_duty_cycle); /*goes forward in this case*/
        set_motor_speed(MOTOR_A_0, MOTOR_FORWARD, right_duty_cycle);

        vTaskDelay(10 / portTICK_PERIOD_MS);

        printf("The current readings in ARRAY are : ");

        for (int i = 0; i < pindex; i++)
        {
            printf("%d ", dry_run[i]);
        }
        printf("\n");
    }
}

void path_follow_task(void *arg)
{
    printf("oompalompa begins here");

    int final_traversal = 1;
    simplify_path();
    while (1)
    {

        if ((lsa_reading[0] == 1000) && (lsa_reading[1] == 1000) && (lsa_reading[2] == 1000)) // checks left first
        {
            left = 1;
            vTaskDelay(10 / portTICK_PERIOD_MS);
            get_raw_lsa(); // funtion that updates the lsa readings
            if (left == 1 || right == 1)
            {
                if ((lsa_reading[0] == 1000) && (lsa_reading[1] == 1000) && (lsa_reading[2] == 1000)) // checks left first
                {
                    left = 1;
                }
                else if (lsa_reading[0] == 0 && lsa_reading[3] == 1000 && lsa_reading[2] == 1000 && lsa_reading[4] == 1000)
                {
                    right = 1;
                }
                else
                {
                    left = 0;
                    right = 0;
                }
            }
        }
        else if (lsa_reading[0] == 0 && lsa_reading[3] == 1000 && lsa_reading[2] == 1000 && lsa_reading[4] == 1000)
        {
            right = true;
            vTaskDelay(10 / portTICK_PERIOD_MS);
            get_raw_lsa(); // funtion that updates the lsa readings
            if (left == 1 || right == 1)
            {
                if ((lsa_reading[0] == 1000) && (lsa_reading[1] == 1000) && (lsa_reading[2] == 1000)) // checks left first
                {
                    left = 1;
                }
                else if (lsa_reading[0] == 0 && lsa_reading[3] == 1000 && lsa_reading[2] == 1000 && lsa_reading[4] == 1000)
                {
                    right = 1;
                }
                else
                {
                    left = 0;
                    right = 0;
                }
            }
        }

        if (left == 1 || right == 1)
        {
            if (final_run[final_traversal] - final_run[final_traversal - 1] == -1)
            {
            }
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void suspend_resume_tasks(void *arg)
{
    // Task used for suspending task 1 and task 2 when boot button is pressed . Has Higher priority than both of these .

    static bool switcher = 1; // switcher variable used to switch between resume and suspend functions
    bool once1 = 1;
    while (1)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);

        bool boot_button_state = gpio_get_level((gpio_num_t)BOOT_BUTTON); // Gets the state of the boot button . If it is pressed gpio pin becomes low level.

        if (!boot_button_state && switcher && once1)
        { // if tasks are to be suspended

            printf("Boot Button was pressed . Bye World ! All Tasks are suspended \n");
            vTaskSuspend(taskhandle1);
            vTaskSuspend(taskhandle2);
            set_motor_speed(MOTOR_A_0, MOTOR_STOP, 0);
            set_motor_speed(MOTOR_A_1, MOTOR_STOP, 0);
            // Suspend Tasks
            // Wait some time . To prevent immediately switching to resume
            switcher = 0;
            vTaskDelay(3000 / portTICK_PERIOD_MS);
        }
        boot_button_state = gpio_get_level((gpio_num_t)BOOT_BUTTON);
        if (!boot_button_state && !switcher && once1)
        { // if tasks are to be resumed

            printf("Boot Button was pressed . Path planning task has been resumed only \n");

            vTaskResume(taskhandle2); // Resume Tasks

            printf("resumed");

            vTaskDelay(3000 / portTICK_PERIOD_MS); // Wait some time . To prevent immediately switching to suspend
            once1 = false;
        }
    }
}

// end of task
bool once = true;
void app_main()
{
    ESP_ERROR_CHECK(enable_lsa());
    ESP_ERROR_CHECK(enable_motor_driver());

    xTaskCreate(&line_follow_task, "line_follow_task", 4096, NULL, 1, &taskhandle1);
    xTaskCreate(&path_follow_task, "path_follow_task", 4096, NULL, 1, &taskhandle2);
    if (once)
    {
        vTaskSuspend(taskhandle2);
        once = false;
    }
    xTaskCreate(&suspend_resume_tasks, "SRTASK", 2048, NULL, 4, &taskhandle3);
}