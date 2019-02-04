#ifndef UTIL_HH
#define UTIL_HH

#include <string>
#include <vector>



class Util{

public:
	Util();

	~Util();

	static bool isLittleEndian();
	


	static void reverse(unsigned char * bytes, int n);

};


#endif