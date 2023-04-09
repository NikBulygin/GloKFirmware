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

    this->mpu = new MPU6050(MPU6050_ADDRESS_AD0_HIGH);
    
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

void item::mpu_initialize()
{
    this->mpu->initialize();
    this->mpu->dmpInitialize();
    this->mpu->setDMPEnabled(true);
    this->mpu->setFullScaleAccelRange(MPU6050_ACCEL_FS_16);
    this->mpu->setFullScaleGyroRange(MPU6050_GYRO_FS_2000);

    this->mpu->setXAccelOffset(0);
    this->mpu->setYAccelOffset(0);
    this->mpu->setZAccelOffset(0);
    this->mpu->setXGyroOffset(0);
    this->mpu->setYGyroOffset(0);
    this->mpu->setZGyroOffset(0);
}

void item::mpu_get_data()
{
    if(millis() - this->last_update >= this->period_updated)
    {
        if (this->mpu->dmpGetCurrentFIFOPacket(fifoBuffer)) {
            // переменные для расчёта (ypr можно вынести в глобал)
            Quaternion q;
            VectorFloat gravity;
            float ypr[3];
            // расчёты
            this->mpu->dmpGetQuaternion(&q, fifoBuffer);
            this->mpu->dmpGetGravity(&gravity, &q);
            this->mpu->dmpGetYawPitchRoll(ypr, &q, &gravity);
      

            this->set_z_rotate(ypr[0] * (180 / M_PI));
            this->set_y_rotate(ypr[1] * (180 / M_PI));
            this->set_x_rotate(ypr[2] * (180 / M_PI));

            this->last_update = millis();  // сброс таймера
        }
    }
}



/*too long
void item::meansensors() {
  long i = 0, buff_ax = 0, buff_ay = 0, buff_az = 0, buff_gx = 0, buff_gy = 0, buff_gz = 0;
  while (i < (buffersize + 101)) { // read raw accel/gyro measurements from device
    this->mpu->getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    if (i > 100 && i <= (buffersize + 100)) { //First 100 measures are discarded
      buff_ax = buff_ax + ax;
      buff_ay = buff_ay + ay;
      buff_az = buff_az + az;
      buff_gx = buff_gx + gx;
      buff_gy = buff_gy + gy;
      buff_gz = buff_gz + gz;
    }
    if (i == (buffersize + 100)) {
      mean_ax = buff_ax / buffersize;
      mean_ay = buff_ay / buffersize;
      mean_az = buff_az / buffersize;
      mean_gx = buff_gx / buffersize;
      mean_gy = buff_gy / buffersize;
      mean_gz = buff_gz / buffersize;
    }
    i++;
    delay(2);
  }
}

void item::mpu_calibrate()
{
    ax_offset = -mean_ax / 8;
    ay_offset = -mean_ay / 8;
    az_offset = (16384 - mean_az) / 8;
    gx_offset = -mean_gx / 4;
    gy_offset = -mean_gy / 4;
    gz_offset = -mean_gz / 4;
  while (1) {
    int ready = 0;
    this->mpu->setXAccelOffset(ax_offset);
    this->mpu->setYAccelOffset(ay_offset);
    this->mpu->setZAccelOffset(az_offset);
    this->mpu->setXGyroOffset(gx_offset);
    this->mpu->setYGyroOffset(gy_offset);
    this->mpu->setZGyroOffset(gz_offset);
    meansensors();
    if (abs(mean_ax) <= acel_deadzone) ready++;
    else ax_offset = ax_offset - mean_ax / acel_deadzone;
    if (abs(mean_ay) <= acel_deadzone) ready++;
    else ay_offset = ay_offset - mean_ay / acel_deadzone;
    if (abs(16384 - mean_az) <= acel_deadzone) ready++;
    else az_offset = az_offset + (16384 - mean_az) / acel_deadzone;
    if (abs(mean_gx) <= gyro_deadzone) ready++;
    else gx_offset = gx_offset - mean_gx / (gyro_deadzone + 1);
    if (abs(mean_gy) <= gyro_deadzone) ready++;
    else gy_offset = gy_offset - mean_gy / (gyro_deadzone + 1);
    if (abs(mean_gz) <= gyro_deadzone) ready++;
    else gz_offset = gz_offset - mean_gz / (gyro_deadzone + 1);
    if (ready == 6) break;
  }

}

*/

void item::mpu_calibrate()
{
    long offsets[6];
    long offsetsOld[6];
    int16_t mpuGet[6];
    // используем стандартную точность
    this->mpu->setFullScaleAccelRange(MPU6050_ACCEL_FS_16);
    this->mpu->setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
  // обнуляем оффсеты
    this->mpu->setXAccelOffset(0);
    this->mpu->setYAccelOffset(0);
    this->mpu->setZAccelOffset(0);
    this->mpu->setXGyroOffset(0);
    this->mpu->setYGyroOffset(0);
    this->mpu->setZGyroOffset(0);
    delay(10);
     for (byte n = 0; n < 10; n++) {     // 10 итераций калибровки
        for (byte j = 0; j < 6; j++) {    // обнуляем калибровочный массив
            offsets[j] = 0;
    }

    for (byte i = 0; i < 100 + buffersize; i++) { // делаем BUFFER_SIZE измерений для усреднения
      this->mpu->getMotion6(&mpuGet[0], &mpuGet[1], &mpuGet[2], &mpuGet[3], &mpuGet[4], &mpuGet[5]);
      if (i >= 99) {                         // пропускаем первые 99 измерений
        for (byte j = 0; j < 6; j++) {
          offsets[j] += (long)mpuGet[j];   // записываем в калибровочный массив
        }
      }
    }

    for (byte i = 0; i < 6; i++) {
      offsets[i] = offsetsOld[i] - ((long)offsets[i] / buffersize); // учитываем предыдущую калибровку
      if (i == 2) offsets[i] += 16384;                               // если ось Z, калибруем в 16384
      offsetsOld[i] = offsets[i];
    }
    // ставим новые оффсеты
    this->mpu->setXAccelOffset(offsets[0] / 8);
    this->mpu->setYAccelOffset(offsets[1] / 8);
    this->mpu->setZAccelOffset(offsets[2] / 8);
    this->mpu->setXGyroOffset(offsets[3] / 4);
    this->mpu->setYGyroOffset(offsets[4] / 4);
    this->mpu->setZGyroOffset(offsets[5] / 4);
    delay(2);
  }
}