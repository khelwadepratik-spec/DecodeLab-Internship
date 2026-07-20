#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long previousMillis = 0;
const long interval = 2000; // DHT22 minimum safe sampling interval

void setup() {
  Serial.begin(115200);
  dht.begin();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Env Monitor Init");
  delay(1000); // one-time startup delay is fine, not in loop()
  lcd.clear();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      lcd.setCursor(0, 0);
      lcd.print("Sensor Error    ");
      return;
    }

    char tempStr[8];
    char humStr[8];
    dtostrf(temperature, 4, 1, tempStr);
    dtostrf(humidity, 4, 1, humStr);

    Serial.print("Temperature: ");
    Serial.print(tempStr);
    Serial.print(" C  Humidity: ");
    Serial.print(humStr);
    Serial.println(" %");

    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(tempStr);
    lcd.print((char)223); // degree symbol
    lcd.print("C  ");

    lcd.setCursor(0, 1);
    lcd.print("Hum:  ");
    lcd.print(humStr);
    lcd.print("%   ");
  }

  // rest of loop stays unblocked here for other tasks
}