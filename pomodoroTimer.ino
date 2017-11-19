// Время таймера в минутах
#define POMODORO_TIME 1

// Используемые пины
#define LED_R 10
#define LED_Y 3
#define LED_G 9
#define LED_B 4
#define BUZZER 2
#define BUTTON 8

// Количество светодиодов
#define LED_COUNT 4

// Массив пинов со светодиодами
int leds[] = {LED_R,
              LED_G,
              LED_Y,
              LED_B
             };
// Переменная для хранения времени таймера в миллисекундах
unsigned long pomodoroTimeInMillis;

// Переменная для хранения времени переключения светодиодов
unsigned long ledSwitchTime;

// Переменная для хранения времени начала работы таймера
unsigned long lastTime = 0;

// Состояние таймера — выключен
bool pomodoroState = false;

void setup() {

  // Рассчитаем время таймера в миллисекундах
  pomodoroTimeInMillis = POMODORO_TIME * 60000;
  
  // Рассчитаем время переключения светодиодов
  ledSwitchTime = pomodoroTimeInMillis / LED_COUNT;
  
  // Переключим пины светодиодов в состояние выхода
  for (int i = 0; i < LED_COUNT; ++i) {
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {
  // Если был клик кнопки
  if (checkClick()) {
  
    // Запомним текущее время
    lastTime = millis();
    
    // Переключим состояние таймера вкл/выкл
    pomodoroState = !pomodoroState;
    
    // Выключим все светодиоды
    for (int i = 0; i < LED_COUNT; ++i) {
      digitalWrite(leds[i], LOW);
    }
  }
  
  // Если таймер включён
  if (pomodoroState) {
    
    // Рассчитаем прошедшее время
    unsigned long elapsedTime = millis() - lastTime;
    
    // Если прошедшее время меньше времени срабатывания таймера
    if (elapsedTime < pomodoroTimeInMillis) {
      
      // Рассчитаем, какой из светодиодов должен гореть
      unsigned long currentLed = elapsedTime / ledSwitchTime;
      
      // И зажжём этот светодиод
      digitalWrite(leds[currentLed], HIGH);
      
      // Если прошедшее время таймера больше времени срабатывания — звеним
    } else {
      finish();
    }
  }
}

// Функция для проверки клика кнопки
bool checkClick()
{
  // Переменная для хранения предыдущего состояния кнопки
  static bool buttonWasUp = true;
  bool result = false;

  // Читаем текущее состояние кнопки
  bool buttonIsUp = digitalRead(BUTTON);

  // Если кнопка была не нажата, а теперь нажата
  // значит был клик
  if (buttonWasUp && !buttonIsUp) {
    delay(100);
    result = true;
  }
  buttonWasUp = buttonIsUp;
  return result;
}

void finish() {
  // Переменная для хранения переключателя светодиодов
  bool toggle = true;
  
  // Попиликаем 10 раз...
  for (int i = 0; i < 10; i++) {
    tone(BUZZER, 500 + toggle * 500);
    
    // ... поочерёдно моргая парами светодиодов
    for (int i = 0; i < LED_COUNT; ++i) {
      digitalWrite(leds[i], toggle);
      toggle = !toggle;
    }
    toggle = !toggle;
    delay(100);
  }
  
  // Отключаем звук
  noTone(BUZZER);
  
  // Выключаем таймер
  pomodoroState = false;
}
