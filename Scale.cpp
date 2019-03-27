#include <iostream>
#include "Scale.hh"
#include "3D.h"
#include "Matrix.hh"
#include "Util.hh"


std::string
Scale::scale(std::vector<std::string> paramList){


	if(paramList.size()<=3){
		if(paramList.size() <3){
				return "too few parameters";
			}
			float sx = 0;
			float sy = 0;
			float sz = 0;

			try{
				sx = std::stod(paramList[0]);
				sy = std::stod(paramList[1]);
				sz = std::stod(paramList[2]);

			}
			catch(...){
				return "illegal params";

			}
			matrix_unit scale_mat = {
					   { {sx, 0., 0., 0. },
					     {0., sy, 0., 0. },
					     {0., 0., sz, 0. },
					     {0., 0., 0., 1.0}  },
					};

			matrix_unit tmpsln;

			Mult_mat(stack[top], &scale_mat, &tmpsln);
			Copy_mat(&tmpsln, stack[top]);
/*
			Util::debug_head("Scale.cpp");
				std::cout<<"stack["<<top<<"]:"<<std::endl;
				Util::print_mat(stack[top]);
			Util::debug_tail();
*/
		return "Scale Done";
	}
	else{
		return  "too many parameters";
	}

}


