#include "mazeblaze2.h"

void app_main (void)
{

    ESP_ERROR_CHECK(enable_motor_driver()) ;
    ESP_ERROR_CHECK(enable_lsa());
    set_brightness(95);

    
    //code for turning - clockwise
    while(1)
    {
        get_raw_lsa();

        if (lsa_reading[1] == 1000 && lsa_reading[2] == 1000 && lsa_reading[3] == 1000  )
    {
        set_motor_speed(MOTOR_A_0 , MOTOR_BACKWARD, 90 ) ;
        set_motor_speed(MOTOR_A_1 , MOTOR_FORWARD , 90 ) ;
    }
    else
    {
        set_motor_speed(MOTOR_A_0 , MOTOR_STOP, 100) ;
        set_motor_speed(MOTOR_A_1 , MOTOR_STOP , 100 ) ;
    }

        vTaskDelay(100/ portTICK_RATE_MS) ;
    }

    // //Code for acceleration and retardation
    // for ( int i = 1 ; i < 99 ; i++ ) //acceleration
    // {   
    //     set_motor_speed(MOTOR_A_0 , 1 , i) ;
    //     set_motor_speed(MOTOR_A_1 , 1 , i) ;
    //     vTaskDelay(100/ portTICK_RATE_MS) ;
    // }
    // for ( int i = 99 ; i > 1 ; i-- ) //retardation
    // {   
    //     set_motor_speed(MOTOR_A_0 , 1 , i) ;
    //     set_motor_speed(MOTOR_A_1 , 1 , i) ;
    //     vTaskDelay(100/ portTICK_RATE_MS) ;
    // }

    // //idea to eliminate the need of encoders
    
    // // set_motor_speed(MOTOR_A_0 , MOTOR_FORWARD , 70) ;
    // // set_motor_speed(MOTOR_A_1 , MOTOR_BACKWARD , 70 ) ;
    // // vTaskDelay(2060/ portTICK_RATE_MS) ;
    // // set_motor_speed(MOTOR_A_0 , MOTOR_STOP , 70) ;
    // // set_motor_speed(MOTOR_A_1 , MOTOR_STOP , 70 ) ;

}
