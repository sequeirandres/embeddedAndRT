//
//   Example de uso Queue - 
//
//   sending Data 
//		xQueueSend(Queue Name, &Variable to Send, Delay)
//	
//	Receiving Data
//		xQueueReceive(Queue Name, &Variable to receive)
//
//

#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// DEFINE GPIO PIN FOR LEDS
#define GPIO_PIN_RED     13
#define GPIO_PIN_BLUE    14
#define GPIO_PIN_GREEN	 15	
// DEFINE TIME DELAY IN MS
#define HAL_DELAY_LED_RED	500
#define HAL_DELAY_LED_BLUE 	300
#define HAL_DELAY_LED_GREEN	100

#define LED_STATUS_ON   1
#define LED_STATUS_OFF  0

// Number of word to allocate for use as the task stack
#define UX_STACK_SZIE 	256

#define IDLE_PRIORITY_TASK_RED		1
#define IDLE_PRIORITY_TASK_BLUE		1
#define IDLE_PRIORITY_TASK_GREEN	1
#define IDLE_PRIORITY_TASK_QUEUE	1

#define TASK_NAME_RED	"Task led Red"
#define TASK_NAME_BLUE	"Task led Blue"
#define TASK_NAME_GREEN "Task led Green"
#define TASK_NAME_QUEUE "Task Queue"


static QueueHandle_t xQueue;


void vTask_led_red(void *pvParameters)
{       
    const uint LED_RED = GPIO_PIN_RED;
    
    uint uIValueToSend = 0 ; 	// variable to send Queue
    
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED,GPIO_OUT);
    
    for(;;)
    {
        gpio_put(LED_RED, LED_STATUS_OFF);
        uIValueToSend = 0 ;
        xQueueSend(xQueue, &uIValueToSend, 0U) ;
 		vTaskDelay(HAL_DELAY_LED_RED);
        
        gpio_put(LED_RED, LED_STATUS_ON);
        uIValueToSend = 1;
        xQueueSend(xQueue, &uIValueToSend, 0U) ;
        vTaskDelay(HAL_DELAY_LED_RED);
        
    }
}


void vTask_queue(void *pvParameters)
{
	uint uIReceivedValue;		// variable to send Queue ;
	
	const uint LED_GREEN = GPIO_PIN_GREEN;
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN,GPIO_OUT);
	
	
	while(true)		// Loop 
	{
		xQueueReceive(xQueue,&uIReceivedValue,portMAX_DELAY);
		if(uIReceivedValue!=1)
		{
			gpio_put(LED_GREEN, LED_STATUS_OFF);
			printf("Led OFF \n") ;
		}
		else
		{
			gpio_put(LED_GREEN, LED_STATUS_ON);
			printf("Led ON \n") ;
		}
 	
	}

}



int main(void)
{

    stdio_init_all();
	
	xQueue = xQueueCreate(1, sizeof(uint)) ;	
	
    xTaskCreate(vTask_led_red,TASK_NAME_RED, UX_STACK_SZIE, NULL, IDLE_PRIORITY_TASK_RED, NULL);
    xTaskCreate(vTask_queue,TASK_NAME_QUEUE, UX_STACK_SZIE, NULL, IDLE_PRIORITY_TASK_QUEUE, NULL);

    vTaskStartScheduler();

    while(true){};
	
	return 0;
}
