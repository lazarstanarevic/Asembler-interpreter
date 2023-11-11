#include "OperandList.h"
#include <fstream>

OperandList::~OperandList()
{
	for (int i = 0;i < operands_.size();i++) delete operands_[i];
}

int OperandList::find(const string& name)
{

	for (int i = 0;i < operands_.size();i++)
	{
		if (operands_[i]->getName() == name) return i;
	}

	return -1;
}

int OperandList::getValue(const string& name)
{
	int pos = this->find(name);

	return operands_[pos]->getVal();
}

void OperandList::update(const string& name, const int val)
{
	int pos = this->find(name);

	if (pos == -1) operands_.push_back(new Operand(name, val));

	else operands_[pos]->updateVal(val);
}

void OperandList::sort()
{
	for (int i = 0;i < operands_.size();i++)
	{
		for (int j = i + 1;j < operands_.size();j++)
		{
			if (operands_[i]->getName() > operands_[j]->getName())
			{
				Operand* pom = operands_[i];
				operands_[i] = operands_[j];
				operands_[j] = pom;
			}
		}
	}
}

void OperandList::print(const string& filepath)
{
	ofstream output_file(filepath);

	for (int i = 0;i < operands_.size();i++)
	{
		output_file << operands_[i]->getName() << "=" << operands_[i]->getVal();

		//cout << operands_[i]->getName() << "=" << operands_[i]->getVal() << endl; 

		if (i != (operands_.size() - 1)) output_file << endl;
	}

	output_file.close();
}
