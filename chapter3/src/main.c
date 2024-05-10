#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

void vTaskCode( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below. 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    */
    for( ;; )
    {
            gpio_put(PICO_DEFAULT_LED_PIN, 1);
            vTaskDelay(1000);
            gpio_put(PICO_DEFAULT_LED_PIN, 0);
            vTaskDelay(1000);
    }
}

int main() {
    stdio_init_all();
    
    TaskHandle_t xHandle = NULL;
    /* Create the task, storing the handle. */
    BaseType_t xReturned = xTaskCreate(
                    vTaskCode,       /* Function that implements the task. */
                    "Blinky task",   /* Text name for the task. */
                    512,             /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    &xHandle );   

    vTaskStartScheduler();
    __builtin_unreachable();
}