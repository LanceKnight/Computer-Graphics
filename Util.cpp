#include <cstring>
#include <iomanip>
#include <iostream>
#include <utility>
#include "Util.hh"


Util::Util(){}

Util::~Util(){}


bool
Util::isLittleEndian(){
	short int one =1;
	char * byte = (char*) & one;
	return (byte[0]?1:0);


}

//reverse the order of elements in an array. n is the actual number of elements, excluding the ending null.
void 
Util::reverse(unsigned char * bytes, int n){

/*	#ifdef DEBUG
		std::cout<<"=====DEBUG  INFO====="<<std::endl;
		std::cout<<"old array 0x";
		for (int i = 0; i< n;i++){
			std::cout<<std::setfill('0')<<std::setw(2)<<std::hex<<(int) bytes[i] <<" ";
		}
		std::cout<<std::endl;
		std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
	#endif*/
		for(int i = 0; i<n/2; i++){
			std::swap(bytes[i], bytes[n-i-1]);
		}	
/*	#ifdef DEBUG

		std::cout<<"=====DEBUG  INFO====="<<std::endl;
		std::cout<<"new array 0x";
		for (int i = 0; i< n;i++){
			std::cout<<std::setfill('0')<<std::setw(2)<<std::hex<<(int) bytes[i] <<" ";
		}
		std::cout<<std::endl;
		std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
	#endif*/

}

