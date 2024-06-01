#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "hardware/uart.h"
#include "hardware/irq.h"


#define UART_ID     uart0
#define BAUD_RATE   115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

#if 1
// A synchronous driver
SemaphoreHandle_t xUARTMutex;
SemaphoreHandle_t xUARTSemaphore;
const char *messages_ptr = NULL;
const char *messages[] = { "Hello IUA FREERTOS from Task 1\r\n", "Hello IUA FREERTOS from Task 2\r\n" };
char message_index = 0;
int UART_IRQ;

static inline void on_uart(bool is_irq) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    while (uart_is_writable(uart0) && messages_ptr[message_index] != '\0') {
        uart_putc_raw(UART_ID, messages_ptr[message_index]);
        message_index++;
	}
    if (is_irq && messages_ptr[message_index] == '\0') {
        uart_set_irq_enables(UART_ID, false, false);
        xSemaphoreGiveFromISR(xUARTSemaphore, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken);
    }
}

static void on_uart_irq() {
    on_uart(true);
}

void vTaskSynchronous( void * pvParameters ) {
    int taskId = *((int *)pvParameters);

    for( ;; ) {
        printf("1. TaskId %d Wants to send message through UART, will capture UART\n", taskId);
        if  (xSemaphoreTake(xUARTMutex, portMAX_DELAY)) {
            printf("2. TaskId %d Captured UART\n", taskId);
            message_index = 0;
            messages_ptr = taskId == 1 ? messages[0] : messages[1]; 
            
            uart_set_irq_enables(UART_ID, false, true);
            on_uart(false);
            if (xSemaphoreTake(xUARTSemaphore, portMAX_DELAY)) {
                printf("4. TaskId %d completed TX, release UART and will sleep\n", taskId);
                xSemaphoreGive(xUARTMutex);
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main() {
    stdio_init_all();
    for (int cnt = 1; cnt < 11; cnt++) {
        printf("\r%d", cnt);
        sleep_ms(1000);
    }
    printf("\r%d");

    xUARTMutex = xSemaphoreCreateMutex();
    printf("USART Mutex creation was %s\n", xUARTMutex == NULL ? "failed": "succesful");
    vSemaphoreCreateBinary(xUARTSemaphore);
    printf("USART Semaphore creation was %s\n", xUARTSemaphore == NULL ? "failed": "succesful");

    // UART Config
    UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;
    irq_set_enabled(UART_IRQ, false);
    uart_set_irq_enables(UART_ID, false, false);
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    irq_set_exclusive_handler(UART_IRQ, on_uart_irq);
    irq_set_enabled(UART_IRQ, true);


    int tid1 = 1, tid2 = 2;
    TaskHandle_t xHandle1 = NULL, xHandle2 = NULL;;
    BaseType_t xReturned1 = xTaskCreate(vTaskSynchronous, "Synchronous Task 1", 512, (void *) &tid1, 2, &xHandle1);
    BaseType_t xReturned2 = xTaskCreate(vTaskSynchronous, "Synchronous Task 2", 512, (void *) &tid2, 2, &xHandle2);

    vTaskStartScheduler();
    __builtin_unreachable();
}
#endif