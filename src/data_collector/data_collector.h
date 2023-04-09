#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H


#include <Wire.h>
#include <ArduinoJson.h>
#include <MPU6050.h>
#include "../item/i_adapter.h"

class data_collector
{
    protected:
        bool loop_execute = false;
        i_adapter* i_adp;

    public:
        data_collector(i_adapter* i_adp)
        {
            this->i_adp = i_adp;
        }

        virtual void start()
        {

            delay(100);

            //prepare pinout
            for(int i = 0; i < this->i_adp->get_count(); i++)
            {
                uint8_t pin = this->i_adp->get_item_by_id(i)->get_pinout();
                #if GLOK_DEBUG_ON
                Serial.print("Data_Collector:");
                Serial.println(i);
                Serial.print("Pin:");
                Serial.println(pin);
                #endif
                pinMode(pin, OUTPUT);
                digitalWrite(pin, LOW);
            }

            //dmp_initialize
            for(int i = 0; i < this->i_adp->get_count();i++)
            {
                digitalWrite(this->i_adp->get_item_by_id(i)->get_pinout(), HIGH);
                this->i_adp->get_item_by_id(i)->mpu_initialize();
                digitalWrite(this->i_adp->get_item_by_id(i)->get_pinout(), LOW);
            }



            this->loop_execute = true;
            while(this->loop_execute)
            {
                if(!this->i_adp->get_flag_calibrate())
                {
                    for(int i = 0; i < this->i_adp->get_count(); i++)
                    {
                        uint8_t pin = this->i_adp->get_item_by_id(i)->get_pinout();
                        #if GLOK_DEBUG_ON
                            Serial.print(pin);
                            Serial.println(":HIGH");
                        #endif
                        digitalWrite(pin, HIGH);
                        
                        #if GLOK_DEBUG_ON
                            Serial.print(pin);
                            Serial.println(":LOW");
                        #endif
                        this->i_adp->get_item_by_id(i)->mpu_get_data();
                        digitalWrite(pin, LOW);
                    }
                }
                else
                {
                  for(int i = 0; i < this->i_adp->get_count(); i++)
                    {
                            uint8_t pin = this->i_adp->get_item_by_id(i)->get_pinout();
                            #if GLOK_DEBUG_ON
                                Serial.print(pin);
                                Serial.println(":HIGH");
                            #endif
                            digitalWrite(pin, HIGH);
                            this->i_adp->get_item_by_id(i)->mpu_calibrate();

                            
                            #if GLOK_DEBUG_ON
                                Serial.print(pin);
                                Serial.println(":LOW");
                            #endif
                            digitalWrite(pin, LOW);
                            this->i_adp->set_flag_calibrate(false);
                    }   
                }
            }
        }
        virtual void stop()
        {
            this->loop_execute = false;
        };

        static String I2C_Scanner()
        {
            Wire.begin();
            DynamicJsonDocument json_output(1024);

            int n_devices = 0;
            for(byte address = 1; address < 127; address++)
            {
                Wire.beginTransmission(address);
                byte error = Wire.endTransmission();
                if(error == 0)
                {
                    String s_addr = "0x";
                    if (address < 16)
                    {
                        s_addr += "0";
                    }
                    s_addr += String(address, HEX);
                    json_output["address"]["know"][n_devices] = s_addr;
                    n_devices++;
                }
                else if (error == 4)
                {
                    String s_addr = "0x";
                    if (address < 16)
                    {
                        s_addr += "0";
                    }
                    s_addr += String(address, HEX);
                    json_output["address"]["unknow"][n_devices] = s_addr;
                    n_devices++;
                }
            }
            
            String result;
            serializeJson(json_output, result);
            // result += (char) 0x1A; // EOF char
            return result;
        }

};

#endif // DATACOLLECTOR_H
