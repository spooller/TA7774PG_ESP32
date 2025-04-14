// TA7774Mini.h - Ультра-лёгкая библиотека для драйвера TA7774PG
#pragma once
#include <Arduino.h>

class TA7774Mini {
public:
    TA7774Mini(uint8_t INA, uint8_t INB, uint8_t VS2B);
    
    // Всего 3 основные функции
    void moveSteps(int steps, uint16_t speed_rpm = 60); // Движение (+/- шаги)
    void powerOn();  // Включить драйвер (аналог enable)
    void powerOff(); // Выключить драйвер (аналог disable)

private:
    uint8_t _ina, _inb, _vs2b;
    void _doStep(uint8_t a, uint8_t b); // Внутренняя функция шага
};