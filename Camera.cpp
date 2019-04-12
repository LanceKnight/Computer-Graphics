#include <iostream>
#include "Camera.hh"
#include "Util.hh"


bool Camera::perspect_ = false;

std::string
Camera::camera(std::vector<std::string> paramList){


	if(paramList.size()<=14){
		if(paramList.size() <14){
				return "too few parameters";
			}
			float ex = 0;
			float ey = 0;
			float ez = 0;

			try{
				ex = std::stod(paramList[0]);
				ey = std::stod(paramList[1]);
				ez = std::stod(paramList[2]);

			}
			catch(...){
				return "illegal params";

			}




		return "Camera Done";
	}
	else{
		return  "too many parameters";
	}

}


