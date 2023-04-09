#include <Arduino.h>
#include "item/i_adapter.h"

#define GLOK_INCLUDE_SERIAL_PORT_BLOCK 1
#define GLOK_INCLUDE_BLUETOOTH_BLOCK 1
#define GLOK_INCLUDE_GET_DATA_BLOCK 1
#define GLOK_INCLUDE_CALCULATION_BLOCK 0
#define GLOK_DEBUG_ON 0
// //GLOBAL VARIABLE
i_adapter i_adp;


#if GLOK_INCLUDE_SERIAL_PORT_BLOCK
#include "interfaces/serial_interface/serial_interface.h"
TaskHandle_t communication_serial_port;

void com_serial( void * pvParameters ){
  serial_interface ser_in(&i_adp);
  ser_in.begin();
  for(;;){
    ser_in.start();
    delay(700);
  }
}
#endif

#if GLOK_INCLUDE_BLUETOOTH_BLOCK
#include "interfaces/bluetooth_interfacce/bluetooth_interface.h"
TaskHandle_t commnunication_ble_port;
void com_blue( void * pvParameters ){
  bluetooth_interface ble_in(&i_adp);
  ble_in.begin();
  for(;;){
    ble_in.start();
    delay(700);
  }
}
#endif

#if GLOK_INCLUDE_GET_DATA_BLOCK
#include "data_collector/data_collector.h"

TaskHandle_t get_data;
void get_data_func( void * pvParameters ){
  data_collector dtcol(&i_adp);
  dtcol.start();

}
#endif

#if GLOK_INCLUDE_CALCULATION_BLOCK
TaskHandle_t calculate_position;
void calculate_data( void * pvParameters ){
  for(;;){
    delay(700);
  }
}
#endif



void setup() {
  Wire.begin();
  #if GLOK_INCLUDE_SERIAL_PORT_BLOCK
    //создаем задачу, которая будет выполняться на ядре 0 с максимальным приоритетом (1)
  xTaskCreatePinnedToCore(
                    com_serial,   /* Функция задачи. */
                    "SerialCommunication",     /* Ее имя. */
                    10000,       /* Размер стека функции */
                    NULL,        /* Параметры */
                    1,           /* Приоритет */
                    &communication_serial_port,      /* Дескриптор задачи для отслеживания */
                    1);          /* Указываем пин для данного ядра */                  
  delay(500);
  #else
    Serial.begin(115200); 
  #endif

  #if GLOK_INCLUDE_BLUETOOTH_BLOCK
    //создаем задачу, которая будет выполняться на ядре 0 с максимальным приоритетом (1)
  xTaskCreatePinnedToCore(
                    com_blue,   /* Функция задачи. */
                    "BluetothCommunication",     /* Ее имя. */
                    10000,       /* Размер стека функции */
                    NULL,        /* Параметры */
                    1,           /* Приоритет */
                    &commnunication_ble_port,      /* Дескриптор задачи для отслеживания */
                    1);          /* Указываем пин для данного ядра */                  
  delay(500); 
  #endif

  #if GLOK_INCLUDE_GET_DATA_BLOCK
    //создаем задачу, которая будет выполняться на ядре 0 с максимальным приоритетом (1)
  xTaskCreatePinnedToCore(
                    get_data_func,   /* Функция задачи. */
                    "DataCollector",     /* Ее имя. */
                    10000,       /* Размер стека функции */
                    NULL,        /* Параметры */
                    1,           /* Приоритет */
                    &get_data,      /* Дескриптор задачи для отслеживания */
                    0);          /* Указываем пин для данного ядра */                  
  delay(500); 
  #endif

  #if GLOK_INCLUDE_CALCULATION_BLOCK
    //создаем задачу, которая будет выполняться на ядре 0 с максимальным приоритетом (1)
  xTaskCreatePinnedToCore(
                    calculate_data,   /* Функция задачи. */
                    "CalculationData",     /* Ее имя. */
                    10000,       /* Размер стека функции */
                    NULL,        /* Параметры */
                    1,           /* Приоритет */
                    &calculate_position,      /* Дескриптор задачи для отслеживания */
                    0);          /* Указываем пин для данного ядра */                  
  delay(500); 
  #endif
}

void loop() {
}
