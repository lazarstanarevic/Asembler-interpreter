#ifndef OPERAND_H
#define OPERAND_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Operand {
public:
	Operand(const string& name,const int val);
	~Operand();

	string getName();
	int getVal();

	void updateVal(const int val);

private:
	string name_;
	int val_;
}; 

#endif // !OPERAND_H

