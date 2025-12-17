#include <LiquidCrystal.h>

// adjust these to YOUR wiring
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

void setup() {
  lcd.begin(16, 2);
  lcd.print("Hello, world!");
}

void loop() {
  lcd.setCursor(0, 40);
  static int n = 0;
  lcd.print("Count: ");
  lcd.print(n++);
  delay(1000);
}