#include "BFI.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
	cout << "making bfi" << endl;
	BFI *test = new BFI;
	cout << "Running code" << endl << endl;
	unsigned int stat = test->runCode(",----------[++++++++++>,----------]<[.<]+++++++++++++.---.");
	cout << endl << endl <<"Done running code" << endl;
	switch(stat)
	{
		case INIT_ERROR_VALUES:
			cout << "Error initializing values." << endl;
			//Some jerk set the number of cells to 0
			break;

		case INIT_ERROR_MEMORY:
			cout << "Not enough memory." << endl;
			//Not enough memory to allocate an array of chars with length cellnum
			break;

		case RUN_ERROR_BOUNDARIES:
			cout << "Boundary error in BF prog" << endl;
			//Whilst running, the cell iterator got below 0 or over cellnum. ie. not enough cells
			break;

		case STARTLOOP_MISSING:
			cout << "Error in code. Start loop missing" << endl;
			//An endloop was found without a startloop
			break;

		case ENDLOOP_MISSING:
			cout << "Error in code. End loop missing" << endl;
			//A Startloop was found without an endloop
			break;

		case FINISHED:
			cout << "Finished with no problems" << endl;
			//Code finished without any probs
			break;
	};

	delete test;
	return 0;
}
