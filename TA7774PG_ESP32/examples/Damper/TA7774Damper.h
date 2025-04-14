#pragma once
#include <Arduino.h>

class TA7774Damper {
public:
    // Конструктор с настройками
    TA7774Damper(uint8_t INA, uint8_t INB, uint8_t VS2B, 
                uint16_t max_steps = 2000, 
                uint16_t max_speed = 300);
    
    void open();       // Полное открытие
    void close();      // Полное закрытие
    void update();     // Неблокирующее обновление
    bool isMoving();   // Проверка движения
    void setSpeed(uint16_t rpm); // Установка скорости

private:
    uint8_t _ina, _inb, _vs2b;
    const uint16_t MAX_STEPS;
    uint16_t _current_steps = 0;
    uint32_t _step_delay;
    bool _is_moving = false;
    bool _direction;

    void _doStep();
    void _powerOn();
    void _powerOff();
};