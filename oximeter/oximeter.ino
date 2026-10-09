#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS 1000

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change 0x27 if needed
PulseOximeter pox;

uint32_t tsLastReport = 0;

void onBeatDetected() {
  Serial.println("Beat!");
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");

  if (!pox.begin()) {
    Serial.println("FAILED to initialize MAX30100");
    lcd.setCursor(0, 1);
    lcd.print("Sensor Error");
    while (1);
  }

  lcd.setCursor(0, 1);
  lcd.print("Sensor Ready");
  delay(1000);
  lcd.clear();

  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  pox.update();

  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    tsLastReport = millis();

    float bpm = pox.getHeartRate();
    float spo2 = pox.getSpO2();

    Serial.print("HR: ");
    Serial.print(bpm);
    Serial.print(" | SpO2: ");
    Serial.println(spo2);

    lcd.setCursor(0, 0);
    lcd.print("HR: ");
    if (bpm > 0) {
      lcd.print(bpm, 1);
      lcd.print(" bpm   ");
    } else {
      lcd.print("--- bpm   ");
    }

    lcd.setCursor(0, 1);
    lcd.print("SpO2: ");
    if (spo2 > 0) {
      lcd.print(spo2, 1);
      lcd.print(" %     ");
    } else {
      lcd.print("--- %     ");
    }
  }
}
