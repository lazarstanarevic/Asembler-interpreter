#ifndef MACHINE_H
#define MACHINE_H

#include "Instructions.h"
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

class Machine {
public:
	Machine(); 
	~Machine();

	void loadProgram(const string& filepath);
	void execute(const string& filepath);

private:
	bool empty;	//govori da li je nesto ucitano u Machine
	vector<string> data; //vektor koji sadrzi linije ulaznog fajla 
	vector<Instructions*> inst; //vektor instrukcija
	OperandList* operand_list; //lista operanada koji se pojavljuju u zadatku

	void makeInstructions(); // pravi instrukcije
	void pairing(); // uparuje IF-ELSE, ELSE-ENDIF, LOOP-ENDLOOP
	void process(); // izvrsava instrukciju po instrukciju iz ulazog fajla
	void print(const string& filepath);//ispisuje paramtere operand_list u odgovarajuci fajl
	void clearOut(); // brise podatke ako je nesto vec ucitano,a u toku je novo ucitavanje
	
	//parsira line
	//u string type stavlja naziv instrukcije
	//a vraca vektor operanada koji se koriste u toj instrukciji 
	vector<string> parsing(string& type,const string& line);
};

#endif // !MACHINE_H


