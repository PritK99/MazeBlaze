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
#define no_of_sensors 6 // total number of sensors in bot

int lsa_reading[no_of_sensors];

#define ADC1_GPIO36_CHANNEL     ADC1_CHANNEL_0
#define ADC1_CHANNEL_0_GPIO_NUM 36

#define ADC1_GPIO37_CHANNEL     ADC1_CHANNEL_1
#define ADC1_CHANNEL_1_GPIO_NUM 37

#define ADC1_GPIO38_CHANNEL     ADC1_CHANNEL_2
#define ADC1_CHANNEL_2_GPIO_NUM 38

#define ADC1_GPIO39_CHANNEL     ADC1_CHANNEL_3
#define ADC1_CHANNEL_3_GPIO_NUM 39

#define ADC1_GPIO32_CHANNEL     ADC1_CHANNEL_4
#define ADC1_CHANNEL_4_GPIO_NUM 32

#define ADC1_GPIO33_CHANNEL     ADC1_CHANNEL_5
#define ADC1_CHANNEL_5_GPIO_NUM 33

#define ADC1_GPIO34_CHANNEL     ADC1_CHANNEL_6
#define ADC1_CHANNEL_6_GPIO_NUM 34

#define ADC1_GPIO35_CHANNEL     ADC1_CHANNEL_7
#define ADC1_CHANNEL_7_GPIO_NUM 35

#define WHITE_PATCH 1 // LSA values that > 1000 are white patch
#define BLACK_PATCH 0    // LSA values that lie between 0 - 1000 are black patch
#define no_of_sensors 6 // total number of sensors in bot

int lsa_reading[no_of_sensors];
esp_err_t enable_lsa() ;
esp_err_t set_brightness(int val) ;
int get_raw_lsa() ;


#endif 
