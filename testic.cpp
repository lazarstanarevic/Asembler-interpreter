#include "Machine.h"
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

int tests_public_passed = 0;

// Funkcija koja poredi Vas izlaz sa ocekivanim izlazom za odgovarajuci test primer
void checkOutput(const string& student_output_filename, int test_number) { 

	ifstream correct_output("correct_output_" + to_string(test_number) + ".txt");
	stringstream ss_correct;
	ss_correct << correct_output.rdbuf(); correct_output.close();
	string string_correct = ss_correct.str();

	ifstream student_output(student_output_filename);
	stringstream ss_student;
	ss_student << student_output.rdbuf(); student_output.close();
	string string_student = ss_student.str();

	if (string_student != "" && string_correct == string_student) {
		cout << "Test " + to_string(test_number) + ": PASSED" << endl;
		tests_public_passed++;
	}
	else {
		cout << "Test " + to_string(test_number) + ": FAILED" << endl;
	}

}

void test(int test_number) {

	Machine* machine = new Machine;

	machine->loadProgram("input_" + to_string(test_number) + ".txt");

	machine->execute("student_log_" + to_string(test_number) + ".txt");

	checkOutput("student_log_" + to_string(test_number) + ".txt", test_number);

	delete machine;
}

int main() {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Izmeniti elemente niza u zavisnosti od testova koje zelite da pokrenete!
	// Svi testovi testiraju ucitavanje i izvrsavanje programa
	// Test 1 - Testira izvrsavanje naredbe SET
	// Test 2 - Testira izvrsavanje aritmetickih naredbi
	// Test 3 - Testira izvrsavanje naredbi uslovnog grananja
	// Test 4 - Testira izvrsavanje naredbe beskonacne petlje
	// Test 5 - Testira izvrsavanje naredbe uslovne petlje
	int active_tests[] = { 1,2,3,4,5,6,7,8,9,10 }; 
	int number_of_tests = sizeof(active_tests) / sizeof(int);

	cout << "============================================" << endl;
	cout << "TESTING FILE_SYSTEM" << endl << endl;
	cout << "Executing tests: ";
	for (int i = 0; i < number_of_tests; i++) cout << active_tests[i] << (i < number_of_tests - 1 ? ", " : "");
	cout << endl;

	for (int i = 0; i < number_of_tests; i++) test(active_tests[i]);

	cout << "============================================" << endl;
	cout << "Public tests passed count: " << tests_public_passed << endl; 

	return 0;
} 