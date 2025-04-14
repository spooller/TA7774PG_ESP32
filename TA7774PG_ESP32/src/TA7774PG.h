#ifndef TA7774PG_H
#define TA7774PG_H

#include <Arduino.h>

class TA7774PG {
public:
    TA7774PG(uint8_t pinINA, uint8_t pinINB, uint8_t pinVS2B);
    void begin();
    void moveSteps(int steps, uint32_t speed_rpm = 60);
    void setPosition(uint8_t percent);
    bool isOpen() const;
    bool isClosed() const;
    void calibrate();
    void enable();
    void disable();
    int getCurrentPositionPercent() const; // Добавленный метод

private:
    uint8_t _pinINA, _pinINB, _pinVS2B;
    int _currentPosition = 0;
    static const int STEPS_PER_REV = 300;
    static const int MAX_STEPS = STEPS_PER_REV * 2;
    void stepMotor(uint8_t step);
};

#endif