#include <cmath>
#include <iostream>
#include <GL/gl.h>
#include "3D.h"
#include "Perspective.hh"
#include "Util.hh"


Perspective::Perspective(){}

Perspective::~Perspective(){}


std::string
Perspective::perspective(std::vector<std::string> paramList){


	if(paramList.size()<=4){
		if(paramList.size() <4){
			return "too few parameters";
		}
		float fov = 0;
		float a = 0;
		float n = 0;
		float f = 0;

		try{
			fov = std::stod(paramList[0]);
			a = std::stod(paramList[1]);
			n = std::stod(paramList[2])+0.0001;
			f = std::stod(paramList[3])+0.0001;
		}
		catch(...){
			return "illegal params";

		}

		perspflag = 1;

/*
		matrix_unit new_perspect= {
				   { {-n*(t-b)/(r-l), 0.          , 0           , 0            },
				     {0.            , -n          , 0.          , 0            },
				     {0.            , 0.          , (f+n)/(f-n) , -2*n*f/(f-n) },
				     {0.            , 0.          , -1.0         , 0           }  },
				   };
*/
		float t = tan(fov*M_PI/(2.0*180))*n;
		float b = -t;
		float r = t * a;
		float l = -t * a;
		matrix_unit new_perspect= {
				   { {2.0*n/(r-l)  , 0.          , (l+r)/(l-r) , 0            },
				     {0.           , 2.0*n/(t-b) , 0.          , 0            },
				     {0.           , 0.          , (f+n)/(n-f) , 2.0*f*n/(f-n)},
				     {0.           , 0.          , 1.0         , 0            }  },
				   };

		 GLint viewport[4];
		 glGetIntegerv(GL_VIEWPORT,viewport);
		 width = abs(viewport[2]-viewport[0]);
		 height = abs(viewport[3]-viewport[1]);
			Util::debug_head("Perspective.cpp");
				std::cout<<"width:"<<width<<", height:"<<height<<std::endl;
			Util::debug_tail();

		matrix_unit viewport_mat = {
				   { {width/2.0, 0.        , 0.     , (width-1) /2.0},
				     {0.       , height/2.0, 0.     , (height-1)/2.0},
				     {0.       , 0.        , 1      , 0  		    },
				     {0.       , 0.        , 0.     , 1             }  },
				   };
		Mult_mat(&viewport_mat, &new_perspect, &perspect);
		//Copy_mat(&new_perspect, &perspect);
		Util::debug_head("Perspective.cpp");
			std::cout<<"perspect:"<<std::endl;
			Util::print_mat(&perspect);
			std::cout<<"I:"<<std::endl;
			Util::print_mat(&I);
		Util::debug_tail();

		return "Perspective Done";
	}
	else{
		return  "too many parameters";
	}

}
