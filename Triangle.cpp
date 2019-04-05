#include <iostream>
#include "Triangle.hh"
#include "3D.h"
#include "Util.hh"


std::string
Triangle::triangle(std::vector<std::string> paramList){


	if(paramList.size()<=3){
		if(paramList.size() <3){
				return "too few parameters";
			}
			float x = 0;
			float y = 0;
			float z = 0;

			try{
				x = std::stod(paramList[0]);
				y = std::stod(paramList[1]);
				z = std::stod(paramList[2]);

			}
			catch(...){
				return "illegal params";

			}



			matrix_unit * triangle_mat = new matrix_unit({
					   { {1., 0., 0., x},
					     {0., 1., 0., y},
					     {0., 0., 1., z},
					     {0., 0., 0., 1.0}  },
					});
			matrix_unit tmpsln;

			Mult_mat(stack[top], triangle_mat, &tmpsln);
			Copy_mat(&tmpsln, stack[top]);

		return "Triangle Done";
	}
	else{
		return  "too many parameters";
	}

}


