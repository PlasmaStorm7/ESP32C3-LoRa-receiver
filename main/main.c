#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"
#include "lora.h"

#define PMODCLS_CS_PIN 9
uint8_t buf[50]={"buna\0"};
static spi_device_handle_t __spicls;


void PmodCls_Writestring(const char * p){
   
   //char out[50]={0};
   uint8_t length =strlen(p);
   uint8_t in[50];
   //uint8_t i=0;
   spi_transaction_t pmod = {
      .flags = 0,
      .length = 8 * length,
      .tx_buffer = (void*)p,
      .rx_buffer = in  
   };
   //printf("trimit %s spre display\n",p);
   //Ultimul element dintr-un sir e 0
   //while(p[i])//Verificam, daca nu e ultimul element
   //{
      //out[0]=p[i];
      //printf("sending %c to display\n",out[0]);
      if(spi_device_transmit(__spicls, &pmod) != ESP_OK)
      {
         printf("SPI WRITE ERROR!\n");
         return ;
      }
      //i++;
   //}
}

void clearScreen()
{
   //Secventa Escape pentru Clear screen si Return Home
   PmodCls_Writestring("\e[j");
   //printf("cleared display\n");
}

void HelloWorld()
{
   //Folosim secvente Escape: \e[Linie;ColoanaH    
   PmodCls_Writestring("\e[0;1HPMOD CLS DEMO \e[1;2HHello World");
   //PmodCls_Writestring("B");
   //printf("wrote hello world\n");
}

void init_display()
{
   esp_err_t retCLS;
   spi_device_interface_config_t pmodCLS = {
      .clock_speed_hz = 500000,
      .mode = 0,
      .spics_io_num = PMODCLS_CS_PIN, //pin ales ca CS pt PmodCLS
      .queue_size = 1,
      .flags = 0,
      .pre_cb = NULL
   };
   gpio_pad_select_gpio(PMODCLS_CS_PIN);
   gpio_set_direction(PMODCLS_CS_PIN, GPIO_MODE_OUTPUT);

   retCLS = spi_bus_add_device(SPI2_HOST, &pmodCLS, &__spicls);
   assert(retCLS == ESP_OK);
   PmodCls_Writestring("\e[0*");
   vTaskDelay(50);
   //enable display and backlight(doesn't seem to work)
   PmodCls_Writestring("\e[3e");
   vTaskDelay(50);
   //disable cursor
   PmodCls_Writestring("\e[0c");
   vTaskDelay(50);
}

void task_displayMsg(void *p)
{
   init_display();
   for(;;){
      vTaskDelay(20);
      clearScreen();
      PmodCls_Writestring((char*)buf);
      //printf("wrote message to display");
   }
   vTaskDelete(NULL);
}

void task_rx(void *p)
{
   int x;
   for(;;) {
      lora_receive();    // put into receive mode
      while(isRxDone()) {
         x = lora_receive_packet(buf, sizeof(buf));
         resetRxDone();
         if(x==0){
            printf("CRC Error with %.2f SNR and %d RSSI\n",lora_packet_snr(),lora_packet_rssi());
            break;
         }
         buf[x] = 0;
         printf("Received: \"%s\" with %.2f SNR and %d RSSI\n", buf,lora_packet_snr(),lora_packet_rssi());
         lora_receive();
      }
      //xTaskCreate(&task_displayMsg,"task_displayMsg",2048,NULL,5,NULL);
      //vTaskDelete(task_displayHandle);
      vTaskDelay(1);
   }
   vTaskDelete(NULL);
}

void task_display(void *p)
{
   init_display();
   vTaskDelay(pdMS_TO_TICKS(1000));
   for(;;){
      clearScreen();
      HelloWorld();
      vTaskDelay(pdMS_TO_TICKS(6000));
      clearScreen();
      PmodCls_Writestring((char*)buf);      
      printf("wrote message to display\n");
      vTaskDelay(pdMS_TO_TICKS(6000));
   }
   vTaskDelete(NULL);
}

void app_main()
{
   
   lora_init();
   lora_set_frequency(8683e5);
   lora_enable_crc();
   
   xTaskCreate(&task_rx, "task_rx", 2048, NULL, 5, NULL);
   xTaskCreate(&task_displayMsg,"task_displayMsg",2048,NULL,4,NULL);
}


