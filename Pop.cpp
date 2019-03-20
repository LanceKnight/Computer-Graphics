#include <iostream>
#include "3D.h"
#include "POP.hh"


Pop::Pop(){}

Pop::~Pop(){}


std::string
Pop::pop(std::vector<std::string> paramList){


	if(paramList.size()<1){

		top++;
		std::cout<<"Pop.c:"<<"top:"<<top<<std::endl;
		if(top>1){
			stack[top] = &current;
		}
		else{
			return "at least one identity matrix should be remained in the stack";
		}
		return "Pop Done";
	}
	else{
		return  "too many parameters";
	}

}


