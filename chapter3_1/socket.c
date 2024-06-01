/*
 * A socket based audio player
 */
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include "pico/stdlib.h"

#include "pico/cyw43_arch.h"
#include "lwip/ip4_addr.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"

static struct tcp_pcb *server_pcb;

err_t on_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p != NULL) {
        tcp_recved(tpcb, p->tot_len);
        printf("Received: %.*s\n", p->len, (char *)p->payload);
        tcp_write(tpcb, p->payload, p->len, TCP_WRITE_FLAG_COPY);
        pbuf_free(p);
    } else if (err == ERR_OK) {
        return tcp_close(tpcb);
    }
    return ERR_OK;
}

err_t on_accept(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_recv(newpcb, on_recv);
    return ERR_OK;
}

void start_server() {
    server_pcb = tcp_new();
    if (server_pcb != NULL) {
        err_t err;
        err = tcp_bind(server_pcb, IP_ADDR_ANY, SERVER_PORT);
        if (err == ERR_OK) {
            server_pcb = tcp_listen(server_pcb);
            tcp_accept(server_pcb, on_accept);
            printf("Server listening on port %d\n", SERVER_PORT);
        } else {
            printf("Failed to bind to port %d: %d\n", SERVER_PORT, err);
        }
    }
}

void socket_task(void *p)
{
    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return;
    }

    cyw43_arch_enable_sta_mode();
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("Failed to connect to WiFi\n");
        return;
    }
    const ip4_addr_t *ip = netif_ip4_addr(netif_default);

    printf("Connected with IP Address: %s\n", ip4addr_ntoa(ip));

    start_server();

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        printf("%s: core%u\n", pcTaskGetName(NULL), get_core_num());
    }

    cyw43_arch_deinit(); 
}

int main()
{
    stdio_init_all();
    printf("Start Server\n");

    xTaskCreate(socket_task, "Socket Task", 256, NULL, 1, NULL);

    // Create a 3rd task
    // Force it to run on core1
    // TaskHandle_t task3_handle;
    // UBaseType_t uxCoreAffinityMask;
    // xTaskCreate(led_task, "LED_Task 3", 256, &arg3, 1, &( task3_handle ));
    // uxCoreAffinityMask = ( ( 1 << 1 ));
    // vTaskCoreAffinitySet( task3_handle, uxCoreAffinityMask );
    vTaskStartScheduler();

    while (true)
        ;
}
