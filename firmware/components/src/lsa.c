#include "mazeblaze2.h"
static const char *TAG_MOTOR_DRIVER = "motor_driver";
#define WHITE_PATCH 1 // LSA values that > 1000 are white patch
#define BLACK_PATCH 0    // LSA values that lie between 0 - 1000 are black patch
#define no_of_sensors 6 // total number of sensors in bot

int lsa_reading[no_of_sensors];

// esp_err_t is an unsigned int given by esp for error handling . 0 denotes OK_ESP i.e. success
esp_err_t enable_lsa()
{

    esp_err_t err;
    CHECK_LOGE(err, mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM2A, MOSFET_PWM), TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
    
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 20000;
    pwm_config.cmpr_a = 0;
    pwm_config.cmpr_b = 0;
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    esp_err_t err_0 = mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_2, &pwm_config);

    esp_err_t flag_0, flag_2, flag_3, flag_4, flag_5, flag_6, flag_7;

    flag_0 = adc1_config_width(ADC_WIDTH_BIT_12);

    flag_2 = adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
    flag_3 = adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11);
    flag_4 = adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
    flag_5 = adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);
    flag_6 = adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);
    flag_7 = adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_11);
    return err_0|| flag_0 || flag_2 || flag_3 || flag_4 || flag_5 || flag_6 || flag_7 ; // we use or || because success of flag is 0 here
}

void set_brightness(int val) {
    mcpwm_set_duty(MCPWM_UNIT_1, MCPWM_TIMER_2, MCPWM_OPR_A, val);
    mcpwm_set_duty_type(MCPWM_UNIT_1, MCPWM_TIMER_2, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
}

int get_raw_lsa() // gets raw readings in range of 400 - 2000 appx
{
    while (1)
    {

    lsa_reading[0] = adc1_get_raw(ADC1_CHANNEL_0);
    lsa_reading[1] = adc1_get_raw(ADC1_CHANNEL_3);
    lsa_reading[2] = adc1_get_raw(ADC1_CHANNEL_6);
    lsa_reading[3] = adc1_get_raw(ADC1_CHANNEL_7);
    lsa_reading[4] = adc1_get_raw(ADC1_CHANNEL_4);
    lsa_reading[5] = adc1_get_raw(ADC1_CHANNEL_5);

    for (int i = 0; i < no_of_sensors; i++)
    {
        if (lsa_reading[i] > 1000)
        {
            lsa_reading[i] = WHITE_PATCH;
        }
        else
        {
            lsa_reading[i] = BLACK_PATCH;
        }
    }

        for (int i = 0; i < no_of_sensors; i++)
        {
            printf("%d ", lsa_reading[i]);
        }
        printf("\n") ;
    }
}