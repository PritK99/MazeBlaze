#include "mazeblaze2.h"

// esp_err_t is an unsigned int given by esp for error handling . 0 denotes OK_ESP i.e. success
esp_err_t enable_lsa()
{
    adc1_config_width(ADC_WIDTH_BIT_12);

    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);

    return ESP_OK;
}

int get_raw_lsa() // gets raw readings in range of 400 - 2000 appx
{
    lsa_reading[0] = adc1_get_raw(ADC1_CHANNEL_0);
    lsa_reading[1] = adc1_get_raw(ADC1_CHANNEL_3);
    lsa_reading[2] = adc1_get_raw(ADC1_CHANNEL_6);
    lsa_reading[3] = adc1_get_raw(ADC1_CHANNEL_7);
    lsa_reading[4] = adc1_get_raw(ADC1_CHANNEL_4);

    // for (int i = 0; i < no_of_sensors; i++)
    // {
    
    //     if (lsa_reading[i] > 3600)
    //     {
    //         lsa_reading[i] = BLACK_PATCH;        
    //     }
    //     else
    //     {
    //         lsa_reading[i] = WHITE_PATCH;
    //     }
        
    // }  
        
    

    for (int i = 0; i < no_of_sensors; i++)
    {
        printf("%d ", lsa_reading[i]);
    }
    printf("\n");

    return 0;
}