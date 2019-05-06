#include <iostream>
#include <cmath>
#include "Background.hh"
#include "3D.h"
#include "Util.hh"

float Background::r_ =0;
float Background::g_ =0;
float Background::b_ =0;

std::string
Background::background(std::vector<std::string> paramList){


	if(paramList.size()<=3){
		if(paramList.size() <3){
				return "too few parameters";
		}

		try{
			r_ = std::max(0., std::min(255.,std::stod(paramList[0])));
			g_ = std::max(0., std::min(255.,std::stod(paramList[1])));
			b_ = std::max(0., std::min(255.,std::stod(paramList[2])));
		}
		catch(...){
			return "illegal params";

		}



		return "Background Done";
	}
	else{
		return  "too many parameters";
	}

}


