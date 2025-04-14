#include <TA7774PG.h>

// ===== Конфигурация пинов =====
#define PIN_INA  12  // Управление фазой A
#define PIN_INB  13  // Управление фазой B
#define PIN_VS2B 14 // Режим Standby (HIGH - включен)
#define ENDSTOP_CLOSE -1  // Концевик "Закрыто" (опционально)
#define ENDSTOP_OPEN  -1  // Концевик "Открыто" (опционально)

TA7774PG motor(PIN_INA, PIN_INB, PIN_VS2B);

// ===== Настройки двигателя =====
const int STEPS_PER_REV = 2000;   // Шагов на 1 оборот (NEMA 17)
const int MAX_TRAVEL_STEPS = 1700; // Полный ход заслонки в шагах
const int DEFAULT_RPM = 100;      // Скорость по умолчанию

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  // Инициализация концевиков (если есть)
  pinMode(ENDSTOP_CLOSE, INPUT_PULLUP);
  pinMode(ENDSTOP_OPEN, INPUT_PULLUP);

  // Настройка двигателя
  motor.begin();
  motor.calibrate();  // Калибровка (с концевиками или без)

  Serial.println(F("=== Functional Test Started ==="));
  Serial.println(F("1. Полное открытие (100%)"));
  Serial.println(F("2. Закрытие (0%)"));
  Serial.println(F("3. Позиции 25%, 50%, 75%"));
  Serial.println(F("4. Ручное управление (шаги)"));
}

void loop() {
  // === Тест 1: Полное открытие ===
  Serial.println("\n[Тест 1] Открытие на 100%");
  motor.setPosition(1700);
  printPosition();
  delay(2000);

  // === Тест 2: Полное закрытие ===
  Serial.println("\n[Тест 2] Закрытие на 0%");
  motor.setPosition(0);
  printPosition();
  delay(2000);

  // === Тест 3: Промежуточные позиции ===
  Serial.println("\n[Тест 3] Позиционирование");
  for (int pos : {25, 50, 75}) {
    Serial.printf("Перемещение на %d%%\n", pos);
    motor.setPosition(pos);
    printPosition();
    delay(1000);
  }

  // === Тест 4: Ручное управление шагами ===
  Serial.println("\n[Тест 4] Ручные шаги (+200/-100)");
  motor.moveSteps(200, 30);  // 200 шагов вперед, 30 RPM
  printPosition();
  motor.moveSteps(-100);     // 100 шагов назад (скорость по умолчанию)
  printPosition();
  delay(2000);

  // === Тест 5: Режимы питания ===
  Serial.println("\n[Тест 5] Управление питанием");
  Serial.println("Выключение драйвера (Standby)");
  motor.disable();
  delay(1000);
  Serial.println("Включение драйвера");
  motor.enable();
  delay(1000);
}

// ===== Вспомогательные функции =====
void printPosition() {
  Serial.printf("Текущая позиция: %d%%\n", motor.getCurrentPositionPercent());
  Serial.printf("Состояние: %s\n", 
    motor.isOpen() ? "Открыто" : 
    motor.isClosed() ? "Закрыто" : "Промежуточное");
}

// ===== Доработка библиотеки =====
// Добавьте в TA7774PG.h:
//   int getCurrentPositionPercent() const { 
//     return map(_currentPosition, 0, MAX_STEPS, 0, 100); 
//   }