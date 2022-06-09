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
uint8_t buf[50]={"salut\0"};
static spi_device_handle_t __spicls;
bool msgReady;


void PmodCls_Writestring(const char * p){
   
   uint8_t length =strlen(p);
   uint8_t in[50];
   spi_transaction_t pmod = {
      .flags = 0,
      .length = 8 * length,
      .tx_buffer = (void*)p,
      .rx_buffer = in  
   };
   //printf("trimit %s spre display\n",p);
   if(spi_device_transmit(__spicls, &pmod) != ESP_OK)
   {
      printf("SPI WRITE ERROR!\n");
      return ;
   }
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
   //enable display and backlight(doesn't seem to work)
   PmodCls_Writestring("\e[3e");
   //disable cursor
   PmodCls_Writestring("\e[0c");

}

void task_displayMsg(void *p)
{
   init_display();
   clearScreen();
   //HelloWorld();
   PmodCls_Writestring((char*)buf);//initial mesajul e "salut"
   for(;;){
      vTaskDelay(20);
      if(msgReady)
      {
         clearScreen();
         PmodCls_Writestring((char*)buf);
         //printf("wrote message to display");
         msgReady=false;
      }
   }
   vTaskDelete(NULL);
}

void task_rx(void *p)
{
   int x;
   
   for(;;) {
      //lora_receive();// put into receive mode
      while(isRxDone()) {
         x = lora_receive_packet(buf, sizeof(buf));
         resetRxDone();
         if(x==0){
            printf("CRC Error with %.2f SNR and %d RSSI\n",lora_packet_snr(),lora_packet_rssi());
            lora_receive();
            break;
         }
         msgReady=true;
         buf[x+1] = 0;//string terminator
         printf("Received: \"%s\" with %.2f SNR and %d RSSI\n", buf+1,lora_packet_snr(),lora_packet_rssi());
         //printf("Received: \"%s\" \n",buf+1);
         lora_receive();
      }
      vTaskDelay(10);
   }
   vTaskDelete(NULL);
}

void app_main()
{
   lora_init();
   lora_set_frequency(8683e5);//868.3Mhz, bandwidth 125Khz ,banda libera up to 1% duty cycle
   lora_enable_crc();
   lora_receive();
   xTaskCreate(&task_rx, "task_rx", 2048, NULL, 5, NULL);
   xTaskCreate(&task_displayMsg,"task_displayMsg",2048,NULL,6,NULL);
}


