#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Exceptions.h"
#include "OperandList.h"
#include<iostream>
#include<string>
#include<vector>
using namespace std;

enum Type { SET, ADD, SUB, MUL, DIV, GOTO, IFGR, IFEQ, ELSE, ENDIF, LOOP, ENDLOOP };

class Instructions {
public:
	Instructions(const int line, const Type type, const vector<string>& operands);
	virtual ~Instructions();

	//izvrsava obradu instrukcije i vraca liniju na kojoj se nalazi sledeca komanda
	virtual int work(const vector<Instructions*>& inst, OperandList* operand_list) = 0;

	Type getType();
	int getLine();
	int getLoopCnt();
	int getJumpLine();
	bool getCondition();
	int getLoopLimit();

	void updateJumpLine(int jump_line);
	void updateCondition(bool condition);
	void updateLoopCnt(int loop_cnt);
	void updateLoopLimit(int loop_limit);

protected:
	const int line_; // redni broj linije u fajlu gde se nalazi instrukcija (0,1,2...)
	const Type type_; // vrsta operacije
	const vector<string> operands_; // vektor operanada koju ucestvuju u instukciji

	// Odnosi se na instrukcije tipa IF,ELSE,LOOP,ENDLOOP 
	// to je ona linija na kojoj se nalazi instrukcija sa kojom su uparene
	int jump_line_;

	//brojac koliko puta smo obisli LOOP
	int loop_cnt_;
	//ako je LOOP bez parametara ima vrenost -1, u suprtnom je granica vrtenja u LOOPu
	int loop_limit_;
	
	//Odnosi se na instrukcije ELSE i LOOP
	//za ELSE govori da li je odgovarajuci uslov u IF ispunjen
	//za LOOP govori da li treba resetovati loop_cnt_ 
	bool condition_;

	bool checkConst(const string& operand);
	//provera da li je operand string duzine 1  (A,B,C...Z)
	bool checkString(const string& operand);
	//provera da li je operand konstanta ili string
	bool checkBothOptions(const string& operand);
};

class Set : public Instructions {
public:
	Set(const int line, const Type type, const vector<string>& operands);
	virtual ~Set();
	
	virtual int work(const vector<Instructions*>& inst, OperandList* operand_list) override;
};

class Aritmetic : public Instructions {
public:
	Aritmetic(const int line, const Type type, const vector<string>& operands);
	virtual ~Aritmetic();
	
	virtual int work(const vector<Instructions*>& inst, OperandList* operand_list) override;
};

class GoTo :public Instructions {
public:
	GoTo(const int line, const Type type, const vector<string>& operands);
	virtual ~GoTo();

	virtual int work(const vector<Instructions*>& inst, OperandList* operand_list) override;
};

class If : public Instructions {
public:
	If(const int line, const Type type, const vector<string>& operands);
	virtual ~If();
	
	virtual int work(const vector<Instructions*>& inst, OperandList* operand_list) override;
};

class Else : public Instructions {
public:
	Else(const int line, const Type type, const vector<string>& operands);
	virtual ~Else();

	virtual int work(const vector<Instructions*>& inst, OperandList* operand_list) override;
};

class EndIf : public Instructions {
public:
	EndIf(const int line, const Type type, const vector<string>& operands);
	
	virtual ~EndIf();
	virtual int work(const vector<Instructions*>& inst, OperandList* operand_list) override;
};

class Loop : public Instructions {
public:
	Loop(const int line, const Type type, const vector<string>& operands);
	virtual ~Loop();

	virtual int work(const vector<Instructions*>& inst, OperandList* operand_list) override;
};

class EndLoop : public Instructions {
public:
	EndLoop(const int line, const Type type, const vector<string>& operands);
	virtual ~EndLoop();

	virtual int work(const vector<Instructions*>& inst, OperandList* operand_list) override;
};

#endif