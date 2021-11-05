/*******************************************************************************
*
*    Projeto: Hello Word! - COMUNICAÇÃO SPI
*    Data: 30/09/2020
*            http://squids.com.br/arduino
*
*******************************************************************************/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const byte SCREEN_WIDTH = 128; // OLED display width, in pixels
const byte SCREEN_HEIGHT = 64; // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI  11
#define OLED_CLK   13
#define OLED_DC     9
#define OLED_CS    10
#define OLED_RESET  8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void setup() {

// initialize with the SPI pins
display.begin(SSD1306_SWITCHCAPVCC); 

// Clear the buffer.
display.clearDisplay();
  
// Display Text "Hello Word"
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,28);
display.println("Hello Word!");
display.display();
}

void loop() {}
