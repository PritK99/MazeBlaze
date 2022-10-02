#include "mazeblaze2.h"

#define WHITE_PATCH 1000 // LSA values that > 1000 are white patch
#define BLACK_PATCH 0    // LSA values that lie between 0 - 1000 are black patch
#define no_of_sensors 6 // total number of sensors in bot

int lsa_reading[no_of_sensors];

// esp_err_t is an unsigned int given by esp for error handling . 0 denotes OK_ESP i.e. success
esp_err_t enable_lsa()
{
    
    mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM2A, 9);
    
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 2000;
    pwm_config.cmpr_a = 0;
    pwm_config.cmpr_b = 0;
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    
    mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_2, &pwm_config);

    adc1_config_width(ADC_WIDTH_BIT_12);

    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_11);
    
    return ESP_OK;
}

esp_err_t set_brightness(int val) {
    /*The timer number runs from 0 to 2 only // mcpwn unit runs from 0 to 1 // */
    mcpwm_set_duty(MCPWM_UNIT_1, MCPWM_TIMER_2, MCPWM_OPR_A, val);
    mcpwm_set_duty_type(MCPWM_UNIT_1, MCPWM_TIMER_2, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
    mcpwm_set_duty(MCPWM_UNIT_1, MCPWM_TIMER_2, MCPWM_OPR_B, 0);
    mcpwm_set_duty_type(MCPWM_UNIT_1, MCPWM_TIMER_2, MCPWM_OPR_B, MCPWM_DUTY_MODE_0);

    return ESP_OK ;
}

int get_raw_lsa() // gets raw readings in range of 400 - 2000 appx
{
    lsa_reading[0] = adc1_get_raw(ADC1_CHANNEL_0);
    lsa_reading[1] = adc1_get_raw(ADC1_CHANNEL_3);
    lsa_reading[2] = adc1_get_raw(ADC1_CHANNEL_6);
    lsa_reading[3] = adc1_get_raw(ADC1_CHANNEL_7);
    lsa_reading[4] = adc1_get_raw(ADC1_CHANNEL_4);
    lsa_reading[5] = adc1_get_raw(ADC1_CHANNEL_5);

    // for (int i = 0; i < no_of_sensors; i++)
    // {
    //     if (lsa_reading[i] > 1000)
    //     {
    //         lsa_reading[i] = WHITE_PATCH;
    //     }
    //     else
    //     {
    //         lsa_reading[i] = BLACK_PATCH;
    //     }
    // }

    for (int i = 0; i < no_of_sensors; i++)
    {
        printf("%d ", lsa_reading[i]);
    }
    printf("\n") ;
    
    return 0;
}