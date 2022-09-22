#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "esp_attr.h"
#include "esp_log.h"
#include "esp_err.h"
#include "pin_defs.h"

#define MOTOR_FORWARD 1
#define MOTOR_BACKWARD -1
#define MOTOR_STOP 0

#define MOTOR_A_0 0
#define MOTOR_A_1 1

esp_err_t enable_motor_driver();
esp_err_t set_motor_speed(int motor_id, int direction, float duty_cycle);

#endif