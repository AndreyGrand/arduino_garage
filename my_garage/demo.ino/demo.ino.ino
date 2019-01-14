/*
  Пример использования тактовой кнопки в ардуино.
  Кнопка подключена к пину 2.
*/
 
const int PIN_BUTTON = 5;
#define PIN_LED 13 
void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
//  digitalWrite(PIN_BUTTON, HIGH);
}
 
void loop() {
  // Получаем состояние кнопки и выводим в мониторе порта
  int buttonState = digitalRead(PIN_BUTTON);
  digitalWrite(PIN_LED, buttonState);
  Serial.println(buttonState);
  delay(50);
}
