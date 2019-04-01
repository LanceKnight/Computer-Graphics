#include <vector>
#include <iostream>
#include "Resize.hh"
#include "Zoom.hh"


Zoom::Zoom(){}

Zoom::~Zoom(){}


std::string
Zoom::zoom(std::vector<std::string> paramList){


	if(paramList.size()<=1){
		if(paramList.size()<1){
			return "you should enter a param.";
		}
		#ifdef DEBUG
			std::cout<<"=====DEBUG  INFO====="<< std::endl;
			for (auto it = paramList.begin(); it!= paramList.end(); it++){
				std::cout<<"param:"<<*it<<std::endl;
			}

			std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
		#endif


		float scale =0;
		try{
			scale = std::stod(paramList[0]);
		}
		catch(...)
		{
			return "illegal param";
		}		

		std::vector<std::string> new_param_list;
		new_param_list.push_back(std::to_string(scale));
		new_param_list.push_back(std::to_string(scale));



		return Resize::resize(new_param_list);
	}
	else{
		return  "too many parameters";
	}

}


