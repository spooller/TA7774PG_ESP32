#include "TA7774Damper.h"
TA7774Damper damper(12, 13, 14, 1700, 100); // Пины, макс.шаги, макс.RPM

void setup() {
    damper.open(); // Начать открытие
}

void loop() {
    damper.update();
    
    if(!damper.isMoving()) {
        // Переключение состояний каждые 5 сек
        static bool is_open = true;
        delay(5000);
        is_open ? damper.close() : damper.open();
        is_open = !is_open;
    }
}