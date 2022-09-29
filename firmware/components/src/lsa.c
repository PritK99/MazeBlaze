#include "mazeblaze2.h"


static const char* TAG = "LSA_READING";
static esp_adc_cal_characteristics_t adc1_chars;

// esp_err_t is an unsigned int given by esp for error handling . 0 denotes OK_ESP i.e. success
esp_err_t enable_lsa()
{
    
    mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM2A, 9);
    
    mcpwm_config_t pwm_config;
<<<<<<< HEAD
    pwm_config.frequency = 20000;
    pwm_config.cmpr_a = 0; 
=======
    pwm_config.frequency = 2000;
    pwm_config.cmpr_a = 0;
>>>>>>> 2b18eb7e785bb96790e985d4c3692c017b5070ff
    pwm_config.cmpr_b = 0;
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    
    mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_2, &pwm_config);

<<<<<<< HEAD
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 0, &adc1_chars);

    flag_0 = adc1_config_width(ADC_WIDTH_BIT_12);

    flag_2 = adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
    flag_3 = adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11);
    flag_4 = adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
    flag_5 = adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);
    flag_6 = adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);
    flag_7 = adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_11);
    return err_0 || flag_0 || flag_2 || flag_3 || flag_4 || flag_5 || flag_6 || flag_7 ; // we use or || because success of flag is 0 here
=======
    adc1_config_width(ADC_WIDTH_BIT_12);

    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_11);
    
    return ESP_OK;
>>>>>>> 2b18eb7e785bb96790e985d4c3692c017b5070ff
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
<<<<<<< HEAD

=======
>>>>>>> 2b18eb7e785bb96790e985d4c3692c017b5070ff
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

<<<<<<< HEAD
        // for (int i = 0; i < no_of_sensors; i++)
        // {
            ESP_LOGI(TAG, "%d %d %d %d %d %d ", lsa_reading[0], lsa_reading[1], lsa_reading[2], lsa_reading[3], lsa_reading[4], lsa_reading[5]);
        // }
    return 0;
 }
=======
    for (int i = 0; i < no_of_sensors; i++)
    {
        printf("%d ", lsa_reading[i]);
    }
    printf("\n") ;
    
    return 0;
}
>>>>>>> 2b18eb7e785bb96790e985d4c3692c017b5070ff
