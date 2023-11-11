#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <iostream>
#include <string>
using namespace std;

class ErrorOccurrence : public exception {
public:
	ErrorOccurrence(const char* msg, int line) : exception(msg),line_(line) {}  

	int getLine()
	{
		return line_;
	}

private:
	int line_;
};

#endif // !EXCEPTIONS_H