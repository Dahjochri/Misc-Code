#ifndef BFI_H
#define BFI_H

enum retval_enum{INIT_ERROR_VALUES, INIT_ERROR_MEMORY, RUN_ERROR_BOUNDARIES, ENDLOOP_MISSING, STARTLOOP_MISSING, FINISHED};

class BFI
{
	public:
		//Defaults to 1000 cells and 1 byte size. Everything is unsigned.
		BFI();
		BFI(unsigned int cellnum);
		BFI(unsigned int cellnum, unsigned int cellsize);
		~BFI();
		void setCellNum(unsigned int cellnum);
		unsigned int getCellNum();

		retval_enum runCode(char *code);

	private:
		unsigned int p_cellnum;
		unsigned char *p_cells;
};

#endif
