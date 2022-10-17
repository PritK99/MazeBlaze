#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_log.h"

#include "rotary_encoder.h"

#define TAG "app"

/*right-most motor*/
#define ROT_ENC_A_GPIO 4
#define ROT_ENC_B_GPIO 16

/*left-most motor*/
#define ROT_ENC_C_GPIO 5
#define ROT_ENC_D_GPIO 17

#define ENABLE_HALF_STEPS false // Set to true to enable tracking of rotary encoder at half step resolution
#define RESET_AT 0              // Set to a positive non-zero number to reset the position if this value is exceeded
#define FLIP_DIRECTION false    // Set to true to reverse the clockwise/counterclockwise sense

void app_main()
{
    // esp32-rotary-encoder requires that the GPIO ISR service is installed before calling rotary_encoder_register()
    ESP_ERROR_CHECK(gpio_install_isr_service(0));

    // Initialise the rotary encoder device with the GPIOs for A and B signals
    rotary_encoder_info_t info1 = {0};
    rotary_encoder_info_t info2 = {0};

    ESP_ERROR_CHECK(rotary_encoder_init(&info1, ROT_ENC_A_GPIO, ROT_ENC_B_GPIO));
    ESP_ERROR_CHECK(rotary_encoder_enable_half_steps(&info1, ENABLE_HALF_STEPS));

    ESP_ERROR_CHECK(rotary_encoder_init(&info2, ROT_ENC_C_GPIO, ROT_ENC_D_GPIO));
    ESP_ERROR_CHECK(rotary_encoder_enable_half_steps(&info2, ENABLE_HALF_STEPS));

#ifdef FLIP_DIRECTION
    ESP_ERROR_CHECK(rotary_encoder_flip_direction(&info1));
    ESP_ERROR_CHECK(rotary_encoder_flip_direction(&info2));
#endif

    // Create a queue for events from the rotary encoder driver.
    // Tasks can read from this queue to receive up to date position information.
    QueueHandle_t event_queue1 = rotary_encoder_create_queue();
    ESP_ERROR_CHECK(rotary_encoder_set_queue(&info1, event_queue1));

    QueueHandle_t event_queue2 = rotary_encoder_create_queue();
    ESP_ERROR_CHECK(rotary_encoder_set_queue(&info2, event_queue2));

    while (1)
    {
        // Wait for incoming events on the event queue.
        rotary_encoder_event_t event1 = {0};
        rotary_encoder_event_t event2 = {0};

        // This portion will wait for 1000 ms for the task to come and if it does not , will go to else portion
        if (xQueueReceive(event_queue1, &event1, 1000 / portTICK_PERIOD_MS) && xQueueReceive(event_queue2, &event2, 1000 / portTICK_PERIOD_MS))
        {
            ESP_LOGI(TAG, "Event: position %d, direction %s", event1.state.position,
                     event1.state.direction ? (event1.state.direction == ROTARY_ENCODER_DIRECTION_CLOCKWISE ? "CW" : "CCW") : "NOT_SET");
            ESP_LOGI(TAG, "Event: position %d, direction %s", event2.state.position,
                     event2.state.direction ? (event2.state.direction == ROTARY_ENCODER_DIRECTION_CLOCKWISE ? "CW" : "CCW") : "NOT_SET");
            printf("\n");
        }
        else
        {
            // simply print current position and direction
            rotary_encoder_state_t state = {0};
            ESP_ERROR_CHECK(rotary_encoder_get_state(&info1, &state));
            ESP_LOGI(TAG, "Paused: position %d, direction %s", event1.state.position,
                     event1.state.direction ? (event1.state.direction == ROTARY_ENCODER_DIRECTION_CLOCKWISE ? "CW" : "CCW") : "NOT_SET");
            ESP_ERROR_CHECK(rotary_encoder_get_state(&info2, &state));
            ESP_LOGI(TAG, "Paused: position %d, direction %s", event2.state.position,
                     event2.state.direction ? (event2.state.direction == ROTARY_ENCODER_DIRECTION_CLOCKWISE ? "CW" : "CCW") : "NOT_SET");
            printf("\n");
            // Reset the device
            if (RESET_AT && (state.position >= RESET_AT || state.position <= -RESET_AT))
            {
                ESP_LOGI(TAG, "Reset");
                ESP_ERROR_CHECK(rotary_encoder_reset(&info1));
                ESP_ERROR_CHECK(rotary_encoder_reset(&info2));
            }
        }
    }
    ESP_LOGE(TAG, "queue receive failed");

    ESP_ERROR_CHECK(rotary_encoder_uninit(&info1));
    ESP_ERROR_CHECK(rotary_encoder_uninit(&info2));
}
