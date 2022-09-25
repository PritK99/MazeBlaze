#ifndef LSA_H 
#define LSA_H 

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"


esp_err_t enable_lsa() ;
void set_brightness(int val) ;
int get_raw_lsa() ;

#endif 
