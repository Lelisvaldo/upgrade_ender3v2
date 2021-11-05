#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Thermistor.h>     //Includes Temperature Sensor Library

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 32    // OLED display height, in pixels
#define OLED_RESET     -1   // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Thermistor temp(A0);
/*******Communication ports with the Display*********/
/*    On an arduino UNO:       A4(SDA), A5(SCL)     */
/*    On an arduino MEGA 2560: 20(SDA), 21(SCL)     */
/*    On an arduino LEONARDO:   2(SDA),  3(SCL)     */
/****************************************************/
int touchPin = 4;
int ledPins[] = {6,7};
int relauPin = 8;

int pinCount = 2; 
bool buttonTouchState = false;
const int millisecondsToOff = 0;
float countTimerToOff = 180;

void setup() {
  Serial.begin(9600);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay(); // Clear the buffer
  
  pinMode(touchPin, INPUT);
  pinMode(relauPin, OUTPUT); 
  for (int thisPin = 5; thisPin < pinCount; thisPin++) {
    pinMode(ledPins[thisPin], OUTPUT);
  }

  digitalWrite(relauPin, LOW); // Defines that the relay is off, I use the connection normally on.
}

void loop() {
  int state = digitalRead(touchPin);

  if(state == true)
    onOffLed();
  
  countTimerToOff > millisecondsToOff ? countTimer(getTemperatura()) : digitalWrite(relauPin, HIGH);

  displayOled();
  delay(1000);
}

// SWITCH ON LED
void onOffLed(){
    buttonTouchState = !buttonTouchState;
    if(buttonTouchState){
        for (int thisPin = 5; thisPin < pinCount; thisPin++) {
            digitalWrite(ledPins[thisPin], HIGH);
        }
    }else{
        for (int thisPin = 5; thisPin < pinCount; thisPin++) {
            digitalWrite(ledPins[thisPin], LOW);
        }
   }
}
// GET TEMPERATURE
int getTemperatura(){
  int temperature = temp.getTemp();
  return temperature;
}

// INCREASE THE COUNTER TO DISCONNECT THE RELAY
void countTimer(int t){
  t <= 30 ? countTimerToOff -= 0.05 : countTimerToOff = 0;
}

void displayOled() {
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);
  display.print("Hot-End: ");
  display.print(getTemperatura());
  display.write(248);
  display.print("C");

  display.setCursor(0, 15);
  display.print("Desligara em \nT -");
  display.print(countTimerToOff);

  display.display();

}
