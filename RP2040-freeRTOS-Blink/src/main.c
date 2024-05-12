#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

// DEFINE GPIO PIN FOR LEDS
#define GPIO_PIN_RED     13
#define GPIO_PIN_BLUE    14
#define GPIO_PIN_GREEN	 15	
// DEFINE TIME DELAY IN MS
#define HAL_DELAY_LED_RED	1000
#define HAL_DELAY_LED_BLUE 	300
#define HAL_DELAY_LED_GREEN	100

#define LED_STATUS_ON   1
#define LED_STATUS_OFF  0

// Number of word to allocate for use as the task stack
#define UX_STACK_SZIE 	512
#define IDLE_PRIORITY_TASK_RED		1
#define IDLE_PRIORITY_TASK_BLUE		1
#define IDLE_PRIORITY_TASK_GREEN	1

#define TASK_NAME_RED	"Task led Red"
#define TASK_NAME_BLUE	"Task led Blue"
#define TASK_NAME_GREEN "Task led Green"

void vTask_led_red(void *pvParameters)
{       
    const uint LED_RED = GPIO_PIN_RED;
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED,GPIO_OUT);
    
    for(;;)
    {
        gpio_put(LED_RED, LED_STATUS_OFF);
        vTaskDelay(HAL_DELAY_LED_RED);
        gpio_put(LED_RED, LED_STATUS_ON);
        vTaskDelay(HAL_DELAY_LED_RED);
    }
}

void vTask_led_blue(void *pvParameters)
{   
    const uint LED_BLUE = GPIO_PIN_BLUE;
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE,GPIO_OUT);

    for(;;)
    {   
        gpio_put(LED_BLUE, LED_STATUS_OFF);
        vTaskDelay(HAL_DELAY_LED_BLUE);
        gpio_put(LED_BLUE, LED_STATUS_ON);
        vTaskDelay(HAL_DELAY_LED_BLUE);
    }
}   


void vTask_led_green(void *pvParameters)
{   
    const uint LED_GREEN = GPIO_PIN_GREEN;
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN,GPIO_OUT);

    for(;;)
    {   
        gpio_put(LED_GREEN, LED_STATUS_OFF);
        vTaskDelay(HAL_DELAY_LED_GREEN);
        gpio_put(LED_GREEN, LED_STATUS_ON);
        vTaskDelay(HAL_DELAY_LED_GREEN);
    }
}   



int main(void)
{

    stdio_init_all();

    xTaskCreate(vTask_led_red,TASK_NAME_RED, UX_STACK_SZIE, NULL, IDLE_PRIORITY_TASK_RED, NULL);
    xTaskCreate(vTask_led_blue,TASK_NAME_BLUE, UX_STACK_SZIE, NULL, IDLE_PRIORITY_TASK_BLUE, NULL);
    xTaskCreate(vTask_led_green,TASK_NAME_GREEN, UX_STACK_SZIE, NULL, IDLE_PRIORITY_TASK_GREEN, NULL);

    vTaskStartScheduler();

    while(true){};
}
