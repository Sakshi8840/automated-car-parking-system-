#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;

int IR1 = 2;
int IR2 = 3;
int Slot = 3; // Total number of parking Slots

unsigned long lastActionTime = 0; // Tracks the last time an action was performed
bool carEntering = false;
bool carExiting = false;
const unsigned long actionDelay = 3000; // 3-second delay

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  myservo.attach(5); // Attach the servo motor to pin 5
  myservo.write(100); // Initial position of the servo (closed)

  lcd.setCursor(0, 0);
  lcd.print("     ARDUINO    ");
  lcd.setCursor(0, 1);
  lcd.print("PARKING SYSTEM");
  delay(2000);
  lcd.clear();
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if a car is entering
  if (digitalRead(IR1) == LOW && !carEntering && (currentMillis - lastActionTime > actionDelay)) {
    if (Slot > 0) {
      Serial.println("Car entering");
      myservo.write(0); // Move to open position
      lastActionTime = currentMillis; // Record the time of the last action
      carEntering = true;
      Slot--;
    } else {
      lcd.setCursor(0, 0);
      lcd.print("SORRY :(");
      lcd.setCursor(0, 1);
      lcd.print("Parking Full");
      delay(3000);
      lcd.clear();
    }
  }

  // Check if a car is exiting
  if (digitalRead(IR2) == LOW && !carExiting && (currentMillis - lastActionTime > actionDelay)) {
    Serial.println("Car exiting");
    myservo.write(0); // Move to open position
    lastActionTime = currentMillis; // Record the time of the last action
    carExiting = true;
    Slot++;
  }

  // After handling the car (entering or exiting), close the servo
  if (carEntering || carExiting) {
    if (currentMillis - lastActionTime > actionDelay) {
      Serial.println("Servo moving to closed position");
      myservo.write(100); // Move to closed position
      carEntering = false;
      carExiting = false;
      lastActionTime = currentMillis; // Record the time of the last action
    }
  }

  // Update the LCD display
  lcd.setCursor(0, 0);
  lcd.print("WELCOME!");
  lcd.setCursor(0, 1);
  lcd.print("Slot Left: ");
  lcd.print(Slot);
}