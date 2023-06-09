#ifndef ITEM_H
#define ITEM_H

#include <vector>
#include <cmath>
#include <string>
#include "MPU6050_6Axis_MotionApps20.h"

class item
{
protected:
    std::vector<float>* m_geometry = nullptr; //4

    std::vector<float>* start_pos = nullptr;
    std::vector<float>* end_pos = nullptr;

    std::vector<float>* m_rotate = nullptr; //3
    
    std::string name;

    item* parent = nullptr;

    
 
    uint8_t pinout;

    MPU6050* mpu;

private:

    int16_t ax, ay, az, gx, gy, gz;
    int mean_ax, mean_ay, mean_az, mean_gx, mean_gy, mean_gz, state = 0;
    int ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;
    
    const int buffersize = 100;
    const int acel_deadzone = 10;  // точность калибровки акселерометра (по умолчанию 8)
    const int gyro_deadzone = 6;   // точность калибровки гироскопа (по умолчанию 2)
    uint8_t I2C_addr_enable = 0x69;
    uint8_t i2C_addr_disable = 0x68;

    void meansensors();
    


    uint8_t fifoBuffer[45];
    uint32_t last_update =0;
    const uint32_t period_updated = 11;
    enum vec_type
    {
        geom = 0,
        rot = 1
    };

    bool check_input(std::vector<float>* input, vec_type type);

public:
    enum rotate
    {
        x = 0,
        y = 1,
        z = 2
    };

    enum geom
    {
        width = 0,
        height = 1,
        length = 2,
        scale = 3
    };

    item(std::vector<float>* sp = nullptr, std::string name = "", float w = 0.5, float h = 0.25, float l = 1, float sc = 0.5, float x_r = 0, float y_r = 0, float z_r = 0, item* prnt = nullptr, uint8_t pinout = 0, uint8_t I2C_addr_enable = 0x69, uint8_t I2C_addr_disable = 0x68);

        
    std::vector<float>* get_start_pos();
    std::vector<float>* get_end_pos();
    void set_start_pos(std::vector<float>* sp);

    std::vector<float>* get_geometry();
    float get_width();
    float get_height();
    float get_length();
    float get_scale();
    void set_width(float w);
    void set_height(float h);
    void set_length(float l);
    void set_scale(float s);

    std::vector<float>* get_rotate();
    float get_x_rotate();
    float get_y_rotate();
    float get_z_rotate();
    void set_x_rotate(float x);
    void set_y_rotate(float y);
    void set_z_rotate(float z);

    item* get_parent();
    void set_parent(item* prnt);

    std::string get_name();
    void set_name(std::string name);

    void set_pinout(uint8_t new_pin)
    {
        this->pinout = new_pin;
    }
    uint8_t get_pinout()
    {
        return this->pinout;
    }


    void calculate_end_pos();

    void mpu_initialize();
    void mpu_get_data();
    void mpu_calibrate();
};

#endif // ITEM_H
