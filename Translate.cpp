#include <iostream>
#include "Translate.hh"
#include "3D.h"
#include "Matrix.hh"
#include "Util.hh"


std::string
Translate::translate(std::vector<std::string> paramList){


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
			matrix_unit translate_mat = {
					   { {1., 0., 0., x},
					     {0., 1., 0., y},
					     {0., 0., 1., z},
					     {0., 0., 0., 1.0}  },
					};
			matrix_unit temp_mat;
			Mult_mat(&current, &translate_mat, &temp_mat);
			Copy_mat(&temp_mat, &current);

			Util::debug_head("Translate.cpp");
				std::cout<<"translate_mat:"<<std::endl;
				Util::print_mat(translate_mat);
			Util::debug_tail();

		return "Translate Done";
	}
	else{
		return  "too many parameters";
	}

}


