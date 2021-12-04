#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Thermistor.h>     //Includes Temperature Sensor Library

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 32    // OLED display height, in pixels
#define OLED_RESET    -1    // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define TIMEOUT -180

Thermistor temp(A0);
/*******Communication ports with the Display**************************************/
/*    On an arduino UNO:       A4(SDA - CABLE WHITE), A5(SCL - CABLE GREEN)      */
/*    On an arduino MEGA 2560: 20(SDA - CABLE WHITE), 21(SCL - CABLE GREEN)      */
/*    On an arduino LEONARDO:   2(SDA - CABLE WHITE),  3(SCL - CABLE GREEN)      */
/*********************************************************************************/
int touchPin = 4;
int ledPins1 = 6;
int ledPins2 = 7;
int relauPin = 8;

bool buttonTouchState = false;
const int t = 35;
const int millisecondsToOff = 0;
float countTimerToOff = TIMEOUT;

void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.clearDisplay(); // Clear the buffer

  pinMode(touchPin, INPUT);
  pinMode(relauPin, OUTPUT);
  pinMode(ledPins1, OUTPUT);
  pinMode(ledPins2, OUTPUT);

  digitalWrite(relauPin, LOW); // Defines that the relay is off, I use the connection normally on.
}

void loop() {
  int state = digitalRead(touchPin);

  if (state == true)
    onOffLed();

  if(getTemperatura() <= t){
    countTimer();
  }else{
    countTimerToOff = TIMEOUT;
  }

  displayOled();
  delay(1000);
}

// SWITCH ON LED
void onOffLed() {
  buttonTouchState = !buttonTouchState;
  if (buttonTouchState) {
    digitalWrite(ledPins1, HIGH);
    digitalWrite(ledPins2, HIGH);
  } else {
    digitalWrite(ledPins1, LOW);
    digitalWrite(ledPins2, LOW);
  }
}
// GET TEMPERATURE
int getTemperatura() {
  int temperature = temp.getTemp();
  return temperature;
}

// INCREASE THE COUNTER TO DISCONNECT THE RELAY
void countTimer() {
    if(countTimerToOff < millisecondsToOff){
      countTimerToOff += 0.05;
    }else{
      digitalWrite(relauPin, HIGH);
    }
}

void displayOled() {
  //TEXT ABOUT THE TEMPERATURE OF HOT-END
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);
  display.print("Hot-End: ");
  display.print(getTemperatura());
  display.write(248);
  display.print("C");
  //TEXT ABOUT SHUTDOWN TIME
  display.setCursor(0, 12);
  display.print("Shutdown in T ");
  display.print(countTimerToOff);
  
  display.display();
}
