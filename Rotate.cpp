#include <iostream>
#include "Rotate.hh"
#include "3D.h"



std::string
Rotate::rotate(std::vector<std::string> paramList){


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
			matrix_unit rotate_mat = {
					   { {1., 0., 0., x},
					     {0., 1., 0., y},
					     {0., 0., 1., z},
					     {0., 0., 0., 1.0}  },
					};
			Mult_mat(&current, &rotate_mat, &current);


		return "Rotate Done";
	}
	else{
		return  "too many parameters";
	}

}


