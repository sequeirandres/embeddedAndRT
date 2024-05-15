#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "stdlib.h"

void trigger_hard_fault() {
    // Access an invalid memory address
    volatile uint32_t *invalid_ptr = (volatile uint32_t *)0xFFFFFFFF;
    *invalid_ptr = 0xDEADBEEF;
}

// A Crash app
void vTaskCrashMemory( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below. 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    */
    int tid = *((int *)pvParameters);
    int count = 5;
    for( ;; ) {
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
        printf("TaskId %d generating a hardfault in %d\n", tid, count);
        if (count == 0) {
            count = 5;
            trigger_hard_fault();
        }
        count--;
    }
}

int main() {
    stdio_init_all();

#ifdef PICO_DEFAULT_LED_PIN
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
#endif

    TaskHandle_t xHandle = NULL;
    int tid1 = 1;

    /* Create the task, storing the handle. */
    BaseType_t xReturned = xTaskCreate(vTaskCrashMemory, "Crashing task", 512, (void *) &tid1, tskIDLE_PRIORITY, &xHandle );   

    vTaskStartScheduler();
    __builtin_unreachable();
}