/**
 * @file rotary_encoder.h
 * @brief Interface definitions for the ESP32-compatible Incremental Rotary Encoder component.
 */

#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include <stdbool.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_err.h"
#include "driver/gpio.h"

void get_degree();


#ifdef __cplusplus
extern "C" {
#endif

typedef int32_t rotary_encoder_position_t;

/**
 * @brief Enum representing the direction of rotation.
 */
typedef enum
{
    ROTARY_ENCODER_DIRECTION_NOT_SET = 0,        ///< Direction not yet known (stationary since reset)
    ROTARY_ENCODER_DIRECTION_CLOCKWISE,
    ROTARY_ENCODER_DIRECTION_COUNTER_CLOCKWISE,
} rotary_encoder_direction_t;

// Used internally
///@cond INTERNAL
#define TABLE_COLS 4
typedef uint8_t table_row_t[TABLE_COLS];
///@endcond

/**
 * @brief Struct represents the current state of the device in terms of incremental position and direction of last movement
 */
typedef struct
{
    rotary_encoder_position_t position;    ///< Numerical position since reset. 
    rotary_encoder_direction_t direction;  ///< Direction of last movement. Set to NOT_SET on reset.
} rotary_encoder_state_t;

/**
 * @brief Struct carries all the information needed by this driver to manage the rotary encoder device.
 *        The fields of this structure should not be accessed directly.
 */
typedef struct
{
    gpio_num_t pin_a;                       ///< GPIO for Signal A from the rotary encoder device
    gpio_num_t pin_b;                       ///< GPIO for Signal B from the rotary encoder device
    QueueHandle_t queue;                    ///< Handle for event queue, created by ::rotary_encoder_create_queue
    const table_row_t * table;              ///< Pointer to active state transition table
    uint8_t table_state;                    ///< Internal state
    volatile rotary_encoder_state_t state;  ///< Device state
} rotary_encoder_info_t;

/**
 * @brief Struct represents a queued event, used to communicate current position to a waiting task
 */
typedef struct
{
    rotary_encoder_state_t state;  ///< The device state corresponding to this event
} rotary_encoder_event_t;

/**
 * @brief Initialise the rotary encoder device with the specified GPIO pins and full step increments.
 *        This function will set up the GPIOs as needed,
 *        Note: this function assumes that gpio_install_isr_service(0) has already been called.
 * @param[in, out] info Pointer to allocated rotary encoder info structure.
 * @param[in] pin_a GPIO number for rotary encoder output A.
 * @param[in] pin_b GPIO number for rotary encoder output B.
 * @return ESP_OK if successful, ESP_FAIL or ESP_ERR_* if an error occurred.
 */
esp_err_t rotary_encoder_init(rotary_encoder_info_t * info, gpio_num_t pin_a, gpio_num_t pin_b);

/**
 * @brief Enable half-stepping mode. This generates twice as many counted steps per rotation.
 * @param[in] info Pointer to initialised rotary encoder info structure.
 * @param[in] enable If true, count half steps. If false, only count full steps.
 * @return ESP_OK if successful, ESP_FAIL or ESP_ERR_* if an error occurred.
 */
esp_err_t rotary_encoder_enable_half_steps(rotary_encoder_info_t * info, bool enable);

/**
 * @brief Reverse (flip) the sense of the direction.
 *        Use this if clockwise/counterclockwise are not what you expect.
 * @param[in] info Pointer to initialised rotary encoder info structure.
 * @return ESP_OK if successful, ESP_FAIL or ESP_ERR_* if an error occurred.
 */
esp_err_t rotary_encoder_flip_direction(rotary_encoder_info_t * info);

/**
 * @brief Remove the interrupt handlers installed by ::rotary_encoder_init.
 *        Note: GPIOs will be left in the state they were configured by ::rotary_encoder_init.
 * @param[in] info Pointer to initialised rotary encoder info structure.
 * @return ESP_OK if successful, ESP_FAIL or ESP_ERR_* if an error occurred.
 */
esp_err_t rotary_encoder_uninit(rotary_encoder_info_t * info);

/**
 * @brief Create a queue handle suitable for use as an event queue.
 * @return A handle to a new queue suitable for use as an event queue.
 */
QueueHandle_t rotary_encoder_create_queue(void);

/**
 * @brief Set the driver to use the specified queue as an event queue.
 *        It is recommended that a queue constructed by ::rotary_encoder_create_queue is used.
 * @param[in] info Pointer to initialised rotary encoder info structure.
 * @param[in] queue Handle to queue suitable for use as an event queue. See ::rotary_encoder_create_queue.
 * @return ESP_OK if successful, ESP_FAIL or ESP_ERR_* if an error occurred.
 */
esp_err_t rotary_encoder_set_queue(rotary_encoder_info_t * info, QueueHandle_t queue);

/**
 * @brief Get the current position of the rotary encoder.
 * @param[in] info Pointer to initialised rotary encoder info structure.
 * @param[in, out] state Pointer to an allocated rotary_encoder_state_t struct that will
 * @return ESP_OK if successful, ESP_FAIL or ESP_ERR_* if an error occurred.
 */
esp_err_t rotary_encoder_get_state(const rotary_encoder_info_t * info, rotary_encoder_state_t * state);

/**
 * @brief Reset the current position of the rotary encoder to zero.
 * @param[in] info Pointer to initialised rotary encoder info structure.
 * @return ESP_OK if successful, ESP_FAIL or ESP_ERR_* if an error occurred.
 */
esp_err_t rotary_encoder_reset(rotary_encoder_info_t * info);


#ifdef __cplusplus
}
#endif

#endif  // ROTARY_ENCODER_H
