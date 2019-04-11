#include <cstring>
#include <iostream>
#include "Ilight.hh"
#include "Trace.hh"
#include "Util.hh"

int Ilight::counter_=0;

Ilight::Ilight(){

}

Ilight::Ilight(float lr, float lg, float lb,
		   	   float dx, float dy, float dz){
	lr_ = lr;
	lg_ = lg;
	lb_ = lb;
	dx_ = dx;
	dy_ = dy;
	dz_ = dz;
	name_ = "light "+std::to_string(counter_);
	counter_++;

}

Ilight::~Ilight(){

}


std::string
Ilight::ilight(std::vector<std::string> paramList){


	if(paramList.size()<=6){
		if(paramList.size() <6){
				return "too few parameters";
			}
			float lr = 0;
			float lg = 0;
			float lb = 0;
			float dx = 0;
			float dy = 0;
			float dz = 0;
			try{
				lr = std::stod(paramList[0]);
				lg = std::stod(paramList[1]);
				lb = std::stod(paramList[2]);
				dx = std::stod(paramList[3]);
				dy = std::stod(paramList[4]);
				dz = std::stod(paramList[5]);

			}
			catch(...){
				return "illegal params";

			}

			Ilight * new_light = new Ilight(lr, lg, lb, dx, dy, dz);

			Trace::light_list_.push_back(new_light);

		return "Ilight Done";
	}
	else{
		return  "too many parameters";
	}

}


