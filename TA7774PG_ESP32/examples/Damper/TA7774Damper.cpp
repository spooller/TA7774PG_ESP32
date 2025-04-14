#include "TA7774Damper.h"

const uint8_t STEP_SEQ[4][2] = {{1,0}, {1,1}, {0,1}, {0,0}};

TA7774Damper::TA7774Damper(uint8_t INA, uint8_t INB, uint8_t VS2B, 
                          uint16_t max_steps, uint16_t max_speed)
    : _ina(INA), _inb(INB), _vs2b(VS2B), 
      MAX_STEPS(max_steps) {
    pinMode(_ina, OUTPUT);
    pinMode(_inb, OUTPUT);
    pinMode(_vs2b, OUTPUT);
    setSpeed(max_speed);
    _powerOff();
}

void TA7774Damper::_doStep() {
    static uint8_t phase = 0;
    
    phase = (_direction) ? 
        (phase + 1) % 4 : 
        (phase + 3) % 4;
    
    digitalWrite(_ina, STEP_SEQ[phase][0]);
    digitalWrite(_inb, STEP_SEQ[phase][1]);
}

void TA7774Damper::_powerOn() {
    digitalWrite(_vs2b, HIGH);
    _is_moving = true;
}

void TA7774Damper::_powerOff() {
    digitalWrite(_vs2b, LOW);
    _is_moving = false;
}

void TA7774Damper::setSpeed(uint16_t rpm) {
    _step_delay = 60000000UL / (200 * rpm); // Для 200 шагов/оборот
}

void TA7774Damper::open() {
    if(_current_steps >= MAX_STEPS) return;
    _direction = true;
    _powerOn();
}

void TA7774Damper::close() {
    if(_current_steps <= 0) return;
    _direction = false;
    _powerOn();
}

void TA7774Damper::update() {
    static uint32_t last_step = 0;
    
    if(!_is_moving) return;
    
    if(micros() - last_step >= _step_delay) {
        _doStep();
        last_step = micros();
        
        _direction ? _current_steps++ : _current_steps--;
        
        // Автоотключение при достижении цели
        if((_direction && _current_steps >= MAX_STEPS) || 
           (!_direction && _current_steps <= 0)) {
            _powerOff();
        }
    }
}

bool TA7774Damper::isMoving() {
    return _is_moving;
}