//Menu Grundstruktur

#include <Arduino.h>

#include <OneButton.h>
// Setup a new OneButton on named pin  
OneButton button1(4, true);
//OneButton button2(7, true);

#include "DrehEnco.h"
//Setup a new encoder
DrehEnco DrehEncoEins(5, 6);

//#include <SPI.h>
#include <Wire.h>
//include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino NANO:       A4(SDA), A5(SCL)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3C for 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include "lmenu.h"
//Menue wichtige Eintraege 
menu a1,a2,a3,a4,a1a1,a1a2,a1a3,a2a1,a2a2,a2a3; //Menues entsprechend des struct definieren

//Menue-Eintraege hier vornehmen
char* a1text= "A1: fct or submenu";
char* a2text= "A2: fct or submenu";
char* a3text= "A3: fct or submenu";
char* a4text= "A4: fct or submenu";
char* a1a1text= "A1A1: subm of A1";
char* a1a2text= "A1A2: subm of A1";
char* a1a3text= "A1A3: subm of A1";
char* a2a1text= "A2A1: submenu of A2";
char* a2a2text= "A2A2: submenu of A2";
char* a2a3text= "A2A3: submenu of A2";

int vorZurueckB; //Variable die Mittels Knopf verändert wird; muss manuell zurück gesetzt werden

int setValFlag;  //Variable für Eingabe einer Variablen

//ENDE Menue wichtige Eintraege 

//declaration of functions
void click1();
void doubleclick1();
void longPressStart1();
void longPress1();
void longPressStop1();

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
  
  // link the doubleclick function to be called on a doubleclick event.
    // link the button 1 functions.
  button1.attachClick(click1);
  button1.attachDoubleClick(doubleclick1);
  button1.attachLongPressStart(longPressStart1);
  button1.attachLongPressStop(longPressStop1);
  button1.attachDuringLongPress(longPress1);
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_EXTERNALVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    Serial.println(F("WTF!?"));
    delay(2000);
  }
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    Serial.println(F("Really WTF!?"));
    delay(2000);
  }
  else {
    Serial.println("Display should do");
  }
    
  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);
  delay(2000);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10,20);
  display.println("--Start--");
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setCursor(20,20);
  display.println("--Continue--");
  display.display();
  
  Serial.println("++Start++"); //debug
  //Setup Menue
  //menu_lcdInit(&lcd);
  menu_ssd1306_init(&display);
  //menu_ST7735_init(&tft);
  link_menu();
  menu_init(&a1, &setValFlag);
  menu_print3();
  menu_ssd1306Print();
  //ENDE Setup Menue
}

void loop() {
  // put your main code here, to run repeatedly:
  button1.tick();
  DrehEncoEins.check();
  int aufAbB = DrehEncoEins.getStep();
  
  workMenu(aufAbB, vorZurueckB);
  vorZurueckB = 0;
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
    //ptr_init(&multiplier_13, &toggleInterval_13, dutyUnit_blue2);
    //drawVarSet(toggleInterval_13);
    //drawVarSet_lcd(toggleInterval_13);
    //drawVarSet_ssd1306(toggleInterval_13);
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
//Main Menu
  a1.text = a1text;
  a1.up = &a4;
  a1.down = &a2;
  a1.ok = &a1a1;
  a1.back = &a1;

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