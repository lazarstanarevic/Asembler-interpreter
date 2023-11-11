#include "Operand.h"

Operand::Operand(const string& name, int val) : name_(name), val_(val) {}

Operand::~Operand()
{
}

void Operand::updateVal(const int val)
{
	val_ = val;
}

string Operand::getName()
{
	return name_;
}

int Operand::getVal()
{
	return val_;
}