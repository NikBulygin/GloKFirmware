#ifndef I_ADAPTER_H
#define I_ADAPTER_H

#include "item.h"
#include <vector>
#include <unordered_map>

class i_adapter
{
    protected:
        std::vector<item*>* items;
        std::unordered_map<std::string, int> map;
        bool flag_get_data = false;
        bool flag_start_calibrate = false;

    public:

    bool get_flag_calibrate()
    {
        return this->flag_start_calibrate;
    }
    void set_flag_calibrate(bool val)
    {
        this->flag_start_calibrate = val;
    }

    void start_loop_get_data()
    {
        this->flag_get_data = true;
    }
    void stop_loop_get_data()
    {
    this->flag_get_data = false;
    }

    bool get_flag_loop_of_get_data()
    {
        return this->flag_get_data;
    }

    i_adapter()
    {
        this->items = new std::vector<item*>;

        this->items->push_back(new item(
            new std::vector<float>{-1,0,0}, // start pos
            "thumb_left", //name
            0.5, //w
            0.25, //h
            1, // l
            0.5, //s
            0, //xr
            0, //xy
            0, //xz
            nullptr, //parent,
            13  // pinout
            ));
            map["thumb_left"] = this->get_count() - 1;


        this->items->push_back(new item(
            new std::vector<float>{0,0,2}, // start pos
            "index_main_left", //name
            0.5, //w
            0.25, //h
            1, // l
            0.5, //s
            0, //xr
            0, //xy
            0, //xz
            nullptr, //parent
            12  // pinout
            ));
            map["index_main_left"] = this->get_count() - 1;
            
        this->items->push_back(new item(
            nullptr, // start pos
            "index_second_left", //name
            0.5, //w
            0.25, //h
            1, // l
            0.5, //s
            0, //xr
            0, //xy
            0, //xz
            this->get_item_by_name("index_main_left"), //parent
            27  // pinout
            ));
            map["index_second_left"] = this->get_count() - 1;


    }

    item* get_item_by_name(std::string name)
    {
        if(this->map.find(name) != this->map.end())
            return this->items->at(this->map[name]);
        else
            return nullptr;
    }

    item* get_item_by_id(int i)
        {
            return i < this->get_count() ? this->items->at(i) : nullptr;
        }

    


    int get_count()
    {
        return this->items->size();
    }
};

#endif // I_ADAPTER_H
