#include <iostream>
#include "Matrix.hh"
#include "Push.hh"


Push::Push(){}

Push::~Push(){}


std::string
Push::push(std::vector<std::string> paramList){


	if(paramList.size()<1){


		return "Push Done";
	}
	else{
		return  "too many parameters";
	}

}


