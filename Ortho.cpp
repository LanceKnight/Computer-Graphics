#include <GL/gl.h>
#include <iostream>
#include "3D.h"
#include "Ortho.hh"
#include "Util.hh"


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

		Near = -n;
		Far = -f;
		perspflag = 0;
		matrix_unit new_orth= {
				   { {2.0/(r-l), 0.     ,   0.     , -(r+l)/(r-l) },
				     {0.       , 2.0/(t-b), 0.     , -(t+b)/(t-b) },
				     {0.       , 0.     ,   -2.0/(f-n),-(f+n)/(f-n) },
				     {0.       , 0.     ,   0.     , 1            }  },
				   };
		 GLint viewport[4];
		 glGetIntegerv(GL_VIEWPORT,viewport);
		 width = abs(viewport[2]-viewport[0]);
		 height = abs(viewport[3]-viewport[1]);
			Util::debug_head("Ortho.cpp");
				std::cout<<"width:"<<width<<", height:"<<height<<std::endl;
			Util::debug_tail();

		matrix_unit viewport_mat = {
				   { {width/2.0, 0.        , 0.     , (width-1) /2.0},
				     {0.       , height/2.0, 0.     , (height-1)/2.0},
				     {0.       , 0.        , 1      , 0  		    },
				     {0.       , 0.        , 0.     , 1             }  },
				   };
		Mult_mat(&viewport_mat, &new_orth, &orth);

		Util::debug_head("Ortho.cpp");
			std::cout<<"Ortho:"<<std::endl;
			Util::print_mat(&orth);
			std::cout<<"I:"<<std::endl;
			Util::print_mat(&I);
		Util::debug_tail();



		return "Ortho Done";
	}
	else{
		return  "too many parameters";
	}

}


