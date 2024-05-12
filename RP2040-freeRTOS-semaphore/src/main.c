#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// DEFINE GPIO PIN FOR LEDS
#define GPIO_PIN_RED     13
#define GPIO_PIN_BLUE    14
#define GPIO_PIN_GREEN	 15	
// DEFINE TIME DELAY IN MS
#define HAL_DELAY_LED_RED	100
#define HAL_DELAY_LED_BLUE 	100
#define HAL_DELAY_LED_GREEN	100

#define MAX_COUNTER	5 

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


#define HAL_WAIT_LED_RED	100
#define HAL_WAIT_LED_BLUE 	100
#define HAL_WAIT_LED_GREEN	100

// variable global

SemaphoreHandle_t xSemaphore;


void vTask_led_red(void *pvParameters)
{       
    const uint LED_RED = GPIO_PIN_RED;
    int counter =0 ;
    
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED,GPIO_OUT);
    
    for(;;)
    {
    	if(xSemaphoreTake(xSemaphore, pdMS_TO_TICKS(HAL_WAIT_LED_RED))) 
    	{
		    for(counter=0;counter<MAX_COUNTER;counter++)  // Blink LED_RED MAX_COUNTER 
			{
			    gpio_put(LED_RED, LED_STATUS_ON);
			    vTaskDelay(HAL_DELAY_LED_RED);
			    gpio_put(LED_RED, LED_STATUS_OFF);
			    vTaskDelay(HAL_DELAY_LED_RED);
			}
			xSemaphoreGive(xSemaphore) ;    	
    	}
    	else
    	{

    	}    
		vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void vTask_led_blue(void *pvParameters)
{   
    const uint LED_BLUE = GPIO_PIN_BLUE;
    int counter=0 ;
    
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE,GPIO_OUT);

    for(;;)
    {   
        if(xSemaphoreTake(xSemaphore, pdMS_TO_TICKS(HAL_WAIT_LED_BLUE)))
    	{
    	    for(counter=0;counter<MAX_COUNTER;counter++)  // Blink LED_RED MAX_COUNTER 
			{
        		gpio_put(LED_BLUE, LED_STATUS_ON);
        		vTaskDelay(HAL_DELAY_LED_BLUE);
       			gpio_put(LED_BLUE, LED_STATUS_OFF);
        		vTaskDelay(HAL_DELAY_LED_BLUE);
    		}
    		xSemaphoreGive(xSemaphore) ;
    	
    	}
    	else
    	{

    	}
    	vTaskDelay(pdMS_TO_TICKS(100));
    }
}   


void vTask_led_green(void *pvParameters)
{   
    const uint LED_GREEN = GPIO_PIN_GREEN;
    int counter=0;
    
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN,GPIO_OUT);

    for(;;)
    {
        if(xSemaphoreTake(xSemaphore, pdMS_TO_TICKS(HAL_WAIT_LED_GREEN)))
    	{
    	    for(counter=0;counter<MAX_COUNTER;counter++)  // Blink LED_RED MAX_COUNTER 
			{
        		gpio_put(LED_GREEN, LED_STATUS_ON);
        		vTaskDelay(HAL_DELAY_LED_GREEN);
        		gpio_put(LED_GREEN, LED_STATUS_OFF);
        		vTaskDelay(HAL_DELAY_LED_GREEN);
    		}
    		xSemaphoreGive(xSemaphore) ;
    	}
    	else
    	{

    	}
    	vTaskDelay(pdMS_TO_TICKS(100));
    }
}   


int main(void)
{

    stdio_init_all();
    
    TaskHandle_t xHandle1 = NULL;
    TaskHandle_t xHandle2 = NULL;
    TaskHandle_t xHandle3 = NULL;
    
    vSemaphoreCreateBinary(xSemaphore) ;

    xTaskCreate(vTask_led_red,TASK_NAME_RED, UX_STACK_SZIE, NULL, IDLE_PRIORITY_TASK_RED, &xHandle1);
    xTaskCreate(vTask_led_blue,TASK_NAME_BLUE, UX_STACK_SZIE, NULL, IDLE_PRIORITY_TASK_BLUE, &xHandle2);
    xTaskCreate(vTask_led_green,TASK_NAME_GREEN, UX_STACK_SZIE, NULL, IDLE_PRIORITY_TASK_GREEN, &xHandle3);

    vTaskStartScheduler();

    while(true){};
}
