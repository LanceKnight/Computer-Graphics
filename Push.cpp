#include <iostream>
#include "3D.h"
#include "Push.hh"
#include "Util.hh"


Push::Push(){}

Push::~Push(){}


std::string
Push::push(std::vector<std::string> paramList){


	if(paramList.size()<1){

		if(top<49){
			matrix_unit * current = new matrix_unit(*stack[top]);
			top++;
			stack[top] = current;
/*
			Util::debug_head("Push.cpp");
				std::cout<<"stack["<<top<<"]:"<<std::endl;
				Util::print_mat(stack[top]);
				std::cout<<"I:"<<std::endl;
				Util::print_mat(&I);
			Util::debug_tail();
			*/
		}
		else{
			return "too many matrics are in the stack";
		}
		return "Push Done";
	}
	else{
		return  "too many parameters";
	}

}


