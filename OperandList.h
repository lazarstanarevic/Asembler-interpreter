#ifndef OPERANDLIST_H
#define OPERANDLIST_H

#include "Operand.h"

class OperandList {
public:
	~OperandList();

	//vraca indeks operanda koji ima naziv name, ako ne postoji takav vraca -1
	int find(const string& name);

	//vraca vrednost operanda koji ima naziv name
	int getValue(const string& name);

	//updatejuje vrednost operanda koji ima naziv name na val,
	//ukoliko ne postoji takav dodaje u listu novi operand sa parametrima name i val
	void update(const string& name, const int val);

	//sortira listu operanada po imenu rastuce
	void sort();

	//ispisuje listu u odgovarajuci fajl
	void print(const string& filepath);

protected:
	vector <Operand*> operands_;
};

#endif // !OPERANDLIST_H

