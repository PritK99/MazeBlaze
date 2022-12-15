#ifndef PIN_DEFS_H
#define PIN_DEFS_H

#define CHECK(x)                \
    do                          \
    {                           \
        esp_err_t __;           \
        if ((__ = x) != ESP_OK) \
            return __;          \
    } while (0)
#define CHECK_LOGE(err, x, tag, msg, ...)      \
    do                                         \
    {                                          \
        if ((err = x) != ESP_OK)               \
        {                                      \
            ESP_LOGE(tag, msg, ##__VA_ARGS__); \
            return err;                        \
        }                                      \
    } while (0)



#define END_LED 10

#define DEBUG_SWITCH 13



/////// motor driver ////////
//****** normal mode ******//
#define PWM_A 12 //MCPWM_UNIT_1 MCPWM0A
#define MDA_NORMAL_IN_3 25 //MCPWM_UNIT_1 MCPWM0B
#define MDA_NORMAL_IN_4 26 //MCPWM_UNIT_1 MCPWM1A
#define PWM_B 33 //MCPWM_UNIT_1 MCPWM1B
#define MDA_NORMAL_IN_5 27 //MCPWM_UNIT_1 MCPWM2A bin1
#define MDA_NORMAL_IN_6 14 //MCPWM_UNIT_1 MCPWM2B bin2



#endif
