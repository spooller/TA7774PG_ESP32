#include "TA7774PG.h"

TA7774PG::TA7774PG(uint8_t pinINA, uint8_t pinINB, uint8_t pinVS2B) 
    : _pinINA(pinINA), _pinINB(pinINB), _pinVS2B(pinVS2B) {}

void TA7774PG::begin() {
    pinMode(_pinINA, OUTPUT);
    pinMode(_pinINB, OUTPUT);
    pinMode(_pinVS2B, OUTPUT);
    enable();
}

void TA7774PG::stepMotor(uint8_t step) {
    const uint8_t steps[4][2] = {
        {LOW, LOW},    // Шаг 0
        {HIGH, LOW},   // Шаг 1
        {HIGH, HIGH},  // Шаг 2
        {LOW, HIGH}    // Шаг 3
    };
    digitalWrite(_pinINA, steps[step][0]);
    digitalWrite(_pinINB, steps[step][1]);
}

void TA7774PG::moveSteps(int steps, uint32_t speed_rpm) {
    if (speed_rpm == 0) return; // Защита от деления на 0
    
    uint64_t delay_us = 60000000UL / (STEPS_PER_REV * speed_rpm); // Расчет задержки
    uint8_t direction = (steps > 0) ? 1 : 0;
    steps = abs(steps);

    for (int i = 0; i < steps; i++) {
        static uint8_t currentStep = 0;
        currentStep = (direction) ? (currentStep + 1) % 4 : (currentStep + 3) % 4; // +3 вместо -1 для корректного перехода
        stepMotor(currentStep);
        delayMicroseconds(delay_us);
    }
    _currentPosition += (direction ? steps : -steps);
}

void TA7774PG::setPosition(uint8_t percent) {
    percent = constrain(percent, 0, 100); // Ограничение 0-100%
    int targetSteps = map(percent, 0, 100, 0, MAX_STEPS);
    moveSteps(targetSteps - _currentPosition);
}

bool TA7774PG::isOpen() const {
    return (_currentPosition >= MAX_STEPS * 0.95); // 95% для учета погрешностей
}

bool TA7774PG::isClosed() const {
    return (_currentPosition <= MAX_STEPS * 0.05); // 5% для учета погрешностей
}

void TA7774PG::calibrate() {
    // Простейшая калибровка (без концевиков)
    _currentPosition = 0;
    moveSteps(MAX_STEPS); // Пройти весь диапазон
    _currentPosition = MAX_STEPS;
    moveSteps(-MAX_STEPS); // Вернуться в начало
    _currentPosition = 0;
}

void TA7774PG::enable() {
    digitalWrite(_pinVS2B, HIGH); // Включить драйвер
}

void TA7774PG::disable() {
    digitalWrite(_pinVS2B, LOW); // Перевести в Standby
}

int TA7774PG::getCurrentPositionPercent() const {
    return map(_currentPosition, 0, MAX_STEPS, 0, 100);
}