// TA7774Mini.cpp - Реализация минималистичного управления
#include "TA7774Mini.h"

TA7774Mini::TA7774Mini(uint8_t INA, uint8_t INB, uint8_t VS2B) 
    : _ina(INA), _inb(INB), _vs2b(VS2B) {
    pinMode(_ina, OUTPUT);
    pinMode(_inb, OUTPUT);
    pinMode(_vs2b, OUTPUT);
    powerOn(); // Автовключение при инициализации
}

void TA7774Mini::_doStep(uint8_t a, uint8_t b) {
    digitalWrite(_ina, a);
    digitalWrite(_inb, b);
}

void TA7774Mini::moveSteps(int steps, uint16_t speed_rpm) {
    if (speed_rpm == 0) return;
    uint32_t step_delay = 60000000UL / (200 * speed_rpm); // 200 шагов/оборот
    
    uint8_t dir = steps > 0;
    steps = abs(steps);
    
    // Полношаговый режим (4 фазы)
    const uint8_t steps_seq[4][2] = {{1,0}, {1,1}, {0,1}, {0,0}};
    
    for (int i = 0; i < steps; i++) {
        uint8_t phase = dir ? i % 4 : 3 - (i % 4);
        _doStep(steps_seq[phase][0], steps_seq[phase][1]);
        delayMicroseconds(step_delay);
    }
}

void TA7774Mini::powerOn() {
    digitalWrite(_vs2b, HIGH);
}

void TA7774Mini::powerOff() {
    digitalWrite(_vs2b, LOW);
}