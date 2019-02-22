#include <vector>
#include <iostream>
#include "Select.hh"


Select::FilterType Select::filter_ = lanczos;

Select::Select(){}

Select::~Select(){}


std::string
Select::select(std::vector<std::string> paramList){


	if(paramList.size()<=2){
		if(paramList.size()==0){
			return "at least one param needs to be provided";
		}
		if(paramList[0]=="gaussian"){
			Select::filter_ = gaussian;
			std::cout<< "Gaussian filter selected"<<std::endl;
		}
		else if(paramList[0] == "lanczos"){
			Select::filter_ = lanczos;
			std::cout<< "Lanczos filter selected"<<std::endl;
		}
		else if(paramList[0] == "triangle"){
			Select::filter_ = lanczos;
			std::cout<< "Triangle filter selected"<<std::endl;
		}
		else if(paramList[0] == "box"){
			Select::filter_ = lanczos;
			std::cout<< "Box filter selected"<<std::endl;
		}
		else if(paramList[0] == "mitchell"){
			Select::filter_ = lanczos;
			std::cout<< "Mitchell filter selected"<<std::endl;
		}
		else{
			std::cout<< "unknown filter. Filter name should be lower-cased"<<std::endl;
		}
		return "Select Done";
	}
	else{
		return  "too many parameters";
	}

}


