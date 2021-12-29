#include "arduino.h"
#include "lmenu.h"
//#include <LiquidCrystal_I2C.h>
//#include <Adafruit_GFX.h>
//#include <Wire.h>
//#include <Adafruit_SSD1306.h>
//#include <Adafruit_ST7735.h>
//#include <U8g2lib.h>
#include "SSD1306AsciiWire.h"

//static LiquidCrystal_I2C* lcd_a;
//static Adafruit_SSD1306* ssd1306;
//static Adafruit_ST7735* ST7735;
//static U8G2_SSD1306_128X64_NONAME_1_HW_I2C* u8g2_f;
static SSD1306AsciiWire* ssd1306;

static menu* current;

static int* setValFlag;
static int* multiPtr;
static int* varPtr;
static char* unitPtr;

void menu_init(menu* erster, int* flagSetVal_f){
  current = erster;
  setValFlag = flagSetVal_f;
}

void ptr_init(int* multiPtr_f, int* varPtr_f, char* unitPtr_f){
  multiPtr = multiPtr_f;
  varPtr = varPtr_f;
  unitPtr = unitPtr_f;
}

/*
void menu_lcdInit(LiquidCrystal_I2C* usedLCD) {
  lcd_a = usedLCD;
}
void menu_ssd1306_init(Adafruit_SSD1306* usedTFT) {
  ssd1306 = usedTFT;
}
void menu_ST7735(Adafruit_ST7735* usedLCD) {
  ST7735 = usedLCD;
}
void menu_u8g2_init(U8G2_SSD1306_128X64_NONAME_1_HW_I2C* usedOLED) {
  u8g2_f = usedOLED;
}
*/

void menu_ssd1306Ascii_init(SSD1306AsciiWire* usedOled) {
  ssd1306 = usedOled;
}

void menu_print(){
  menu* p = current;
  while(p->up != NULL) {
    p=p->up;
  }
  do{
    if(p==current) { 
      Serial.print(" ->");
    }
    else {
      Serial.print("   ");
    }
    Serial.println(p->text);
    p = p->down;
  }while(p!=NULL);
}

void menu_print2() {
  menu* p = current;
  p=p->back;
  if (current == p) {
    Serial.print(" ->");
    Serial.println(current->text);
    p=p->down;
    Serial.print("   ");
    Serial.println(p->text);
  }
  else {
    Serial.println(p->text);
    Serial.print("  ->");
    Serial.println(current->text);
  }
}

void menu_print3(){
  menu* p = current;
  do{
    if(p==current) { 
      Serial.print(" ->");
    }
    else {
      Serial.print("   ");
    }
    Serial.println(p->text);
    p = p->down;
  }while(p!=current);
  Serial.println(" ");
}

void drawVarSet(int whichVar) {
  Serial.println(current->text);
  Serial.print(whichVar);
  Serial.print(" ");
  Serial.println(unitPtr);
}
/*
void menu_lcdPrint() {
  lcd_a->backlight();
  lcd_a->clear();
  menu* p = current;
  p=p->back;
  if (current == p) {
    lcd_a->setCursor(0,0);
    lcd_a->print("->");
    lcd_a->print(current->text);
    
    p=p->down;
    
    lcd_a->setCursor(0,1);
    lcd_a->print("  ");
    lcd_a->print(p->text);
   
  }
  else {
    lcd_a->setCursor(0,0);
    lcd_a->print(p->text);
    lcd_a->setCursor(0,1);
    lcd_a->print("->");
    lcd_a->print(current->text);
  }
}

void drawVarSet_lcd(int whichVar) {
  lcd_a->backlight();
  lcd_a->clear();
  lcd_a->setCursor(0,0);
  lcd_a->print(current->text);
  lcd_a->setCursor(0,1);
  lcd_a->print(whichVar);
}

void menu_ssd1306Print() {
  ssd1306->clearDisplay();
  ssd1306->setTextSize(1);
  ssd1306->setTextColor(WHITE);
  menu* p = current;
  p=p->back;
  if (current == p) {
    ssd1306->setCursor(0,0);
    ssd1306->print("->");
    ssd1306->println(current->text);
    
    p=p->down;
    ssd1306->setCursor(0,10);
    ssd1306->print("  ");
    ssd1306->println(p->text);

    p=p->down;
    ssd1306->setCursor(0,20);
    ssd1306->print("  ");
    ssd1306->println(p->text);   
  }
  else {
    ssd1306->setCursor(0,0);
    ssd1306->print(p->text);
    ssd1306->println(": ");
    ssd1306->setCursor(6,12);
    ssd1306->print("->");
    ssd1306->println(current->text);
    p=current;
    p=p->down;
    ssd1306->setCursor(6,22);
    ssd1306->print("  ");
    ssd1306->println(p->text);
  }
  ssd1306->display();
}

void drawVarSet_ssd1306(int whichVar) {
  ssd1306->clearDisplay();
  ssd1306->setTextSize(1);
  ssd1306->setCursor(0,0);
  ssd1306->println(current->text);
  ssd1306->setCursor(0,16);
  ssd1306->setTextSize(2);
  ssd1306->print(whichVar);
  ssd1306->setCursor(52,16);
  ssd1306->println(unitPtr);
  ssd1306->display();
}

void menu_u8g2Print() {
  u8g2_f->clearBuffer();
  u8g2_f->setFont(u8x8_font_chroma48medium8_r);
  
  menu* p = current;
  p=p->back;
  if (current == p) {
    u8g2_f->drawStr(0,0,"->");
    u8g2_f->drawStr(12,0,current->text);
    
    p=p->down;
    u8g2_f->drawStr(0,10,"  ");
    u8g2_f->drawStr(12,10,p->text);

    p=p->down;
    u8g2_f->drawStr(0,20,"  ");
    u8g2_f->drawStr(12,20,p->text);  
  }
  else {
    u8g2_f->drawStr(0,0,p->text);
    u8g2_f->drawStr(12,0,": ");
    u8g2_f->drawStr(6,12,"->");
    u8g2_f->drawStr(12,12,current->text);
    p=current;
    p=p->down;
    u8g2_f->drawStr(6,22,"  ");
    u8g2_f->drawStr(12,22,p->text);
  }
  u8g2_f->sendBuffer();
}

void drawVarSet_u8g2(int whichVar) {
  u8g2_f->clearBuffer();
  u8g2_f->setFont(u8x8_font_chroma48medium8_r);
  u8g2_f->drawStr(0,0,current->text);
  //u8g2_f->drawStr(0,16,whichVar);
  u8g2_f->drawStr(52,16,unitPtr);
  u8g2_f->sendBuffer();
}

void menu_ST7735Print() {
  ST7735->fillScreen(ST77XX_BLACK);
  ST7735->setTextSize(1);
  ST7735->setTextColor(ST77XX_WHITE);
  menu* p = current;
  p=p->back;
  if (current == p) {
    ST7735->setCursor(0,0);
    ST7735->print("->");
    ST7735->println(current->text);
    
    p=p->down;
    ST7735->setCursor(0,10);
    ST7735->print("  ");
    ST7735->println(p->text);

    p=p->down;
    ST7735->setCursor(0,20);
    ST7735->print("  ");
    ST7735->println(p->text);   
  }
  else {
    ST7735->setCursor(0,0);
    ST7735->print(p->text);
    ST7735->println(": ");
    ST7735->setCursor(6,12);
    ST7735->print("->");
    ST7735->println(current->text);
    p=current;
    p=p->down;
    ST7735->setCursor(6,22);
    ST7735->print("  ");
    ST7735->println(p->text);
  }
  //ST7735->display();
}

void drawVarSet_ST7735(int whichVar) {
  ST7735->fillScreen(ST77XX_BLACK);
  ST7735->setTextSize(1);
  ST7735->setCursor(0,0);
  ST7735->println(current->text);
  ST7735->setCursor(0,16);
  ST7735->setTextSize(2);
  ST7735->print(whichVar);
  ST7735->setCursor(52,16);
  ST7735->println(unitPtr);
  //ST7735->display();
}
*/
void menu_ssd1306AsciiPrint() {
  ssd1306->clear();
  menu* p = current;
  p=p->back;
  if (current == p) {
    ssd1306->setCursor(0,0);
    ssd1306->setInvertMode(1);
    ssd1306->setFont(fixed_bold10x15); 
    ssd1306->print("    Menu     ");
    ssd1306->setInvertMode(0);
    p=p->up;
    ssd1306->setCursor(0,3);
    ssd1306->setFont(font8x8); //System5x7
    ssd1306->print("  /");
    ssd1306->println(p->text);

    p=current;
    ssd1306->setCursor(0,4);
    ssd1306->setFont(ZevvPeep8x16);
    //ssd1306->print(" ");
    ssd1306->println(p->text);
    
    p=p->down;
    ssd1306->setCursor(0,6);
    ssd1306->setFont(font8x8);
    ssd1306->print("  \\");
    ssd1306->println(p->text);
  }
  else {
    ssd1306->setInvertMode(1);
    ssd1306->setFont(fixed_bold10x15);
    ssd1306->print(p->text);
    ssd1306->println(": ");
    ssd1306->setInvertMode(0);
    p=current;
    p=p->up;
    ssd1306->setCursor(0,3);
    ssd1306->setFont(font8x8);
    ssd1306->print("  /");
    ssd1306->println(p->text);

    p=p->down;
    ssd1306->setCursor(0,4);
    ssd1306->setFont(ZevvPeep8x16);
    //ssd1306->print(" ");
    ssd1306->println(p->text);
    
    p=p->down;
    ssd1306->setCursor(0,6);
    ssd1306->setFont(font8x8);
    ssd1306->print("  \\");
    ssd1306->println(p->text);
  }
}

void drawVarSet_ssd1306Ascii(int whichVar) {
  ssd1306->clear();
  ssd1306->setInvertMode(1);
  ssd1306->setFont(fixed_bold10x15);
  ssd1306->setCursor(0,0);
  ssd1306->println(current->text);
  ssd1306->setInvertMode(0);
  ssd1306->set2X();
  ssd1306->setCursor(0,3);
  ssd1306->setFont(Verdana12_bold);
  ssd1306->print(whichVar);
  ssd1306->setCursor(72,3);
  ssd1306->println(unitPtr);
  ssd1306->set1X();
  //ssd1306->setCursor(0,7);
  //ssd1306->setFont(System5x7);
  //ssd1306->print("Push Confirms");
}

void menu_up(){
  if (current->up != NULL) {
    current = current->up;
  }
}

void menu_down(){
  if (current->down != NULL) {
    current = current->down;
  }
}

void menu_ok(){
  if(current->ok != NULL){
    current = current->ok;
    //Serial.println(current->text);
  }
  else if(current->function != NULL){
    current->function();
  }
}

void menu_back(){
  current = current->back;
  //Serial.println(current->text);
}

void workMenu(int aufAb, int vorZurueck) {
  if (aufAb != 0 | vorZurueck != 0) { //Menu-Encoder oder Menu-Knopf wurden benutzt
    if (aufAb == 1 && *setValFlag == 0) {
      menu_down();
      menu_print3();
      //menu_lcdPrint();
      //menu_ssd1306Print();
      //menu_ST7735Print();
      //menu_u8g2Print();
      menu_ssd1306AsciiPrint();
    }
    else if (aufAb == -1 && *setValFlag == 0) {
      menu_up();
      menu_print3();
      //menu_lcdPrint();
      //menu_ssd1306Print();
      //menu_ST7735Print();
      //menu_u8g2Print();
      menu_ssd1306AsciiPrint();
    }
    else if (*setValFlag == 1 && vorZurueck == 0){
      *varPtr = *varPtr + *multiPtr * aufAb; //erhöht bzw eniedrigt die zugewiesene Variable mittels zugewiesenem Multiplier
      drawVarSet(*varPtr);
      //drawVarSet_lcd(*varPtr);
      //drawVarSet_ssd1306(*varPtr);
      //drawVarSet_ST7735(*varPtr);
      //drawVarSet_u8g2(*varPtr);
      drawVarSet_ssd1306Ascii(*varPtr);
    }

    if (vorZurueck == 1) {
      menu_ok();
      if (*setValFlag == 0) {
        menu_print3();
        //menu_lcdPrint();
        //menu_ssd1306Print();
        //menu_ST7735Print();
        //menu_u8g2Print();
        menu_ssd1306AsciiPrint();
      }
      else if (*setValFlag == 1) {
      }
    }
    else if (vorZurueck == -1) {
      if (*setValFlag == 1) {
        *setValFlag = 0;
      }
      else { 
        menu_back();
      }
      menu_print3();
      //menu_lcdPrint();
      //menu_ssd1306Print();
      //menu_ST7735Print();
      //menu_u8g2Print();
      menu_ssd1306AsciiPrint();
    }
  }
}