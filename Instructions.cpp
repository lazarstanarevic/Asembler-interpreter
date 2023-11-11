#include "Instructions.h"

#include<iostream>
#include<string>
using namespace std;

Instructions::Instructions(const int line, const Type type, const vector<string>& operands) : line_(line),type_(type),operands_(operands) {}

Instructions::~Instructions()
{
}

bool Instructions::checkConst(const string& operand)
{
	for (int i = 0;i < operand.size();i++)
	{
		if (operand[i] == '-' && i == 0) continue;

		if (!(operand[i] >= '0' && operand[i] <= '9')) return false;
	}

	return true;
}

bool Instructions::checkString(const string& operand)
{
	if (operand.size() != 1) return false;

	for (int i = 0;i < operand.size();i++)
	{
		if (!(operand[i] <= 'Z' && operand[i] >= 'A')) return false;
	}

	return true;
} 

bool Instructions::checkBothOptions(const string& operand)
{
	if (checkConst(operand) || checkString(operand)) return true;
	else return false;
}

Type Instructions::getType()
{
	return type_;
}

int Instructions::getLine()
{
	return line_;
}

int Instructions::getLoopCnt()
{
	return loop_cnt_;
}

int Instructions::getJumpLine()
{
	return jump_line_;
}

bool Instructions::getCondition()
{
	return condition_;
}

int Instructions::getLoopLimit()
{
	return loop_limit_;
}

void Instructions::updateJumpLine(int jump_line)
{
	jump_line_ = jump_line;
}

void Instructions::updateCondition(bool condition)
{
	condition_ = condition;
}

void Instructions::updateLoopCnt(int loop_cnt)
{
	loop_cnt_ = loop_cnt;
}

void Instructions::updateLoopLimit(int loop_limit)
{
	loop_limit_ = loop_limit;
}

//----------------------------------------------------------------------------------------------------------------

Set::Set(const int line, const Type type, const vector<string>& operands):Instructions(line,type,operands){}

Set::~Set()
{
}

int Set::work(const vector<Instructions*>& inst, OperandList* operand_list)
{
	if(operands_.size()!=2) throw new ErrorOccurrence("Ivnalid number of operands in line ", this->line_);

	int new_val;

	string operand0 = operands_[0], operand1 = operands_[1];

	if (!(checkString(operand0) && checkBothOptions(operand1))) 
	{
		throw new ErrorOccurrence("Bad declaration in line ", this->line_);
	}

	//dobija brojne vrenosti operand1 i njeno smestanje u new_val
	if (operand1[0] <= 'Z' && operand1[0] >= 'A')
	{
		if (operand_list->find(operand1) == -1)
		{
			throw new ErrorOccurrence("Uninitialized operand in line ", this->line_);
		}

		new_val = operand_list->getValue(operand1);
	}
	else new_val = stoi(operand1);

	//setovanje operand0 na vrednost new_val
	operand_list->update(operand0, new_val);

	return line_ + 1;
}


Aritmetic::Aritmetic(const int line, const Type type, const vector<string>& operands) : Instructions(line, type, operands){}

Aritmetic::~Aritmetic()
{
}

int Aritmetic::work(const vector<Instructions*>& inst, OperandList* operand_list)
{
	if (operands_.size() != 3) throw new ErrorOccurrence("Ivnalid number of operands in line ", this->line_);
	
	if (!checkString(operands_[0]))
	{
		throw new ErrorOccurrence("Bad declaration in line ", this->line_);
	}

	vector<int> val;

	//dobijanje brojnih vrenosti operanda1 i operanda2 i njihovo smestanje u vektor val
	for (int i = 1;i < operands_.size();i++)
	{
		string tmp = operands_[i];
		int tmp_val;

		if (!checkBothOptions(tmp))
		{
			throw new ErrorOccurrence("Bad declaration in line ", this->line_);
		}

		if (tmp[0] <= 'Z' && tmp[0] >= 'A')
		{
			if (operand_list->find(tmp) == -1)
			{
				throw new ErrorOccurrence("Uninitialized operand in line ", this->line_);
			}

			tmp_val = operand_list->getValue(tmp);
		}
		else tmp_val = stoi(tmp);

		val.push_back(tmp_val);
	}

	int new_val = 0;

	// u zavisnosti od tipa aritmeticke operacije vrsi se odgovarajuca obrada
	// rezultat se smesta u new_val
	if (type_ == ADD) new_val = val[0] + val[1];

	if (type_ == SUB) new_val = val[0] - val[1];

	if (type_ == MUL) new_val = val[0] * val[1];

	if (type_ == DIV)
	{
		if (val[1] == 0) throw new ErrorOccurrence("Divizion by zero in line ", this->line_);

		new_val = val[0] / val[1];
	}

	//setovanje operanda0 na new_val
	operand_list->update(operands_[0], new_val);


	return line_ + 1;
}


GoTo::GoTo(const int line, const Type type, const vector<string>& operands) : Instructions(line, type, operands){}

GoTo::~GoTo()
{
}

int GoTo::work(const vector<Instructions*>& inst, OperandList* operand_list)
{	
	if (operands_.size() != 1) throw new ErrorOccurrence("Ivnalid number of operands in line ", this->line_);

	if (!checkConst(operands_[0])) throw new ErrorOccurrence("Bad declaration in line ", this->line_);
	
	if (stoi(operands_[0]) == 0) throw new ErrorOccurrence("GOTO argument cannot be zero in line ", this->line_);

	//vraca sledecu liniju za operand0 mesta udaljenju od trenutne
	return line_ + stoi(operands_[0]);
}


If::If(const int line, const Type type, const vector<string>& operands) : Instructions(line, type, operands) {}

If::~If()
{
}

int If::work(const vector<Instructions*>& inst, OperandList* operand_list)
{
	if (operands_.size() != 2) throw new ErrorOccurrence("Ivnalid number of operands in line ", this->line_);

	vector<int> vals;

	//dohvatanje vrednosti operanda0 i operanda1 i njohovo smesta u vektor vals
	for (int i = 0;i < operands_.size();i++)
	{
		string tmp = operands_[i];
		int val;

		if (!checkBothOptions(tmp)) throw new ErrorOccurrence("Bad declaration in line ", this->line_);

		if (tmp[0] <= 'Z' && tmp[0] >= 'A')
		{
			if (operand_list->find(tmp) == -1)
			{
				throw new ErrorOccurrence("Uninitialized operand in line ", this->line_);
			}
			
			val = operand_list->getValue(tmp);
		}
		else val = stoi(tmp);

		vals.push_back(val);
	}


	bool cond = false;

	// ako je uslov ispunjen setuje se condition na true
	if (type_ == IFGR && vals[0] > vals[1]) cond = true;

	else if (type_ == IFEQ && vals[0] == vals[1]) cond = true;

	//ako je ispunjen cond updateujemo condition_ na odgovarajucoj ELSE instrukciji na true i prelazimo u sledecu liniju
	//ako nije ipusnjen updateujemo condition_ na odgovarajucoj ELSE instrukciji na false i prelazimo ELSE liniju
	if (cond)
	{
		inst[this->jump_line_]->updateCondition(true);
			
		return line_ + 1;
	}
	else
	{
		inst[this->jump_line_]->updateCondition(false);

		return jump_line_;
	}

}


Else::Else(const int line, const Type type, const vector<string>& operands) : Instructions(line, type, operands)
{
	//pre bilo cega setujemo condition_ u ELSE na false
	//zato sto ne zelimo da ako nam neki GOTO prekoci odgovarajuci IF condition_ bude true
	condition_ = false;
}

Else::~Else()
{
}

int Else::work(const vector<Instructions*>& inst, OperandList* operand_list)
{	
	if (operands_.size() != 0) throw new ErrorOccurrence("Ivnalid number of operands in line ", this->line_);

	//ako je uslov iz IF ispunjen setujemo condition_ na false iz istog razloga kao u konstuktoru
	//i potom skacemo na ENDIF preskacuci ELSE-ENDIF blok
	if (this->condition_)
	{
		this->condition_ = false;

		return jump_line_;
	}
	//ako uslov iz IF nije ispunjen prelazimo na sledecu liniju ulazeci u ELSE-ENDIF blok 
	else return line_ + 1;
}


EndIf::EndIf(const int line, const Type type, const vector<string>& operands) : Instructions(line, type, operands) {}

EndIf::~EndIf()
{
}

int EndIf::work(const vector<Instructions*>& inst, OperandList* operand_list)
{
	if (operands_.size() != 0) throw new ErrorOccurrence("Ivnalid number of operands in line ", this->line_);

	//Instrukcija ne radi nista, samo prelazi u sledecu liniju
	return line_ + 1;
}


Loop::Loop(const int line, const Type type, const vector<string>& operands) : Instructions(line, type, operands) 
{
	//uslov sa setovanje loop_cnt_ na odgovorajucem ENDLOOPu
	condition_ = true;
}

Loop::~Loop()
{
}

int Loop::work(const vector<Instructions*>& inst, OperandList* operand_list)
{	
	if (operands_.size() >= 2) throw new ErrorOccurrence("Ivnalid number of operands in line ", this->line_);

	if (operands_.size() == 0) loop_limit_ = -1;
	else
	{
		loop_limit_ = stoi(operands_[0]);
		
		if (loop_limit_ <= 0) throw new ErrorOccurrence("LOOP argument must be positive in line ", this->line_);
	}

	//ako je uslov ispunjen resetuje se loop_cnt_ na 0 u odogovarajucem ENDLOOPu
	
	//u suprotnom - vracanje uslova na true jer on uvek treba da bude true sem u slucaju kad se 
	//u LOOP dolazi iz ENDLOOPa jer tad ne zelimo da resetujemo loop_cnt_
	if (condition_ == true) inst[jump_line_]->updateLoopCnt(0);
	 
	else if (condition_ == false) condition_ = true;

	return line_ + 1;
}


EndLoop::EndLoop(const int line, const Type type, const vector<string>& operands) : Instructions(line, type, operands) 
{
	//ako kojim slucajem neki GOTO preskoci LOOP na pocetku zelimo da pocetna vrednost loop_cnt_ bude 0
	loop_cnt_ = 0;
}

EndLoop::~EndLoop()
{
}

int EndLoop::work(const vector<Instructions*>& inst, OperandList* operand_list)
{
	if (operands_.size() != 0) throw new ErrorOccurrence("Ivnalid number of operands in line ", this->line_);
	

	loop_limit_ = inst[jump_line_]->getLoopLimit();

	//ako je LOOP bez operanda vrati se na njega	
	if (loop_limit_ == -1) return jump_line_;

	//ako smo presli limit obilaska petlje predji na sledecu liniju u fajlu
	//u suprotnom uvecaj loop_cnt_, uslov za resetovanje postave na false u LOOPu i vrati se na njega
	if (loop_limit_ <= (loop_cnt_ + 1)) return line_ + 1;
	else
	{
		loop_cnt_++;

		inst[jump_line_]->updateCondition(false);

		return jump_line_;
	}
}