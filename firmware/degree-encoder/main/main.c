#include "mazeblaze2.h"
#include "esp_system.h"
#include "esp_log.h"

#define TAG "app"
#define ROT_ENC_A_GPIO 17
#define ROT_ENC_B_GPIO 5
float degree;

void get_degree()
{
     // esp32-rotary-encoder requires that the GPIO ISR service is installed before calling rotary_encoder_register()
    ESP_ERROR_CHECK(gpio_install_isr_service(0));

    // Initialise the rotary encoder device with the GPIOs for A and B signals
    rotary_encoder_info_t info = { 0 };
    ESP_ERROR_CHECK(rotary_encoder_init(&info, ROT_ENC_A_GPIO, ROT_ENC_B_GPIO));
  

    // Create a queue for events from the rotary encoder driver.
    // Tasks can read from this queue to receive up to date position information.
    QueueHandle_t event_queue = rotary_encoder_create_queue();
    ESP_ERROR_CHECK(rotary_encoder_set_queue(&info, event_queue));

    while (1)
    {
        // Wait for incoming events on the event queue.
        rotary_encoder_event_t event = { 0 };
        if (xQueueReceive(event_queue, &event, 1000 / portTICK_PERIOD_MS) == pdTRUE)
        {
        
            degree = (((event.state.position)*0.055*57.3)/5.5);
            ESP_LOGI(TAG, "Degree: %lf", degree);

        }
        
    }
    ESP_LOGE(TAG, "queue receive failed");

    ESP_ERROR_CHECK(rotary_encoder_uninit(&info));
   

}

void app_main()
{

    ESP_ERROR_CHECK(enable_motor_driver()) ;

    set_motor_speed(MOTOR_A_0, MOTOR_FORWARD, 75);
    set_motor_speed(MOTOR_A_1, MOTOR_BACKWARD, 75);

    vTaskDelay(1000/portTICK_PERIOD_MS);

    set_motor_speed(MOTOR_A_0, MOTOR_STOP, 75);
    set_motor_speed(MOTOR_A_1, MOTOR_STOP, 75);    

    get_degree();
}