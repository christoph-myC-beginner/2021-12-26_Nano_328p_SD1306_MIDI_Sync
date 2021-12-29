//Menu Grundstruktur

#include <Arduino.h>

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C
// Define proper RST_PIN if required.
#define RST_PIN -1
SSD1306AsciiWire display;

#include <OneButton.h>
// Setup a new OneButton on named pin  
OneButton button1(4, true);
//OneButton button2(7, true);

#include "DrehEnco.h"
//Setup a new encoder
DrehEnco DrehEncoEins(5, 6);

#include <FastLED.h>
#define NUM_LEDS 8
// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 9
//#define CLOCK_PIN 13
// Define the array of leds
CRGB leds[NUM_LEDS];
#define BRIGHTNESS  15
unsigned long blinkMillis = 0;
int blinkZeit = 500;


#include "lmenu.h"
//Menue wichtige Eintraege 
menu a1,a2,a3,a4,a1a1,a1a2,a1a3,a2a1,a2a2,a2a3; //Menues entsprechend des struct definieren

//Menue-Eintraege hier vornehmen

static char* a1text= "A1: fct or submenu";
static char* a2text= "A2: fct or submenu";
static char* a3text= "A3: fct or submenu";
static char* a1a1text= "A1A1: subm of A1";
static char* a1a2text= "A1A2: subm of A1";
static char* a1a3text= "A1A3: subm of A1";
static char* a2a1text= "A2A1: submenu of A2";
static char* a2a2text= "A2A2: submenu of A2";
static char* a2a3text= "A2A3: submenu of A2";
static char* a4text= "A4: fct or submenu";
/*
char* a1text[]   PROGMEM= "A1: fct or submenu";
char* a2text[]   PROGMEM= "A2: fct or submenu";
char* a3text[]   PROGMEM= "A3: fct or submenu";
char* a4text[]   PROGMEM= "A4: fct or submenu";
char* a1a1text[] PROGMEM= "A1A1: subm of A1";
char* a1a2text[] PROGMEM= "A1A2: subm of A1";
char* a1a3text[] PROGMEM= "A1A3: subm of A1";
char* a2a1text[] PROGMEM= "A2A1: submenu of A2";
char* a2a2text[] PROGMEM= "A2A2: submenu of A2";
char* a2a3text[] PROGMEM= "A2A3: submenu of A2";
*/
int vorZurueckB; //Variable die Mittels Knopf verändert wird; muss manuell zurück gesetzt werden

int setValFlag;  //Variable für Eingabe einer Variablen

int toggleInterval = 100;
int toggleMulti = 5;
char* toggleUnit = "bpm";

//ENDE Menue wichtige Eintraege 

//declaration of functions
void click1();
void doubleclick1();
void longPressStart1();
void longPress1();
void longPressStop1();

void a1action();
void a2action();
void a3action();
void a4action();
void a1a1action();
void a1a2action();
void a1a3action();
void a2a1action();
void a2a3action();
void link_menu();
//End declaration of functions


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // enable the standard led on pin 13.
  pinMode(LED_BUILTIN, OUTPUT);      // sets the digital pin as output
  pinMode(DATA_PIN, OUTPUT);
  // link the doubleclick function to be called on a doubleclick event.
  // link the button 1 functions.
  button1.attachClick(click1);
  button1.attachDoubleClick(doubleclick1);
  button1.attachLongPressStart(longPressStart1);
  button1.attachLongPressStop(longPressStop1);
  button1.attachDuringLongPress(longPress1);
  
  //Setup of display SSD1306Ascii
  Wire.begin();
  Wire.setClock(400000L);
  display.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
  display.setFont(Arial14);

  display.clear();
  //display.setLetterSpacing(1);
  display.setCursor(10, 2);
  display.println("Last Change ");
  display.setCursor(10, 5);
  display.println(__DATE__);
  delay(2000);

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);  //
  FastLED.setBrightness(  BRIGHTNESS );

  Serial.println("++Start++"); //debug
  
  //Setup Menue
  //menu_lcdInit(&lcd);
  //menu_ssd1306_init(&display);
  //menu_ST7735_init(&tft);
  //menu_u8g2_init(&u8g2);
  menu_ssd1306Ascii_init(&display);
  link_menu();
  menu_init(&a1, &setValFlag);
  menu_print3();
  menu_ssd1306AsciiPrint();
  //menu_u8g2Print();
  //ENDE Setup Menue
}

void loop() {
  // put your main code here, to run repeatedly:
  button1.tick();
  DrehEncoEins.check();
  int aufAbB = DrehEncoEins.getStep();
  workMenu(aufAbB, vorZurueckB);
  vorZurueckB = 0;
  
  if(millis() - blinkMillis > blinkZeit) {
    blinkMillis = millis();
    static int i;
    
    if (i == 0) {
      leds[i] = CRGB::Red;
      leds[7] = CRGB::Black;
      i++;
    }
    else if(i > 7) {
      i = 0;
    }
    else {
      leds[i] = CRGB::PaleVioletRed;
      leds[i-1] = CRGB::Black;
      i++;
    }
    FastLED.show();
  }
  
}

// This function will be called when the button1 was pressed 1 time.
void click1() {
  Serial.println("Button 1 click.");
  vorZurueckB = 1;
} // click1


// This function will be called when the button1 was pressed 2 times in a short timeframe.
void doubleclick1() {
  Serial.println("Button 1 doubleclick.");
  vorZurueckB = -1;
  //lastUsed = millis();
} // doubleclick1


// This function will be called once, when the button1 is pressed for a long time.
void longPressStart1() {
  Serial.println("Button 1 longPress start");
} // longPressStart1


// This function will be called often, while the button1 is pressed for a long time.
void longPress1() {
  //Serial.println("Button 1 longPress...");
  //Serial.println(DrehEncoEins.getPosition());
} // longPress1


// This function will be called once, when the button1 is released after beeing pressed for a long time.
void longPressStop1() {
  Serial.println("Button 1 longPress stop");
} // longPressStop1

//Menue Funktionen bei Aufruf
void a1action(){ 
  Serial.println("A1 action!\n");
}

void a2action(){ 
  Serial.println("A2 action!\n");
}

void a3action(){
  //Serial.println("A3 action!\n");
  if (setValFlag == 0) {
    setValFlag = 1;
    //ptr_init(&dutyMultiServo, &dutyOnServo, dutyUnitServo);
    //drawVarSet(dutyOnServo);
    //drawVarSet_lcd(dutyOnServo);
    //drawVarSet_ssd1306(dutyOnServo);
  }
}

void a4action(){ 
  Serial.println("A4 action!\n");
  //testdrawcircle();
}

void a1a1action(){ 
  //Serial.println("A1a1 action!\n");
  if (setValFlag == 0) {
    setValFlag = 1;
    ptr_init(&toggleMulti, &toggleInterval, toggleUnit);
    //drawVarSet(toggleInterval_13);
    //drawVarSet_lcd(toggleInterval_13);
    drawVarSet_ssd1306Ascii(toggleInterval);
  }
}

void a1a2action(){ 
  //Serial.println("A1a2 action!\n");
  if (setValFlag == 0) {
    setValFlag = 1;
    //ptr_init(&dutyMulti_green, &dutyOn_green, dutyUnit_green);
    //drawVarSet(dutyOn_green);
    //drawVarSet_lcd(dutyOn_green);
    //drawVarSet_ssd1306(dutyOn_green);
  }
}

void a1a3action(){ 
  //Serial.println("A1a3 action!\n");
  if (setValFlag == 0) {
    setValFlag = 1;
    //ptr_init(&dutyMulti_blue2, &dutyOn_blue2, dutyUnit_blue2);
    //drawVarSet(dutyOn_blue2);
    //drawVarSet_lcd(dutyOn_blue2);
    //drawVarSet_ssd1306(dutyOn_blue2);
  }
}
void a2a1action(){ 
  //Serial.println("A2a1 action!\n");
  if (setValFlag == 0) {
    setValFlag = 1;
    //ptr_init(&multiplierHours, &hours, hoursUnit);
    //drawVarSet(hours);
    //drawVarSet_lcd(hours);
    //drawVarSet_ssd1306(hours);
  }
}
void a2a2action(){ 
  //Serial.println("A2a2 action!\n");
  if (setValFlag == 0) {
    setValFlag = 1;
    //ptr_init(&multiplierMinutes, &minutes, minutesUnit);
    //drawVarSet(minutes);
    //drawVarSet_lcd(minutes);
    //drawVarSet_ssd1306(minutes);
  }
}
void a2a3action(){ 
  //Serial.println("A2a3 action!\n");
  if (setValFlag == 0) {
    setValFlag = 1;
    //ptr_init(&multiplierSecond, &second, secondUnit);
    //drawVarSet(second);
    //drawVarSet_lcd(second);
    //drawVarSet_ssd1306(second);
  }
}
//LINK Menue
void link_menu(){
  Serial.println("Debug Menu linked");
//Main Menu
  a1.text = a1text;
  a1.up = &a4;
  a1.down = &a2;
  a1.ok = &a1a1;
  a1.back = &a1;
  a1.function = a1action;

  a2.text = a2text;
  a2.up = &a1;
  a2.down = &a3;
  a2.ok = &a2a1;
  a2.back = &a2;
  a2.function = a2action;

  a3.text = a3text;
  a3.up = &a2;
  a3.down = &a4;
  a3.ok = NULL;
  a3.back = &a3;
  a3.function = a3action;

  a4.text = a4text;
  a4.up = &a3;
  a4.down = &a1;
  a4.ok = NULL;
  a4.back = &a4;
  a4.function = a4action;

//Submenu
  a1a1.text = a1a1text;
  a1a1.up = &a1a3;
  a1a1.down = &a1a2;
  a1a1.ok = NULL;
  a1a1.back = &a1;
  a1a1.function = a1a1action;

  a1a2.text = a1a2text;
  a1a2.up = &a1a1;
  a1a2.down = &a1a3;
  a1a2.ok = NULL;
  a1a2.back = &a1;
  a1a2.function = a1a2action;

  a1a3.text = a1a3text;
  a1a3.up = &a1a2;
  a1a3.down = &a1a1;
  a1a3.ok = NULL;
  a1a3.back = &a1;
  a1a3.function = a1a3action;

  a2a1.text = a2a1text;
  a2a1.up = &a2a3;
  a2a1.down = &a2a2;
  a2a1.ok = NULL;
  a2a1.back = &a2;
  a2a1.function = a2a1action;

  a2a2.text = a2a2text;
  a2a2.up = &a2a1;
  a2a2.down = &a2a3;
  a2a2.ok = NULL;
  a2a2.back = &a2;
  a2a2.function = a2a2action;

  a2a3.text = a2a3text;
  a2a3.up = &a2a2;
  a2a3.down = &a2a1;
  a2a3.ok = NULL;
  a2a3.back = &a2;
  a2a3.function = a2a3action;
}