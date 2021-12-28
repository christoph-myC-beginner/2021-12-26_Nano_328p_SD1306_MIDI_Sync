//DrehEnco.h
//erster Versuch objektorientiert zu Programmieren
//Christoph S. 2020-12-05

#ifndef DrehEnco_h
#define DrehEnco_h


class DrehEnco {
  private:
    byte PinSpurA;
    byte PinSpurB;
    int pos;
    bool dir;
    bool Aa;
    bool Ba;
	
	public:
	//constructor
	DrehEnco(byte PinA, byte PinB);
		
	//Uebergabe Position
	int getPosition();

        //Uebergabe eines Steps mit Entprellzeit
        int getStep();
	
	//Uebergabe eines viertel Steps ohne Entprellzeit entsprechend Mechanik KY-40
        int get4Step();
		
	//Uebergabe Richtung
	bool getDirection();
		
	//Abfrage-Function, muss in passender Geschwindigkeit abgefragt werden (Interrupt)
        void check(void);		
};

#endif
