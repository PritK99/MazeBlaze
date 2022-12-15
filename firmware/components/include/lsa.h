#ifndef LSA_H 
#define LSA_H 

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

#define WHITE_PATCH 1000 // LSA values that > 1000 are white patch
#define BLACK_PATCH 0    // LSA values that lie between 0 - 1000 are black patch
#define no_of_sensors 5 // total number of sensors in bot

int lsa_reading[no_of_sensors];



esp_err_t enable_lsa() ;
int get_raw_lsa() ;


#endif 
