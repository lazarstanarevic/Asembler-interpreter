#include "Machine.h"

Machine::Machine() : empty(true)
{
	this->operand_list = new OperandList();
}

Machine::~Machine()
{
	delete operand_list;
	
	for (int i = 0;i < inst.size();i++) delete inst[i];
}

void Machine::loadProgram(const string& filepath)
{	
	if (this->empty == false) this->clearOut();

	ifstream input_file(filepath);

	string file_line;

	while (getline(input_file, file_line))
	{
		data.push_back(file_line);
	}

	input_file.close();

	this->empty = false;
}

void Machine::execute(const string& filepath)
{
	try 
	{
		if (this->empty) return;

		this->makeInstructions();

		this->pairing();

		this->process();

		this->print(filepath);
	}
	catch (ErrorOccurrence* e)
	{
		cout << e->what() << e->getLine() << "." << endl;

		delete e;
	}
}

void Machine::makeInstructions()
{

	Instructions* new_inst;

	string type;

	vector<string> operands;

	//ide liniju po liniju iz ulaznog fajla obradjuje je, pravi instukciju i na kraju se doda u vektor inst
	for (int i = 0;i < data.size();i++)
	{
		operands = this->parsing(type, data[i]);

		if (type == "SET")  new_inst = new Set(i, SET, operands);

		else if (type == "ADD")  new_inst = new Aritmetic(i, ADD, operands);

		else if (type == "MUL")  new_inst = new Aritmetic(i, MUL, operands);

		else if (type == "DIV")  new_inst = new Aritmetic(i, DIV, operands);

		else if (type == "SUB")  new_inst = new Aritmetic(i, SUB, operands);

		else if (type == "GOTO")  new_inst = new GoTo(i, GOTO, operands);

		else if (type == "IFEQ")  new_inst = new If(i, IFEQ, operands);

		else if (type == "IFGR")  new_inst = new If(i, IFGR, operands);

		else if (type == "ELSE")  new_inst = new Else(i, ELSE, operands);

		else if (type == "LOOP")  new_inst = new Loop(i, LOOP, operands);

		else if (type == "ENDIF") new_inst = new EndIf(i, ENDIF, operands);

		else if (type == "ENDLOOP") new_inst = new EndLoop(i, ENDLOOP, operands);

		else throw new ErrorOccurrence("Wrong instruction name in line ", i);

		inst.push_back(new_inst);
	}
}

//koristi stack foru za upraivnje(slicno kao sto se stekom uparuju zagrade)
void Machine::pairing()
{
	stack<Instructions*> if_stack;
	stack<Instructions*> loop_stack;
	stack<Instructions*> else_stack;

	for (int i = 0;i < this->inst.size();i++)
	{
		Instructions* inst1 = inst[i], *inst2;

		if (inst1->getType() == IFGR || inst1->getType() == IFEQ) if_stack.push(inst1);

		if (inst1->getType() == LOOP) loop_stack.push(inst1);

		if (inst1->getType() == ELSE)
		{
			else_stack.push(inst1);

			if (if_stack.empty()) throw new ErrorOccurrence("Instruction is unpaired at line ", inst1->getLine());
			
			inst2 = if_stack.top();	if_stack.pop();

			inst2->updateJumpLine(inst1->getLine());
		}

		if (inst1->getType() == ENDLOOP)
		{
			if (loop_stack.empty()) throw new ErrorOccurrence("Instruction is unpaired at line ", inst1->getLine());

			inst2 = loop_stack.top();	loop_stack.pop();

			inst2->updateJumpLine(inst1->getLine());

			inst1->updateJumpLine(inst2->getLine());
		}

		if (inst1->getType() == ENDIF)
		{
			if (else_stack.empty()) throw new ErrorOccurrence("Instruction is unpaired at line ", inst1->getLine());

			inst2 = else_stack.top();	else_stack.pop();

			inst2->updateJumpLine(inst1->getLine());

		}
	}

	if (!(if_stack.empty() && else_stack.empty() && loop_stack.empty()))
	{	
		int line;

		if (!if_stack.empty()) line = if_stack.top()->getLine();

		if (!else_stack.empty()) line = else_stack.top()->getLine();
		
		if (!loop_stack.empty()) line = loop_stack.top()->getLine();

		throw new ErrorOccurrence("Instruction is unpaired at line ", line);
	}
}

void Machine::process()
{
	int line = 0;

	while (line < inst.size())
	{	
		line = inst[line]->work(inst, operand_list);
	}
}

void Machine::print(const string& filepath)
{
	this->operand_list->sort();

	this->operand_list->print(filepath);
}

void Machine::clearOut()
{
	delete operand_list;

	for (int i = 0;i < inst.size();i++) delete inst[i];

	inst.clear();

	data.clear();

	this->empty = true;

	operand_list = new OperandList();
}

vector<string> Machine::parsing(string& type,const string& line)
{
	vector<string> ret;

	string tmp = "";

	int cnt = 0;

	for (int i = 0;i < line.length();i++)
	{
		if (line[i] == ' ' || i==(line.length()-1))
		{   
			cnt++;

			if (i == (line.length() - 1)) tmp += line[i];

			if (cnt == 1) type = tmp;
			else ret.push_back(tmp);

			tmp = "";
		}

		else tmp += line[i];
	}

	return ret;
}