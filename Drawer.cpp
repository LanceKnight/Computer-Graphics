#include <vector>
#include <iostream>
#include "Drawer.hh"


Drawer::Drawer(){}

Drawer::~Drawer(){}


std::string
Drawer::draw(std::vector<std::string> paramList){


	if(paramList.size()<=3){
		while(paramList.size()<3){
			paramList.push_back("0");
		}
		#ifdef DEBUG
			std::cout<<"=====DEBUG  INFO====="<< std::endl;
			for (auto it = paramList.begin(); it!= paramList.end(); it++){
				std::cout<<"param:"<<*it<<std::endl;
			}

			std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
		#endif
		std::string result = "";	
		for (auto it = paramList.begin(); it!= paramList.end(); it++){
			try{
				std::stod(*it);
			}
			catch(...)
			{
				std::cout <<"WARNING: illegal parameter has been replace with 0"<< std::endl;
				*it = "0"; 
			}
			result += *it + " ";
		}
		return result;
	}
	else{
		return  "too many parameters";
	}

}


