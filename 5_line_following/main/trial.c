#include <stdio.h>
#include "mazeblaze2.h"

int main ()
{

}

void led_blink_at_end()
{
    set_motor_speed(MOTOR_A_0 , MOTOR_STOP , 60) ;
    set_motor_speed(MOTOR_A_0 , MOTOR_STOP , 60) ;

    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << END_LED);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    while(true)
    {
    gpio_set_level(END_LED, 1);
    pio_set_level(END_LED, 0);
    }
}