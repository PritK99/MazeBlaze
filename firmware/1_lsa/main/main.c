#include "mazeblaze2.h"
//this code will print the readings of LSA on terminal

void app_main(void)
{
    ESP_ERROR_CHECK(enable_lsa()) ;
    set_brightness(100);

    while (1)
    {
        get_raw_lsa() ;
    }
}