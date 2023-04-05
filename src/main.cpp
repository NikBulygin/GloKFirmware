#include <Arduino.h>
#include "item/i_adapter.h"

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  Serial.println(millis());
}

// #define GLOK_INCLUDE_SERIAL_PORT_BLOCK 0
// #define GLOK_INCLUDE_BLUETOOTH_BLOCK 0
// #define GLOK_INCLUDE_GET_DATA_BLOCK 0
// #define GLOK_INCLUDE_CALCULATION_BLOCK 0;

// //GLOBAL VARIABLE
// i_adapter i_adp;


// #ifdef GLOK_INCLUDE_SERIAL_PORT_BLOCK
// #include "interfaces/serial_interface/serial_interface.h"
// TaskHandle_t communication_serial_port;

// void com_serial( void * pvParameters ){
//   serial_interface ser_in(&i_adp);
//   ser_in.begin();
//   for(;;){
//     ser_in.start();
//     delay(700);
//   }
// }
// #endif

// #ifdef GLOK_INCLUDE_BLUETOOTH_BLOCK
// #include "interfaces/bluetooth_interfacce/bluetooth_interface.h"
// TaskHandle_t commnunication_ble_port;
// void com_blue( void * pvParameters ){
//   bluetooth_interface ble_in(&i_adp);
//   ble_in.begin();
//   for(;;){
//     ble_in.start();
//     delay(700);
//   }
// }
// #endif

// #ifdef GLOK_INCLUDE_GET_DATA_BLOCK

// TaskHandle_t get_data;
// void data_collector( void * pvParameters ){
//   for(int i = 0; i < i_adp.get_count(); i++)
//   {
//     pinMode(i_adp.get_item_by_id(i)->get_pinout(), OUTPUT);
//     digitalWrite(i_adp.get_item_by_id(i)->get_pinout(), LOW);
//   }

//   for(;;){
//     for(int i = 0; i < i_adp.get_count(); i++)
//     {
//       digitalWrite(i_adp.get_item_by_id(i)->get_pinout(), HIGH);
//       delay(100);
//       digitalWrite(i_adp.get_item_by_id(i)->get_pinout(), LOW);
//     }
//     delay(700);
//   }
// }
// #endif

// #ifdef GLOK_INCLUDE_CALCULATION_BLOCK
// TaskHandle_t calculate_position;
// void calculate_data( void * pvParameters ){
//   for(;;){
//     delay(700);
//   }
// }
// #endif



// void setup() {
//   #ifdef GLOK_INCLUDE_SERIAL_PORT_BLOCK
//     //создаем задачу, которая будет выполняться на ядре 0 с максимальным приоритетом (1)
//   xTaskCreatePinnedToCore(
//                     com_serial,   /* Функция задачи. */
//                     "SerialCommunication",     /* Ее имя. */
//                     10000,       /* Размер стека функции */
//                     NULL,        /* Параметры */
//                     1,           /* Приоритет */
//                     &communication_serial_port,      /* Дескриптор задачи для отслеживания */
//                     1);          /* Указываем пин для данного ядра */                  
//   delay(500); 
//   #endif

//   #ifdef GLOK_INCLUDE_BLUETOOTH_BLOCK
//     //создаем задачу, которая будет выполняться на ядре 0 с максимальным приоритетом (1)
//   xTaskCreatePinnedToCore(
//                     com_blue,   /* Функция задачи. */
//                     "BluetothCommunication",     /* Ее имя. */
//                     10000,       /* Размер стека функции */
//                     NULL,        /* Параметры */
//                     1,           /* Приоритет */
//                     &commnunication_ble_port,      /* Дескриптор задачи для отслеживания */
//                     1);          /* Указываем пин для данного ядра */                  
//   delay(500); 
//   #endif

//   #ifdef GLOK_INCLUDE_GET_DATA_BLOCK
//     //создаем задачу, которая будет выполняться на ядре 0 с максимальным приоритетом (1)
//   xTaskCreatePinnedToCore(
//                     data_collector,   /* Функция задачи. */
//                     "DataCollector",     /* Ее имя. */
//                     10000,       /* Размер стека функции */
//                     NULL,        /* Параметры */
//                     1,           /* Приоритет */
//                     &get_data,      /* Дескриптор задачи для отслеживания */
//                     0);          /* Указываем пин для данного ядра */                  
//   delay(500); 
//   #endif

//   #ifdef GLOK_INCLUDE_CALCULATION_BLOCK
//     //создаем задачу, которая будет выполняться на ядре 0 с максимальным приоритетом (1)
//   xTaskCreatePinnedToCore(
//                     calculate_data,   /* Функция задачи. */
//                     "CalculationData",     /* Ее имя. */
//                     10000,       /* Размер стека функции */
//                     NULL,        /* Параметры */
//                     1,           /* Приоритет */
//                     &calculate_position,      /* Дескриптор задачи для отслеживания */
//                     0);          /* Указываем пин для данного ядра */                  
//   delay(500); 
//   #endif
  

// }

// void loop() {
//   if (Serial.available())
//   {
//     String data = Serial.readString();

//     String out = glok_api::parse_request(data, i_adp);
//     // Serial.write(out.c_str(), out.length());
//     Serial.println(out);
//   }
// }
