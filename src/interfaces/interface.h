#ifndef INTERFACE_H
#define INTERFACE_H

#include "../item/i_adapter.h"
#include "../api/api.h"
class interface
{
    protected:
        i_adapter* i_adp;
        bool loop_execute = false;
        bool loop_get_data = false;
        const unsigned long interval_get_data = 500;
        unsigned long last_time_get_data =  0; 
    public:
        interface(i_adapter* i_adp)
        {
            this->i_adp = i_adp;
        }

        virtual void begin();
        virtual void start();
        virtual void pause();
        virtual void close();
        virtual void start_get_data();
        virtual void stop_get_data();

};

#endif // INTERFACE_H
