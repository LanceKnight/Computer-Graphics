#include <vector>
#include <iostream>
#include "Select.hh"


Select::FilterType Select::filter_ = lanczos;

float Select::alpha_ = 2;

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
			Select::filter_ = triangle;
			std::cout<< "Triangle filter selected"<<std::endl;
		}
		else if(paramList[0] == "box"){
			Select::filter_ = box;
			std::cout<< "Box filter selected"<<std::endl;
		}
		else if(paramList[0] == "mitchell"){
			Select::filter_ = mitchell;
			std::cout<< "Mitchell filter selected"<<std::endl;
		}
		else if(paramList[0] == "hemming"){
			Select::filter_ = hemming;
			std::cout<< "Hemming filter selected"<<std::endl;
		}
		else if(paramList[0] == "b-spline"){
			Select::filter_ = b_spline;
			std::cout<< "B-spline filter selected"<<std::endl;
		}
		else if(paramList[0] == "catmull-rom"){
			Select::filter_ = catmull_rom;
			std::cout<< "Catmull-rom filter selected"<<std::endl;
		}
		else{
			std::cout<< "unknown filter. Filter name should be lower-cased"<<std::endl;
		}
		if(paramList.size()==2){
			std::string param2 = paramList[1];
			try{
				alpha_ = std::stoi(param2);
			}
			catch(...){
				return "second param is illegal";
			}
		}


		return "Select Done";
	}
	else{
		return  "too many parameters";
	}

}


