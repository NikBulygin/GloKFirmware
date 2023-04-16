#ifndef API_H
#define API_H

#include <ArduinoJson.h>
#include <vector>

#include "../item/item.h"
#include "../item/i_adapter.h"
#include "../interfaces/interface.h"
class glok_api
{
    public:
    static String parse_request (String input, i_adapter& i_adp)
    {
        DynamicJsonDocument json_input(1024);
        DynamicJsonDocument json_output(1024);


        if(deserializeJson(json_input, input) == DeserializationError::Ok)
        {
            if (json_input.containsKey("start_communication") || json_input.containsKey("start_calibrate"))
            {
                if(json_input["start_communication"] == 1 || json_input["start_calibrate"] == 1)
                {
                    i_adp.stop_loop_get_data();
                    i_adp.set_flag_calibrate(true);
                }
            }
            else if(json_input.containsKey("config"))
            {
                if(json_input["config"] == 1)
                {
                    for(int i = 0; i < i_adp.get_count(); i++)
                    {
                        json_output["config"][i]["name"] = i_adp.get_item_by_id(i)->get_name();

                        json_output["config"][i]["geometry"]["width"] = i_adp.get_item_by_id(i)->get_width();
                        json_output["config"][i]["geometry"]["height"] = i_adp.get_item_by_id(i)->get_height();
                        json_output["config"][i]["geometry"]["length"] = i_adp.get_item_by_id(i)->get_length();
                        json_output["config"][i]["geometry"]["scale"] = i_adp.get_item_by_id(i)->get_scale();

                        json_output["config"][i]["rotate"]["x"] = i_adp.get_item_by_id(i)->get_x_rotate();
                        json_output["config"][i]["rotate"]["y"] = i_adp.get_item_by_id(i)->get_y_rotate();
                        json_output["config"][i]["rotate"]["z"] = i_adp.get_item_by_id(i)->get_z_rotate();

                        json_output["config"][i]["start_pos"]["x"] = i_adp.get_item_by_id(i)->get_start_pos()->at(item::rotate::x);
                        json_output["config"][i]["start_pos"]["y"] = i_adp.get_item_by_id(i)->get_start_pos()->at(item::rotate::y);
                        json_output["config"][i]["start_pos"]["z"] = i_adp.get_item_by_id(i)->get_start_pos()->at(item::rotate::z);

                        if(i_adp.get_item_by_id(i)->get_parent() != nullptr)
                            json_output["config"][i]["parent"] = i_adp.get_item_by_id(i)->get_parent()->get_name();
                    }
                }
            }
            else if(json_input.containsKey("data"))
            {
                if(json_input["data"] == 1)
                {
                    for(int i = 0; i < i_adp.get_count(); i++)
                    {
                        json_output["data"][i]["name"] = i_adp.get_item_by_id(i)->get_name();

                        json_output["data"][i]["rotate"]["x"] = i_adp.get_item_by_id(i)->get_x_rotate();
                        json_output["data"][i]["rotate"]["y"] = i_adp.get_item_by_id(i)->get_y_rotate();
                        json_output["data"][i]["rotate"]["z"] = i_adp.get_item_by_id(i)->get_z_rotate();

                    }
                }
            }
            else if(json_input.containsKey("zero_data"))
            {
                if(json_input["zero_data"] == 1)
                {
                    for(int i = 0; i < i_adp.get_count(); i++)
                    {
                        json_output["zero_data"][i]["name"] = i_adp.get_item_by_id(i)->get_name();

                        json_output["zero_data"][i]["rotate"]["x"] = i_adp.get_item_by_id(i)->get_x_rotate();
                        json_output["zero_data"][i]["rotate"]["y"] = i_adp.get_item_by_id(i)->get_y_rotate();
                        json_output["zero_data"][i]["rotate"]["z"] = i_adp.get_item_by_id(i)->get_z_rotate();

                    }
                }
            }

            if(json_input.containsKey("start_loop_get_data"))
            {
                if(json_input["start_loop_get_data"] == 1)
                    i_adp.start_loop_get_data();
                else
                    i_adp.stop_loop_get_data();
            }
        }

        String result_js, result = "";
        serializeJson(json_output, result_js);
        result += (char) 0x1B;  //Start char
        result += result_js;
        result += (char) 0x1A; // EOF char
        return result;

    }
};

#endif // API_H
