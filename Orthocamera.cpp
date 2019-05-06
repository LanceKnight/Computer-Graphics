#include <iostream>
#include "Orthocamera.hh"
#include "Camera.hh"

#include "3D.h"
#include "Util.hh"

bool Orthocamera::ortho_ = false;

std::string
Orthocamera::orthocamera(std::vector<std::string> paramList){


	if(paramList.size()==0){

		Orthocamera::ortho_ = true;
		Camera::perspect_ = false;

		mat3 a = {
				   { {3    , 4     , 5     },
					 {1    , 3     , 3     },
					 {5    , 5     , 5   }  },
				   };
		std::cout<<"========det======="<<std::endl;
		std::cout<<determinant(a)<<std::endl;


		return "Orthocamera Done";
	}
	else{
		return  "too many parameters";
	}

}


