//DrehEnco.cpp
//erster Versuch objektorientiert zu Programmieren
//Christoph S. 2020-12-05
#include "arduino.h"
#include "DrehEnco.h"

DrehEnco::DrehEnco(byte PinA, byte PinB) {
  PinSpurA = PinA;
  PinSpurB = PinB;
  pinMode(PinSpurA, INPUT_PULLUP);
  pinMode(PinSpurB, INPUT_PULLUP);
}

int DrehEnco::getPosition() {
  return pos;
}

int DrehEnco::getStep() {
  static unsigned long previousMillis;
  static int previousValue;
  if (millis() - previousMillis > 100) {
    previousMillis = millis();
    int zwi = pos-previousValue;
    previousValue = pos;
    if (zwi > 1  ) {
      return 1;
    }
    else if (zwi < -1 ) {
      return (-1);
    }
    else {
      return (0);
    }
  }
  else {
    return (0);
  }
}

int DrehEnco::get4Step() {
  static int previousValue;
  int encoStep = pos/4 - previousValue;
  previousValue = pos/4;
  return (encoStep);
}

bool DrehEnco::getDirection() {
  return dir;
}

void DrehEnco::check(void)
{
  
  bool A = digitalRead(PinSpurA);
	bool B = digitalRead(PinSpurB);
  bool Aa = 0;
  bool Ba = 0;
	
	if (A != Aa) { //also A hat sich gegenüber vorherigem Check veraendert
    if (A != B) {     //Drehung positiv
      dir = 1;
      pos = pos+1;           
      }
    else { // Pin2, A gleich Pin3, B; Drehung negativ
      dir = 0;
      pos = pos-1;
      }
  }
  else { 
  }
  if (B != Ba) {  // also B hat sich verändert
    if (B == A) { //Drehung positiv
      dir = 1;
      pos = pos+1;
      }
    else { // B != A; Drehung negativ
      dir = 0;
      pos = pos-1; 
      }
  }
  else {
  }
  
  Aa=A;
  Ba=B; //Zustand in Zustand alt schreiben
}
