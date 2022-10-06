#include "mazeblaze2.h"

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
    
    return ESP_OK;
}

esp_err_t set_brightness(int val) {
    /*The timer number runs from 0 to 2 only and mcpwn unit runs from 0 to 1 
    timer 0 and timer 1 are used by motors*/
    mcpwm_set_duty(MCPWM_UNIT_1, MCPWM_TIMER_2, MCPWM_OPR_A, val);
    mcpwm_set_duty_type(MCPWM_UNIT_1, MCPWM_TIMER_2, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
    mcpwm_set_duty(MCPWM_UNIT_1, MCPWM_TIMER_2, MCPWM_OPR_B, 0);
    mcpwm_set_duty_type(MCPWM_UNIT_1, MCPWM_TIMER_2, MCPWM_OPR_B, MCPWM_DUTY_MODE_0);

    return ESP_OK ;
}

int get_raw_lsa() // gets raw readings in range of 2900 - 4095 appx
{
    //while (1) infinite loop should not be insterted here , since then it wont allow other functions to run 
    lsa_reading[0] = adc1_get_raw(ADC1_CHANNEL_0);
    lsa_reading[1] = adc1_get_raw(ADC1_CHANNEL_3);
    lsa_reading[2] = adc1_get_raw(ADC1_CHANNEL_6);
    lsa_reading[3] = adc1_get_raw(ADC1_CHANNEL_7);
    lsa_reading[4] = adc1_get_raw(ADC1_CHANNEL_4);
    
    /*Our bot reads 4095 as BLACK and rest readings as WHITE*/
    for (int i = 0; i < no_of_sensors; i++)
    {
        if (lsa_reading[i] == 4095)
        {
            lsa_reading[i] = BLACK_PATCH;
        }
        else
        {
            lsa_reading[i] = WHITE_PATCH;
        }
    }

    for (int i = 0; i < no_of_sensors; i++)
    {
        printf("%d ", lsa_reading[i]);
    }
    printf("\n") ;
    
    return 0;
}