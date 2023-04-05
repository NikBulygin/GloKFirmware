#ifndef BLUETOOTH_INTERFACE_H
#define BLUETOOTH_INTERFACE_H

#include "../interface.h"
#include <Arduino.h>
#include <BluetoothSerial.h>

class bluetooth_interface : interface
{
    private:
        String name;
        String password;
        BluetoothSerial* SerialBT;

    public:
        bluetooth_interface(i_adapter* i_adp, String name = "test_esp32_keyboard", String password = "1234") : interface(i_adp)
        {
            this->name = name;
            this->password = password;
            this->SerialBT = new BluetoothSerial();
            
        }

        void begin() override
        {
            this->SerialBT->begin(name, false);
            this->SerialBT->setPin(password.c_str());

        }
        void start() override
        {
            this->loop_execute = true;
            while(this->loop_execute)
            {
                if(this->SerialBT->hasClient())
                {
                    if(this->SerialBT->available())
                    {
                        String data = this->SerialBT->readString();
                        this->SerialBT->println(glok_api::parse_request(data, *(this->i_adp)));
                    }
                }
                else 
                {
                    if(this->i_adp->get_flag_loop_of_get_data() && millis() - this->last_time_get_data >= this->interval_get_data)
                    {
                        Serial.println(glok_api::parse_request("{\"data\":1}", *(this->i_adp)));   
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
            this->SerialBT->flush();
            this->SerialBT->end();
        }

        void start_get_data() override
        {
            this->loop_get_data = true;
        }

        virtual void stop_get_data() override
        {
            this->loop_get_data = false;
        }

};

#endif // BLUETOOTH_INTERFACE_H
