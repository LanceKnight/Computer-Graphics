#include <iostream>
#include "3D.h"
#include "Perspective.hh"


Perspective::Perspective(){}

Perspective::~Perspective(){}


std::string
Perspective::perspective(std::vector<std::string> paramList){


	if(paramList.size()<=4){
		if(paramList.size() <4){
			return "too few parameters";
		}
		float f = 0;
		float a = 0;
		float n = 0;

		try{
			x = std::stod(paramList[0]);
			y = std::stod(paramList[1]);
			z = std::stod(paramList[2]);

		}
		catch(...){
			return "illegal params";

		}
		top++;
		std::cout<<"Perspective.c:"<<"top:"<<top<<std::endl;
		if(top<50){
			stack[top] = &current;
		}
		else{
			return "too many matrics are in the stack";
		}
		return "Perspective Done";
	}
	else{
		return  "too many parameters";
	}

}
