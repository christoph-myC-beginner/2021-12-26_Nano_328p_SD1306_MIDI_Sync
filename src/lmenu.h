#ifndef MENU_H
#define MENU_H

//#include <LiquidCrystal_I2C.h>
//#include <Adafruit_GFX.h>
//#include <Wire.h>
//#include <Adafruit_SSD1306.h>
//#include <Adafruit_ST7735.h>
//#include <U8g2lib.h>
#include "SSD1306AsciiWire.h"

typedef struct menu menu; //forward declaration & typedef for type menu

struct menu {
  char* text;   //text to output when this is active
  menu* up;   //pointer to whatever is 'up', can be also a reference to itself
  menu* down;   //dito for down
  menu* ok;   //where to go on 'ok' (enter), set to NULL if this is a leaf node
  menu* back;   //where to go on 'back'
  void (*function)(); //action to execute on 'ok' (enter), only for leaf nodes. ok must be NULL for a leaf node.
};

void menu_init(menu* erster, int* flagSetVal_f);

//void menu_lcdInit(LiquidCrystal_I2C* usedLCD);
//void menu_ssd1306_init(Adafruit_SSD1306* usedTFT);
//void menu_ST7735_init(Adafruit_ST7735* usedTFT);
//void menu_u8g2_init(U8G2_SSD1306_128X64_NONAME_1_HW_I2C* usedOLED);
void menu_ssd1306Ascii_init(SSD1306AsciiWire* usedOled);

void workMenu(int aufAb, int vorZurueck);

void ptr_init(int* multiPtr_f, int* varPtr_f, char* unitPtr_f);

void menu_up();
void menu_down();
void menu_ok();
void menu_back();

void menu_print();
void menu_print2();
void menu_print3();
//void menu_lcdPrint();
//void lcd_print();
void drawVarSet(int whichVar);
//void drawVarSet_lcd(int whichVar);
//void menu_ssd1306Print();
//void drawVarSet_ssd1306(int whichVar);
//void menu_ST7735Print();
//void drawVarSet_ST7735(int whichVar);
//void menu_u8g2Print();
//void drawVarSet_u8g2(int whichVar);
void menu_ssd1306AsciiPrint();
void drawVarSet_ssd1306Ascii(int whichVar);

#endif /* MENU_H */
