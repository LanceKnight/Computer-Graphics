#ifndef UTIL_HH
#define UTIL_HH

#include <string>
#include <vector>
#include "3D.h"


class Util{

public:
	Util();

	~Util();

	static bool isLittleEndian();
	
	static void reverse(unsigned char * bytes, int n);

	static void print_mat(matrix_unit * matrix);

	static void debug_head(std::string filename);

	static void debug_tail();

};


#endif
