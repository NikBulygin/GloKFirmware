#ifndef SERIAL_INTERFACE_H
#define SERIAL_INTERFACE_H

#include "../interface.h"
#include <Arduino.h>

class serial_interface : interface
{
    private:
        int baudrate = 115200;
    public:
        serial_interface(i_adapter* i_adp, int baudrate = 115200) : interface(i_adp)
        {
            this->baudrate = baudrate;
        }

        void begin() override
        {
            Serial.begin(this->baudrate);
            Serial.println("Serial port ready for work");

        }
        void start() override
        {
            this->loop_execute = true;
            while(this->loop_execute)
            {
                if(Serial.available())
                {
                    String data = Serial.readString();
                    Serial.println(glok_api::parse_request(data, *(this->i_adp)));
                }
                else 
                {
                    if(this->i_adp->get_flag_loop_of_get_data() && millis() - this->last_time_get_data >= this->interval_get_data)
                    {
                        Serial.println(glok_api::parse_request("{\"data\":1}", *(this->i_adp)));   
                        this->last_time_get_data = millis();
                    }
                }
            }
        }
        void pause() override
        {
            this->loop_execute = false;

        }

        void close() override
        {
            Serial.flush();
            Serial.end();
        }


        void start_get_data() override
        {
            this->i_adp->start_loop_get_data();
        }

        virtual void stop_get_data() override
        {
            this->i_adp->stop_loop_get_data();
        }



};

#endif // SERIAL_INTERFACE_H
