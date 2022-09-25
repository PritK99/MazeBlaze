#include "mazeblaze2.h"
//this code will print the readings of LSA on terminal

void app_main(void)
{
    ESP_ERROR_CHECK(enable_lsa()) ;
    while (1)
    {

        get_raw_lsa() ;

        for (int j = 0 ; j < 99 ; j++)
        {
            set_brightness(j) ;
            vTaskDelay(100/portTICK_PERIOD_MS) ;
        }
        for (int j = 99 ; j >1 ; j--)
        {
            set_brightness(j) ;
            vTaskDelay(100/portTICK_PERIOD_MS) ;
        }
        
        vTaskDelay(100/portTICK_PERIOD_MS) ;
    }
}