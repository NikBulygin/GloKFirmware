#include "item.h"


item::item(std::vector<float>* sp, std::string name, float w, float h, float l, float sc, float x_r, float y_r, float z_r, item* prnt, uint8_t pinout, uint8_t I2C_addr_enable, uint8_t I2C_addr_disable)
{
    this->set_parent(prnt);
    this->m_geometry = new std::vector<float>(4);
    this->start_pos = new std::vector<float>(3);
    this->end_pos = new std::vector<float>(3);
    this->m_rotate = new std::vector<float>(3);

    this->set_start_pos(sp);
    this->set_width(w);
    this->set_height(h);
    this->set_length(l);
    this->set_scale(sc);

    this->set_x_rotate(x_r);
    this->set_y_rotate(y_r);
    this->set_z_rotate(z_r);

    this->name = name;

    this->pinout = pinout;
    this->I2C_addr_enable = I2C_addr_enable;
    this->i2C_addr_disable = I2C_addr_disable;

    this->calculate_end_pos();
    
}



std::vector<float>* item::get_start_pos()
{
    return this->start_pos;
}

std::vector<float>* item::get_end_pos()
{
    return this->end_pos;
}

void item::set_start_pos(std::vector<float>* sp)
{
    if(this->parent != nullptr)
    {
        this->start_pos = this->parent->get_end_pos();
    }
    else
    {
        if(sp == nullptr)
        {
            this->start_pos->at(item::rotate::x) = 0.0f;
            this->start_pos->at(item::rotate::y) = 0.0f;
            this->start_pos->at(item::rotate::z) = 0.0f;

        }
        else
        {
            delete this->start_pos;
            this->start_pos = sp;
        }
    }
}


std::vector<float>* item::get_geometry()
{
    return this->m_geometry;
};

float item::get_width()
{
    return this->m_geometry->at(item::geom::width);
}

float item::get_height()
{
    return this->m_geometry->at(item::geom::height);
}

float item::get_length()
{
    return this->m_geometry->at(item::geom::length);
};

float item::get_scale()
{
    return this->m_geometry->at(item::geom::scale);
}

void item::set_width(float w)
{
    this->m_geometry->at(item::geom::width) = w;
}

void item::set_height(float h)
{
    this->m_geometry->at(item::geom::height) = h;
}

void item::set_length(float l)
{
    this->m_geometry->at(item::geom::length) = l;
}

void item::set_scale(float s)
{
    this->m_geometry->at(item::geom::scale) = s;
}

std::vector<float>* item::get_rotate()
{
    return this->m_rotate;
}

float item::get_x_rotate()
{
    return this->m_rotate->at(item::rotate::x);
}

float item::get_y_rotate()
{
    return this->m_rotate->at(item::rotate::y);
}

float item::get_z_rotate()
{
    return this->m_rotate->at(item::rotate::z);
}

void item::set_x_rotate(float x)
{
    this->m_rotate->at(item::rotate::x) = x;
}

void item::set_y_rotate(float y)
{
    this->m_rotate->at(item::rotate::y) = y;
}

void item::set_z_rotate(float z)
{
    this->m_rotate->at(item::rotate::z) = z;
}

item* item::get_parent()
{
    return this->parent;
}

void item::set_parent(item* prnt)
{
    this->parent = prnt;
}

std::string item::get_name()
{
    return this->name;
}

void item::set_name(std::string name)
{
    this->name = name;
}

void item::calculate_end_pos()
{


      // Перевод углов поворота из градусов в радианы
  float rotation_x = this->m_rotate->at(item::rotate::x)  * M_PI / 180.0;
  float rotation_y = this->m_rotate->at(item::rotate::y) * M_PI / 180.0;
  float rotation_z = this->m_rotate->at(item::rotate::z) * M_PI / 180.0;

  // Вычисляем половину длины, ширины и высоты параллелепипеда
  double half_length = this->get_length() / 2.0;
  double half_width =  this->get_width() / 2.0;
  double half_height = this->get_length() / 2.0;

  // Поворачиваем координаты точки вокруг осей координат
  this->end_pos->at(item::rotate::x) = this->start_pos->at(item::rotate::x) + (half_length * cos(rotation_y) * cos(rotation_z) -
                         half_width * cos(rotation_y) * sin(rotation_z) +
                         half_height * sin(rotation_y)) * cos(rotation_x) -
                         (half_length * sin(rotation_z) +
                         half_width * cos(rotation_z)) * sin(rotation_x);
  this->end_pos->at(item::rotate::y) = this->start_pos->at(item::rotate::y) + (half_length * cos(rotation_y) * cos(rotation_z) -
                         half_width * cos(rotation_y) * sin(rotation_z) +
                         half_height * sin(rotation_y)) * sin(rotation_x) +
                         (half_length * sin(rotation_z) +
                         half_width * cos(rotation_z)) * cos(rotation_x);
  this->end_pos->at(item::rotate::z) = this->start_pos->at(item::rotate::z) - (half_length * cos(rotation_y) * sin(rotation_z) +
                         half_width * cos(rotation_y) * cos(rotation_z) +
                         half_height * sin(rotation_y));


}