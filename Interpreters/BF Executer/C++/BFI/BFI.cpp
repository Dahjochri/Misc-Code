#include "BFI.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;

#define DEBUG 0

//Con/de structors
BFI::BFI()
{
	#if DEBUG
		cout << "Constructing. Cells=0, Cellnum=1000"<< endl;
	#endif
	p_cells=0;
	p_cellnum = 1000;
}

BFI::BFI(unsigned int cellnum)
{
	#if DEBUG
		cout << "Constructing. Cells=0, Cellnum=" << cellnum << endl;
	#endif
	p_cells=0;
	p_cellnum = cellnum;
}


BFI::~BFI()
{
	#if DEBUG
		cout << "Destructor called" << endl;
	#endif
	if(p_cells!=0)
		delete [] p_cells;
}

//Helper functions
void BFI::setCellNum(unsigned int cellnum)
{
	#if DEBUG
		cout << "Helper setCellNum called with cellnum=" << cellnum << endl;
	#endif
	p_cellnum = cellnum;
}

unsigned int BFI::getCellNum()
{
	#if DEBUG
		cout << "getCellNum called, returning cellnum="<< p_cellnum << endl;
	#endif
	return p_cellnum;
}


//The actual interpreting code
retval_enum BFI::runCode(char *code)
{
	#if DEBUG
		cout << "runCode called with code:"<< endl << "START OF CODE" << endl << code << endl << "END OF CODE"<< endl;
	#endif
	//Returns with enum on failure
	if(p_cellnum==0)
		return INIT_ERROR_VALUES;

	//Allocate memory for cells
	p_cells = new unsigned char[p_cellnum];
	if(p_cells == 0)
			return INIT_ERROR_MEMORY;
	int cell_iter = 0;

	#if DEBUG
		cout << "p_cells allocated with size:" << p_cellnum << endl;
	#endif

	int code_len = strlen(code);
	int code_iter = 0;

	//iter is changed according to code
	while(code_iter < code_len)
	{
		switch(code[code_iter])
		{
			case '+':
				#if DEBUG
					cout << "plus found at:" << code_iter << " incrementing at:" << cell_iter << " giving it a size of:" << (int)(p_cells[cell_iter]+1)<<endl;
				#endif
				//Just increase. If it overflows, it'll become 0
				p_cells[cell_iter]++;
				break;

			case '-':
				#if DEBUG
					cout << "minus found at:" << code_iter << " decrementing at:" << cell_iter << " giving it a size of:" << (int)(p_cells[cell_iter]-1)<< endl;
				#endif
				//"Underflowing" will cause it to be max(int)
				p_cells[cell_iter]--;
				break;

			case '>':
				#if DEBUG
					cout << "> found at:" << code_iter << " moving to:" << cell_iter+1 << endl;
				#endif
				//Move pointer and check boundaries
				cell_iter++;
				if(cell_iter == p_cellnum)
					return RUN_ERROR_BOUNDARIES;
				break;

			case '<':
				#if DEBUG
					cout << "< found at:" << code_iter << " moving to:" << cell_iter-1 << endl;
				#endif
				//Move pointer and check boundaries
				cell_iter--;
				if(cell_iter<0)
					return RUN_ERROR_BOUNDARIES;
				break;

			case '.':
				#if DEBUG
					cout << ". found at:" << code_iter << " cell:" << cell_iter << " will be output"<< endl;
				#endif
				//Output current cell
				cout << p_cells[cell_iter];
				break;

			case ',':
				#if DEBUG
					cout << ", found at:" << code_iter << " expecting input to be made and found at:" << cell_iter << endl;
				#endif
				//Take a char and put it in current cell
				p_cells[cell_iter] = getc(stdin);
				break;

			case '[':
				#if DEBUG
					cout << "[ found at:" << code_iter << " cell:" << cell_iter << " is:" << (int)(p_cells[cell_iter]) << endl;
				#endif
				//If current cell is 0, run to the next ']'
				if(p_cells[cell_iter]==0)
					while(code[code_iter]!=']')
					{
						code_iter++;
						if(code_iter>=code_len)
							return ENDLOOP_MISSING;
					}
				break;

			case ']':
				#if DEBUG
					cout << "] found at:" << code_iter << " Moving back " << endl;
				#endif
				//Run all the way back before the last '['
				while(code[code_iter]!='[')
				{
					code_iter--;
					if(code_iter<0)
						return STARTLOOP_MISSING;
				}
				code_iter--;
				break;

			default:
				//It's not a relevant char, ignore it.
				break;
		};
		code_iter++;
	};

	#if DEBUG
		cout << "Done with code. Cleaning up." << endl;
	#endif
	//Just clean up
	delete [] p_cells;
	p_cells=0;


	#if DEBUG
		cout << "Done cleaning up. returning FINISHED." << endl;
	#endif
	//We are done. Give an awesome goodness signal back!
	return FINISHED;
}

