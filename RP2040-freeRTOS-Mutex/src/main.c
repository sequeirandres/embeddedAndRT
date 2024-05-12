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
#define HAL_DELAY_LED_BLUE 	500
#define HAL_DELAY_LED_GREEN	100

#define MAX_COUNTER	5 

#define LED_STATUS_ON   1
#define LED_STATUS_OFF  0

// Number of word to allocate for use as the task stack
#define UX_STACK_SIZE 	512

#define IDLE_PRIORITY_TASK_1		1
#define IDLE_PRIORITY_TASK_2		1

#define TASK_NAME_1	"Task led 1"
#define TASK_NAME_2	"Task led 2"


#define HAL_WAIT_TASK_1		100
#define HAL_WAIT_TASK_2 	100

// variable global Mutex 
SemaphoreHandle_t xMutex;

void vTask_blink1(void *pvParameters)
{       
    int counter =0 ;
   
    for(;;)
    {
    	if(xSemaphoreTake(xMutex, pdMS_TO_TICKS(HAL_WAIT_TASK_1))==pdTRUE ) 
    	{
    		gpio_put(GPIO_PIN_GREEN, LED_STATUS_ON);
		    for(counter=0;counter<MAX_COUNTER+10;counter++)  // Blink LED_RED MAX_COUNTER 
			{
			    gpio_put(GPIO_PIN_RED , LED_STATUS_ON);
			    vTaskDelay(HAL_DELAY_LED_RED);
			    gpio_put(GPIO_PIN_RED , LED_STATUS_OFF);
			    vTaskDelay(HAL_DELAY_LED_RED);
			}
			xSemaphoreGive(xMutex);
			gpio_put(GPIO_PIN_GREEN, LED_STATUS_OFF);    	
    	}
    	else
    	{

    	}    
		vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void vTask_blink2(void *pvParameters)
{   

    int counter=0 ;

    for(;;)
    {   
        if(xSemaphoreTake(xMutex, pdMS_TO_TICKS(HAL_WAIT_TASK_2))==pdTRUE)
    	{
    		gpio_put(GPIO_PIN_BLUE, LED_STATUS_ON);
    	    for(counter=0;counter<MAX_COUNTER;counter++)  // Blink LED_RED MAX_COUNTER 
			{
        		gpio_put(GPIO_PIN_RED, LED_STATUS_ON);
        		vTaskDelay(HAL_DELAY_LED_BLUE);
       			gpio_put(GPIO_PIN_RED, LED_STATUS_OFF);
        		vTaskDelay(HAL_DELAY_LED_BLUE);
    		}
    		xSemaphoreGive(xMutex) ;
    		gpio_put(GPIO_PIN_BLUE, LED_STATUS_OFF);
    	
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
    
    const uint LED_RED = GPIO_PIN_RED;
    const uint LED_BLUE = GPIO_PIN_BLUE;
    const uint LED_GREEN = GPIO_PIN_GREEN;
     
    gpio_init(LED_RED);
    gpio_init(LED_BLUE);
    gpio_init(LED_GREEN);
    
    gpio_set_dir(LED_RED,GPIO_OUT);
    gpio_set_dir(LED_BLUE,GPIO_OUT);
    gpio_set_dir(LED_GREEN,GPIO_OUT);
    
    xMutex =  xSemaphoreCreateMutex() ;
	
	if(xMutex != NULL)
	{
	    xTaskCreate(vTask_blink1,TASK_NAME_1, UX_STACK_SIZE, NULL, IDLE_PRIORITY_TASK_1, &xHandle1);
    	xTaskCreate(vTask_blink2,TASK_NAME_2, UX_STACK_SIZE, NULL, IDLE_PRIORITY_TASK_2, &xHandle2);
    	vTaskStartScheduler();
	}
	else
	{

	}


    while(true)
    {
    };

	return 0;
}
