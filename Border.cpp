#include <vector>
#include <iostream>
#include "Border.hh"


Border::BorderType Border::border_ = Border::zero;


Border::Border(){}

Border::~Border(){}


std::string
Border::border(std::vector<std::string> paramList){


	if(paramList.size()<=1){
		if(paramList.size()<1){
			return "you need to provide a param";
		}
		std::string param = paramList[0];
		if(param == "zero"){
			Border::border_ = Border::zero;

		}
		else if(param == "freeze"){
			Border::border_ = Border::freeze;

		}
		else if(param == "circular"){
			Border::border_ = Border::circular;

		}
		return "Border done";
	}
	else{
		return  "too many parameters";
	}

}


