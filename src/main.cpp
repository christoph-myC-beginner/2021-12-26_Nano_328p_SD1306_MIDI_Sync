//Menu Grundstruktur

#include <Arduino.h>

//#include <MIDI.h>
#include <SoftwareSerial.h>
//using Transport = MIDI_NAMESPACE::SerialMIDI<SoftwareSerial>;
int rxPin = 11;
int txPin = 12;
SoftwareSerial debugSerial = SoftwareSerial(rxPin, txPin);
//Transport serialMIDI(midiSerial);
//MIDI_NAMESPACE::MidiInterface<Transport> MIDI((Transport&)serialMIDI);

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
OneButton button2(3, true);

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
int blinkZeit = 250;

#include "lmenu.h"
//Menue wichtige Eintraege 
menu a1,a2,a3,a4,a1a1,a1a2,a1a3,a2a1,a2a2,a2a3; //Menues entsprechend des struct definieren

//Menue-Eintraege hier vornehmen
static char* a1text= "A1: fct or submenu";
static char* a2text= "A2: fct or submenu";
static char* a3text= "A3: Tap Beat";
static char* a4text= "A4: Exit";
static char* a1a1text= "A1A1: subm of A1";
static char* a1a2text= "A1A2: subm of A1";
static char* a1a3text= "A1A3: subm of A1";
static char* a2a1text= "A2A1: submenu of A2";
static char* a2a2text= "A2A2: submenu of A2";
static char* a2a3text= "A2A3: submenu of A2";

int vorZurueckB; //Variable die Mittels Knopf verändert wird; muss manuell zurück gesetzt werden

int setValFlag;  //Variable für Eingabe einer Variablen

int toggleInterval = 100;
int toggleMulti = 5;
char* toggleUnit = "mys";

int bpm = 60;
int bpmMulti = 1;
char* bpmUnit = "bpm";

//ENDE Menue wichtige Eintraege 
unsigned long refreshMillis = 0;
unsigned long bPeriodMillis = 0;
unsigned long beatMidiMillisOne = 0;
unsigned long beatMidiMillisTwo = 0;
unsigned long beatMidiPeriod = 0;
unsigned long beatMillisTest = 0;
unsigned long beatMillisTestOne = 0;
unsigned long beatMillisTestTwo = 0;
unsigned long beatPeriod = 0;

int bpmMidi = 0; //beats per Minute durch Auswertung der Midi Schnittstelle
int msgInterval = 2900; //Interval mit dem die Serial Messages zum Debug ausgegeben werden
int refreshRate = 150; //refresh Rate des Displays in der showMain Routine
int beatIndex = 0;  // beat 1 + 2 + 3 + 4 + translates as 0 1 2 3 4 5 6 7
int beatIndexComp = 0;
int beatMidiIndexComp = 0;
int flashTestCnt = 0;
int beatInterruptCnt = 0;
int beatMidiCnt = 0;
int beatMidiIndex = 0;
int beatMidiInterruptCnt = 0;
int onTimeOne = 0;
int onTimeOnePercent = 20;
int onTimeHalfsPercent = 10;
int onTimeHalfs = 0;
int tapInterruptCnt = 0;
int tapCnt = 9;
#define tapPin 10
bool flashExtern = 1;
bool doTapFlag = 0;
bool refreshFlag = 1;
bool refreshSyncFlag = 1;
bool clearFlag = 1;
bool enterMenu = 0;
bool debugFlag = 0;

//declaration of functions
void serialMsg(int msgZeit); 

void click1();
void doubleclick1();
void longPressStart1();
void longPress1();
void longPressStop1();
void click2();
void doubleclick2();

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

bool showMain(int refreshRate_f, bool refreshFlag_f, bool clearFlag_f);
bool showMidiSync (bool refresh_f);
void showBeats(int beatIndex_f);
void flashLED(int beatIndex_f);
bool tapBeatOut (int beatIndex_f, bool doFlag_f);
ISR(TIMER2_COMPA_vect);

void handleMidiClock(void);
void handleISRResult(void);

void handleClock(void);
void handleStart(void);
void handleContinue(void);
void handleStop(void);
//End declaration of functions

///// SETUP /// SETUP /// SETUP /////
void setup() {
  // put your setup code here, to run once:
  Serial.begin(31250);
  // enable the standard led on pin 13.
  pinMode(LED_BUILTIN, OUTPUT);      // sets the digital pin as output
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(tapPin, OUTPUT);
  digitalWrite(tapPin, HIGH);
  pinMode(7, OUTPUT);
  // link the doubleclick function to be called on a doubleclick event.
  // link the button 1 functions.
  button1.attachClick(click1);
  button1.attachDoubleClick(doubleclick1);
  button1.attachLongPressStart(longPressStart1);
  button1.attachLongPressStop(longPressStop1);
  button1.attachDuringLongPress(longPress1);
  button2.attachClick(click2);
  button2.attachDoubleClick(doubleclick2);
  //MIDI Setup
  //MIDI.setHandleClock(handleClock);
  //MIDI.setHandleStart(handleStart);
  //MIDI.begin(); // default Channel 1
  debugSerial.begin(9600);
  
  //Setup of display SSD1306Ascii
  Wire.begin();
  Wire.setClock(400000L);
  display.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
  display.clear();
  display.setFont(Arial14);
  display.setCursor(10, 2);
  display.println("Last Change ");
  display.setCursor(10, 5);
  display.println(__DATE__);
  delay(200);
  display.clear();
  
  //Setup of WS2812 LEDs
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);  //
  FastLED.setBrightness(  BRIGHTNESS );  
  for (int i = 0; i<8; i++) {
    leds[i] = CRGB::Orange;
    FastLED.show();
    delay(40);
  }
  for (int i = 7; i>=0; i--) {
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(40);
  }
    
  //Setup Menue
  //menu_lcdInit(&lcd);
  //menu_ssd1306_init(&display);
  //menu_ST7735_init(&tft);
  //menu_u8g2_init(&u8g2);
  menu_ssd1306Ascii_init(&display);
  link_menu();
  menu_init(&a1, &setValFlag);
  //menu_print3();
  //menu_ssd1306AsciiPrint();
  //menu_u8g2Print();
  //ENDE Setup MENUE

  // timer2
  
  TCCR2A = 0; // set TCCR2A register to 0
  TCCR2B = 0; // set TCCR2B register to 0
  TCNT2  = 0; // set counter value to 0
  
  OCR2A = 255; // set compare match register
  
  TCCR2B |= (1<<CS02) | (1<<CS01)| (0<<CS00); // Set CS bits for 1:1024 prescaler 1, 1, 1 = 1024
                                              //                                  1, 1, 0 =  256
                                              //                                  1, 0, 1 =  128
                                              //                                  1, 0, 0 =   64
 
  TCCR2A |= (1 << WGM21); // turn off CTC mode
  TIMSK2 |= (1 << OCIE2A); // disable timer compare interrupt

  interrupts(); // Interrupts einschalten
  beatPeriod = 60000 / bpm;
  uint16_t zwiA = onTimeOnePercent / 10;
  onTimeOne = beatPeriod * zwiA / 10;
  uint16_t zwiB = onTimeHalfsPercent / 10;
  onTimeHalfs = beatPeriod * zwiB / 10; //onTimeHalfsPercent / 100;
}
//End of SETUP

///// LOOP /// LOOP /// LOOP /// LOOP /// LOOP /// LOOP /// LOOP /// LOOP /// LOOP /////
void loop() {
  // put your main code here, to run repeatedly:
  
  if (debugFlag == 1 && beatIndex == 7) {
    beatMillisTest = 0;
    beatMillisTestOne = 0;
    beatMillisTestTwo = 0;
  }
  else {}
  if (beatIndex == 0) {
    beatMillisTestOne = millis();
  }
  if (beatIndex == 2) {
    beatMillisTestTwo = millis();
    beatMillisTest = beatMillisTestTwo - beatMillisTestOne;
  }
  if (debugFlag == 0) {
    
  }
  
  if (beatIndex == 0 || beatIndex == 2 || beatIndex == 4 || beatIndex == 6) {
    digitalWrite(7, HIGH);
  }
  else {
    digitalWrite(7, LOW);
  }

  if (Serial.available() > 0 && doTapFlag == 0) {
    if (Serial.read() == 248) {
      handleMidiClock();
    }
    else if (Serial.read() == 250) {
      //beatInterruptCnt = 0;
      beatIndex = 0;
      beatMidiIndex = 0;
      beatIndexComp = 0;
      beatMidiIndexComp = 0;
    }
  }
  handleISRResult();
  doTapFlag = tapBeatOut(beatIndex, doTapFlag);

  if ( doTapFlag == 0 ) {
    serialMsg(msgInterval);
  }

  button1.tick();
  button2.tick();

  DrehEncoEins.check();

  int aufAbB = DrehEncoEins.getStep(); //liefert nur alle 10ms

  if (enterMenu == 1) {
    workMenu(aufAbB, vorZurueckB);
    vorZurueckB = 0;
  }
  else {
    if (aufAbB != 0) {
      bpm = bpm + aufAbB;
      refreshFlag = 1; //es hat sich was geändert -> mitteilen
      beatPeriod = 60000 / bpm;
      uint16_t zwiA = onTimeOnePercent / 10;
      onTimeOne = beatPeriod * zwiA / 10;
      uint16_t zwiB = onTimeHalfsPercent / 10;
      onTimeHalfs = beatPeriod * zwiB / 10; //onTimeHalfsPercent / 100;
    }
    refreshFlag = showMain(refreshRate, refreshFlag, clearFlag); //mit refreshFlag wird das Display aktualisiert, aber maximal alle x ms je nach refreshRate
    clearFlag = 0;
    refreshSyncFlag = showMidiSync (refreshSyncFlag);
    if (beatIndex == 7) {
      refreshSyncFlag = 1;
    }
    //showBeats(beatIndex);  
  }
  
  
  
  //flashLED(beatMidiIndex); //Ansteuerung der LEDs via FastLED abhaengig vom beatIndex
  
}
//End of LOOP ///End of LOOP ///End of LOOP ///End of LOOP ///

bool tapBeatOut (int beatIndex_f, bool doFlag_f) {
  static unsigned long tapMillis_f;
  static int beatIndexOld_f;
  static int tapCnt_f;
  if (doFlag_f == 1) {
    if (beatIndex_f != beatIndexOld_f) { // mach was nur beim Wechsel des BeatIndex
      beatIndexOld_f = beatIndex_f;      // beatIndex weiter geben
      tapMillis_f = millis();            // merk Dir den Zeitpunkt des Beatwechsels
      if (beatIndex_f == 0 || beatIndex_f == 2 || beatIndex_f == 4 || beatIndex_f == 6 ) { //nur die Indizes mit ganzen Zaehlzeiten
        digitalWrite(tapPin, LOW);              // setze den Ausgang LOW
        digitalWrite(LED_BUILTIN, LOW);
        tapCnt_f = tapCnt_f + 1;
        beatIndexComp = 0;
        beatMidiIndexComp = 0;
      }
    }
    else if (millis() - tapMillis_f > 10) { //nach Ablauf von 200ms nach dem Beatwechsel
      digitalWrite(tapPin, HIGH);                 //setze den Ausgang HIGH
      digitalWrite(LED_BUILTIN, HIGH);
      if (tapCnt_f > 4) {                   //nach Durchgang von 4 "Taps" setze doFlag_f auf "erledigt"
        tapCnt_f = 0;
        doFlag_f = 0;
      }
    }
  }
  return doFlag_f;
}

void handleMidiClock(void) {
  
  if(beatMidiCnt == 24) { //96 = 4* (24 PPQN) Achtung zugehoeriger Zaehler beatMidiCnt wird auf 1 zurueckgesetzt
    beatMidiCnt = 0;
    beatMidiMillisTwo = beatMidiMillisOne;
    beatMidiMillisOne = millis();
    beatMidiPeriod = (beatMidiMillisOne - beatMidiMillisTwo);
    bpmMidi = 60000/beatMidiPeriod;
    beatMidiIndex = beatMidiIndex + 2;
    if (beatMidiIndex > 7) {
      beatMidiIndex = 0;
    }
    beatMidiIndexComp = beatMidiIndexComp + 2;
  }
  
  beatMidiCnt = beatMidiCnt + 1;
}

bool showMain(int refreshRate_f, bool refreshFlag_f, bool clearFlag_f) {
  static unsigned long refreshMillis;
  if ( millis() - refreshMillis > refreshRate_f && refreshFlag_f == 1) {
    refreshMillis = millis();
    refreshFlag_f = 0;
    if (clearFlag_f == 1) {
      clearFlag_f = 0;
      display.clear();
    }
  display.setInvertMode(0);
  display.set2X();
  display.setFont(Verdana12_bold);
  display.setCursor(10, 0);
  if (bpm <100) {
    display.print("  ");
    display.print(bpm);
    display.setCursor(72, 0);
    display.println(bpmUnit);
  }
  else {
    display.print(bpm);
    display.setCursor(72, 0);
    display.println(bpmUnit);
  }
  display.set1X();
  display.setFont(fixed_bold10x15);
  display.setCursor(3, 4);
  display.setLetterSpacing(4);
  display.println("1+2+3+4+");    
  //display.setFont(font8x8); //ZevvPeep8x16
  //display.setCursor(2, 7);
  //display.print("sync");
  //display.setCursor(87, 7);
  //display.print(bpmMidi);
  }
  return refreshFlag_f; 
}

bool showMidiSync (bool refresh_f) {
  if (refresh_f == 1) {
    refresh_f = 0;
    display.setFont(font8x8); //ZevvPeep8x16
    display.setCursor(2, 7);
    display.print("sync");
    display.setCursor(40, 7);
    int zwiD = abs(beatPeriod-beatMidiPeriod);
    if (beatMidiPeriod < beatPeriod) {
      display.print("-");
    }
    else {
      display.print(" ");
    }
    display.print(zwiD);
    display.print("   ");
    display.setCursor(87, 7);
    if (bpm <100) {
      display.print("| ");
      display.print(bpmMidi);
      display.print(" ");
    }
    else {
      display.print("| ");
      display.print(bpmMidi);
      display.print(" ");
    }
  }
  else {

  }
  return refresh_f;
}

void showBeats(int beatIndex_f) {
  static int beatIndexOld_f;
  if (beatIndex_f != beatIndexOld_f) {
    beatIndexOld_f = beatIndex_f;
    display.setFont(fixed_bold10x15);
    display.setCursor(3, 4);
    display.setLetterSpacing(4);
    display.setInvertMode(1);
    switch(beatIndex_f) {
      case 0:
        display.setCursor(87, 4);
        display.setInvertMode(0);
        display.print("4+");
        display.setCursor(3, 4);
        display.setInvertMode(1);
        display.println("1"); 
        display.setInvertMode(0);
      break; 
      case 1:
        display.setInvertMode(0);
        display.print("1");
        display.setInvertMode(1);
        display.println("+");
        display.setInvertMode(0);
      break;
      case 2:
        display.setInvertMode(0);
        display.print("1+");
        display.setInvertMode(1);
        display.println("2"); 
        display.setInvertMode(0);
      break;
      case 3:
        display.setInvertMode(0);
        display.print("1+2");
        display.setInvertMode(1);
        display.println("+"); 
        display.setInvertMode(0);
      break;
      case 4:
        display.setInvertMode(0);
        display.print("1+2+");
        display.setInvertMode(1);
        display.println("3"); 
        display.setInvertMode(0);
      break;
      case 5:
        display.setInvertMode(0);
        display.print("1+2+3");
        display.setInvertMode(1);
        display.println("+"); 
        display.setInvertMode(0);
      break;
      case 6:
        display.setInvertMode(0);
        display.print("1+2+3+");
        display.setInvertMode(1);
        display.println("4"); 
        display.setInvertMode(0);
      break;
      case 7:
        display.setInvertMode(0);
        display.print("1+2+3+4");
        display.setInvertMode(1);
        display.println("+"); 
        display.setInvertMode(0);
      break;
      default:
      break;
    }
  }   
}

void flashLED(int beatIndex_f) {
  static unsigned long bPeriodMillis_f;
  static int beatIndexOld_f;
  static bool doFlagA_f;
  static bool doFlagB_f;
  static bool doFlagC_f;
  
  if (beatIndexOld_f != beatIndex_f) {
    beatIndexOld_f = beatIndex_f;
    doFlagA_f = 1;
    doFlagB_f = 1;
    doFlagC_f = 1;
    bPeriodMillis_f = millis();
  
    switch (beatIndex_f) {
      case 0:
          FastLED.setBrightness(30);
          leds[0] = CRGB::Red;
      break;
      case 1:  // beat 1+
          FastLED.setBrightness(3); 
          leds[1] = CRGB::Yellow;
      break;
      case 2:  //beat 2
          FastLED.setBrightness(15);
          leds[2] = CRGB::Blue;
      break;
      case 3:  //beat 2+
          FastLED.setBrightness(3);
          leds[3] = CRGB::Yellow;
      break;
      case 4:  //beat 3
          FastLED.setBrightness(15);
          leds[4] = CRGB::Blue;
      break;
      case 5:  //beat 3+
          FastLED.setBrightness(3);
          leds[5] = CRGB::Yellow;
      break;
      case 6:  // beat 4
          FastLED.setBrightness(15);
          leds[6] = CRGB::Blue;
      break;
      case 7:  //beat 4+
          FastLED.setBrightness(3);
          leds[7] = CRGB::Yellow; 
      break;
      default:
        //for (int k = 0; k < 8; k++) {
        //  leds[k] = CRGB::Black;
        //}
      break;
    }
    
  }
  else if (millis() - bPeriodMillis_f > onTimeOne && doFlagB_f == 1) {  //onTimeOne
    leds[0] = CRGB::Black;
    FastLED.show();
    flashTestCnt = flashTestCnt + 1;
    doFlagB_f = 0;
  }
  else if (millis() - bPeriodMillis_f > onTimeHalfs && doFlagC_f == 1) {  //onTimeHalfs
    for (int k = 1; k < 8; k++) {
      leds[k] = CRGB::Black;
    }
    FastLED.show(); 
    flashTestCnt = flashTestCnt + 1;
    doFlagC_f = 0;
  }
  if (doFlagA_f == 1) {
    FastLED.show(); 
    flashTestCnt = flashTestCnt + 1;
    doFlagA_f = 0;
  }
}

ISR(TIMER2_COMPA_vect) {
  // timer2 interrupt to-do code here
  beatInterruptCnt = beatInterruptCnt + 1;
  OCR2A = 124;        // set compare match register prescaler 256; 125 -> 2ms 0 & 124 = 125
}

void handleISRResult(void) {
  if (beatInterruptCnt >= beatPeriod/4 ) {  // * 4 fuer Achtelnoten (2*2 fuer 2ms und doppelte Anzahl pro beatPeriod)
    beatInterruptCnt = 0;
    beatIndex = beatIndex + 1;
    beatIndexComp = beatIndexComp + 1;
    //bPeriodMillis = millis();
    if (beatIndex > 7) {
      beatIndex = 0;
    }
  }
}

void serialMsg(int msgZeit) {
  static unsigned long msgMillis; 
  if (millis() - msgMillis > msgZeit) {
    msgMillis = millis();
    
    debugSerial.println("Debug serialMsg");
    debugSerial.print("Vergangene Millisekunden seit Programmstart: ");
    debugSerial.println(millis());
    debugSerial.println("----- ----- ----- ----- -----");
    debugSerial.print("bpm = ");
    debugSerial.println(bpm);
    debugSerial.print("bpmMidi = ");
    debugSerial.println(bpmMidi);
    debugSerial.print("beatPeriod = ");
    debugSerial.println(beatPeriod);
    debugSerial.print("beatMidiPeriod = ");
    debugSerial.println(beatMidiPeriod);
    debugSerial.print("beatIndexComp = "); 
    debugSerial.println(beatIndexComp);
    debugSerial.print("beatMidiIndexComp = "); 
    debugSerial.println(beatMidiIndexComp);
    debugSerial.print("enterMenu = ");
    debugSerial.println(enterMenu);
    debugSerial.print("timeDiff Index 0 and 2 = ");
    debugSerial.println(beatMillisTest);
    debugSerial.print("flashTestCnt = ");
    debugSerial.println(flashTestCnt);
    debugFlag = 0;
  }
}

// This function will be called when the button1 was pressed 1 time.
void click1() {
  debugSerial.println("Button 1 click.");
  if (enterMenu == 0) {
    enterMenu = 1;
    menu_ssd1306AsciiPrint();
  }
  else {
    vorZurueckB = 1;
  }
} // click1

// This function will be called when the button1 was pressed 2 times in a short timeframe.
void doubleclick1() {
  debugSerial.println("Button 1 doubleclick.");
  vorZurueckB = -1;
  //lastUsed = millis();
} // doubleclick1

// This function will be called once, when the button1 is pressed for a long time.
void longPressStart1() {
  debugSerial.println("Button 1 longPress start");
} // longPressStart1

// This function will be called often, while the button1 is pressed for a long time.
void longPress1() {
  //Serial.println("Button 1 longPress...");
  //Serial.println(DrehEncoEins.getPosition());
} // longPress1

// This function will be called once, when the button1 is released after beeing pressed for a long time.
void longPressStop1() {
  debugSerial.println("Button 1 longPress stop");
} // longPressStop1

void click2() {
  debugSerial.println("Button 2 click.");
  doTapFlag = 1;
} // click1

void doubleclick2() {
  debugSerial.println("Button 2 doubleclick.");
  beatIndexComp = 0;
  beatMidiIndexComp = 0;
} // doubleclick2

//Menue Funktionen bei Aufruf
void a1action(){ 
  debugSerial.println("A1 action!\n");
}
void a2action(){ 
  debugSerial.println("A2 action!\n");
}
void a3action(){
  //Serial.println("A3 action!\n");
  //tapCnt = 0;
  //if (setValFlag == 0) {
    //setValFlag = 1;
    //ptr_init(&dutyMultiServo, &dutyOnServo, dutyUnitServo);
    //drawVarSet(dutyOnServo);
    //drawVarSet_lcd(dutyOnServo);
    //drawVarSet_ssd1306(dutyOnServo);
  //}
}
void a4action(){ 
  debugSerial.println("A4 action!\n");
  enterMenu = 0;
  refreshFlag = 1;
  clearFlag = 1;
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
    ptr_init(&bpmMulti, &bpm, bpmUnit);
    //drawVarSet(dutyOn_green);
    //drawVarSet_lcd(dutyOn_green);
    //drawVarSet_ssd1306(dutyOn_green);
    drawVarSet_ssd1306Ascii(toggleInterval);
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
  //Serial.println("Debug Menu linked");
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