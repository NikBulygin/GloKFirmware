#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H


#include <Wire.h>
#include <ArduinoJson.h>
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
            this->loop_execute = true;
            while(this->loop_execute)
            {

            }
        };
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
