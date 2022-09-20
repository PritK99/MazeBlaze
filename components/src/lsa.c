#include "lsa.h"

#define WHITE_PATCH 1000 // LSA values that > 1000 are white patch
#define BLACK_PATCH 0    // LSA values that lie between 0 - 1000 are black patch
#define no_of_sensors 7  // total number of sensors in bot

int lsa_readings[no_of_sensors];

// esp_err_t is an unsigned int given by esp for error handling . 0 denotes OK_ESP i.e. success
esp_err_t enable_lsa()
{
    esp_err_t flag_0, flag_1, flag_2, flag_3, flag_4, flag_5, flag_6, flag_7, flag_8;

    flag_0 = adc1_config_width(ADC_WIDTH_BIT_12);
    flag_1 = adc2_config_width(ADC_WIDTH_BIT_12);

    flag_2 = adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
    flag_3 = adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11);
    flag_4 = adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
    flag_5 = adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);
    flag_6 = adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);
    flag_7 = adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_11);
    flag_8 = adc2_config_channel_atten(ADC2_CHANNEL_12, ADC_ATTEN_DB_11);
    return flag_0 || flag_1 || flag_2 || flag_3 || flag_4 || flag_5 || flag_6 || flag_7 || flag_8; // we use or || because success of flag is 0 here
}

int get_raw_lsa() // gets raw readings in range of 400 - 2000 appx
{

    lsa_reading[0] = adc1_get_raw(ADC1_CHANNEL_0);
    lsa_reading[1] = adc1_get_raw(ADC1_CHANNEL_3);
    lsa_reading[2] = adc1_get_raw(ADC1_CHANNEL_6);
    lsa_reading[3] = adc1_get_raw(ADC1_CHANNEL_7);
    lsa_reading[4] = adc1_get_raw(ADC1_CHANNEL_4);
    lsa_reading[5] = adc1_get_raw(ADC1_CHANNEL_5);
    lsa_reading[6] = adc2_get_raw(ADC2_CHANNEL_12);

    for (int i = 0; i < no_of_sensors; i++)
    {
        if (lsa_readings[i] > 1000)
        {
            lsa_readings[i] = WHITE_PATCH;
        }
        else
        {
            lsa_readings[i] = BLACK_PATCH;
        }
    }

    while (1)
    {
        for (int i = 0; i < no_of_sensors; i++)
        {
            printf("%d ", lsa_readings[i]);
        }
        printf('/n') ;
    }