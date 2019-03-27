#include <iostream>
#include "3D.h"
#include "Pop.hh"
#include "Util.hh"


Pop::Pop(){}

Pop::~Pop(){}


std::string
Pop::pop(std::vector<std::string> paramList){


	if(paramList.size()<1){

		if(top>0){
			delete stack[top];
			top--;
			/*
			Util::debug_head("Pop.cpp");
				std::cout<<"stack["<<top<<"]:"<<std::endl;
				Util::print_mat(stack[top]);
				std::cout<<"I:"<<std::endl;
				Util::print_mat(&I);
			Util::debug_tail();
			*/
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


