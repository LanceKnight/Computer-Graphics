#include <iostream>
#include <cmath>
#include "Rotate.hh"
#include "3D.h"
#include "Util.hh"


std::string
Rotate::rotate(std::vector<std::string> paramList){


	if(paramList.size()<=4){
		if(paramList.size() <4){
				return "too few parameters";
			}
			float theta = 0;
			float x = 0;
			float y = 0;
			float z = 0;

			try{
				theta = std::stod(paramList[0]);
				x = std::stod(paramList[1]);
				y = std::stod(paramList[2]);
				z = std::stod(paramList[3]);

			}
			catch(...){
				return "illegal params";

			}

			Vector w;
			Unitvec(x, y, z, &w);
			Vector arbi = {1,0,0};
			Vector u;
			Vector temp;
			Cross(&w, &arbi, &temp);
			if(temp.i == 0 && temp.j==0 && temp.k == 0){
				arbi.i = 0;
				arbi.j = 1;
				arbi.k = 0;
				Cross(&w, &arbi, &temp);
			}
			Unitvec(temp.i, temp.j, temp.k, &u);

			Vector v;
			Cross(&w, &u, &temp);
			Unitvec(temp.i, temp.j, temp.k, &v);


			matrix_unit  mat1 ={
					   { {u.i, v.i, w.i, 0.},
					     {u.j, v.j, w.j, 0.},
					     {u.k, v.k, w.k, 0.},
					     {0. , 0. , 0. , 1.}  },
					   };
			matrix_unit  mat2 ={
					   { {cos(theta*M_PI/180), -sin(theta*M_PI/180), 0., 0.},
					     {sin(theta*M_PI/180), cos(theta*M_PI/180) , 0., 0.},
					     {0.                 , 0.                 ,  1., 0.},
					     {0.                 , 0.                  , 0. , 1.}  },
					   };

			matrix_unit  mat3 ={
					   { {u.i, u.j, u.k, 0.},
					     {v.i, v.j, v.k, 0.},
					     {w.i, w.j, w.k, 0.},
					     {0. , 0. , 0. , 1.}  },
					   };



			matrix_unit mat12;
			matrix_unit * rotate_mat = new matrix_unit;
			Mult_mat(&mat1, &mat2, &mat12);
			Mult_mat(&mat12, &mat3, rotate_mat);
/*
			*rotate_mat ={
					   { {1, 0, 0, 0.},
					     {0, 1, 0, 0.},
					     {0, 0, 1, 0.},
					     {0. , 0. , 0. , 1.}  },
					   };*/
			matrix_unit tmpsln;

			Mult_mat(stack[top], rotate_mat, &tmpsln);
			Copy_mat(&tmpsln, stack[top]);
/*
			Util::debug_head("Rotate.cpp");
				std::cout<<"w: "<< w.i<<" " <<w.j<<" "<<w.k<<std::endl;
				std::cout<<"u: "<< u.i<<" " <<u.j<<" "<<u.k<<std::endl;
				std::cout<<"v: "<< v.i<<" " <<v.j<<" "<<v.k<<std::endl;
				std::cout<<"mat1:"<<std::endl;
				Util::print_mat(&mat1);
				std::cout<<"mat2:"<<std::endl;
				Util::print_mat(&mat2);
				std::cout<<"mat3:"<<std::endl;
				Util::print_mat(&mat3);
				std::cout<<"rotate_mat:"<<std::endl;
				Util::print_mat(rotate_mat);
				std::cout<<"stack["<<top<<"]:"<<std::endl;
				Util::print_mat(stack[top]);
			Util::debug_tail();
*/

		return "Rotate Done";
	}
	else{
		return  "too many parameters";
	}

}


