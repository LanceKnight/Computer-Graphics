#include <iostream>
#include "3D.h"
#include "Ortho.hh"


Ortho::Ortho(){}

Ortho::~Ortho(){}


std::string
Ortho::ortho(std::vector<std::string> paramList){


	if(paramList.size()<=6){
		if(paramList.size() <6){
			return "too few parameters";
		}
		double l = 0;
		double r = 0;
		double b = 0;
		double t = 0;
		double n = 0;
		double f = 0;
		try{
			l = std::stod(paramList[0]);
			r = std::stod(paramList[1]);
			b = std::stod(paramList[2]);
			t = std::stod(paramList[3]);
			n = std::stod(paramList[4]);
			f = std::stod(paramList[5]);
		}
		catch(...){
			return "illegal params";

		}
		perspflag = 0;
		orth= {
				   { {2.0/(r-l), 0.     , 0.     , -(r+l)/(r-l)},
				     {0.       , 2/(t-b), 0.     , -(t+b)/(t-b)},
				     {0.       , 0.     , 2/(f-n), -(f+n)/(f-n)},
				     {0.       , 0.     , 0.     , 1           }  },
				   };


		return "Ortho Done";
	}
	else{
		return  "too many parameters";
	}

}


