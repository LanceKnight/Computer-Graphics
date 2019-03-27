#include <iostream>
#include "Orient.hh"
#include "3D.h"
#include "Matrix.hh"
#include "Util.hh"


std::string
Orient::orient(std::vector<std::string> paramList){


	if(paramList.size()<=9){
		if(paramList.size() <3){
				return "too few parameters";
			}
			float a = 0;
			float b = 0;
			float c = 0;
			float d = 0;
			float e = 0;
			float f = 0;
			float g = 0;
			float h = 0;
			float i = 0;
			try{
				a = std::stod(paramList[0]);
				b = std::stod(paramList[1]);
				c = std::stod(paramList[2]);
				d = std::stod(paramList[0]);
				e = std::stod(paramList[1]);
				f = std::stod(paramList[2]);
				g = std::stod(paramList[0]);
				h = std::stod(paramList[1]);
				i = std::stod(paramList[2]);

			}
			catch(...){
				return "illegal params";

			}



			matrix_unit * orient_mat = new matrix_unit({
					   { {a , b , c , 0. },
					     {d , e , f , 0. },
					     {g , h , i , 0. },
					     {0., 0., 0., 1.0}  },
					});
			matrix_unit tmpsln;

			Mult_mat(stack[top], orient_mat, &tmpsln);
			Copy_mat(&tmpsln, stack[top]);
/*
			Util::debug_head("Orient.cpp");
				std::cout<<"stack["<<top<<"]:"<<std::endl;
				Util::print_mat(stack[top]);
			Util::debug_tail();
*/
		return "Orient Done";
	}
	else{
		return  "too many parameters";
	}

}


