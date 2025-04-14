#include "TA7774Mini.h"

// Конфигурация
#define FULL_STEPS 1700  // Полный ход заслонки
#define MOTOR_RPM 70     // Скорость вращения

TA7774Mini motor(12, 13, 14); // INA=12, INB=13, VS2B=14

void setup() {
    // Открыть заслонку
    motor.powerOn();
    motor.moveSteps(FULL_STEPS, MOTOR_RPM);
    motor.powerOff();
    delay(5000);
    
    // Закрыть заслонку
    motor.powerOn();
    motor.moveSteps(-FULL_STEPS, MOTOR_RPM);
    motor.powerOff();
}

void loop() {}