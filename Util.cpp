#include "Util.hh"


Util::Util(){}

Util::~Util(){}


bool
isLittleEndian(){
	short int one =1;
	char * byte = (char*) & one;
	return (byte[0]?1:0);


}


