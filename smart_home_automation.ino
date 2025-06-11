
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Pin Definitions
#define TRIG_PIN 9
#define ECHO_PIN 8
#define LDR_PIN A0
#define DHT_PIN 7
#define BULB_RELAY 2
#define FAN_RELAY 3

// Sensor Thresholds
#define DARK_THRESHOLD 500
#define TEMP_THRESHOLD 25.0
#define DISTANCE_THRESHOLD 100  // cm

// Sensor Setup
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C LCD at address 0x27
DHT dht(DHT_PIN, DHT11);

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BULB_RELAY, OUTPUT);
  pinMode(FAN_RELAY, OUTPUT);

  digitalWrite(BULB_RELAY, HIGH); // OFF (active LOW)
  digitalWrite(FAN_RELAY, HIGH);  // OFF (active LOW)

  lcd.begin();
  lcd.backlight();
  dht.begin();

  Serial.begin(9600);
}

void loop() {
  // Measure distance
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;

  // Read LDR and temperature
  int ldrValue = analogRead(LDR_PIN);
  float temp = dht.readTemperature();

  // Show on LCD
  lcd.setCursor(0, 0);
  lcd.print("D:");
  lcd.print((int)distance);
  lcd.print("cm L:");
  lcd.print(ldrValue);

  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print("C");

  // Bulb control: dark + presence
  if (distance < DISTANCE_THRESHOLD && ldrValue < DARK_THRESHOLD) {
    digitalWrite(BULB_RELAY, LOW); // ON
  } else {
    digitalWrite(BULB_RELAY, HIGH); // OFF
  }

  // Fan control: presence + temp > 25
  if (distance < DISTANCE_THRESHOLD && temp > TEMP_THRESHOLD) {
    digitalWrite(FAN_RELAY, LOW); // ON
  } else {
    digitalWrite(FAN_RELAY, HIGH); // OFF
  }

  delay(500);
}
