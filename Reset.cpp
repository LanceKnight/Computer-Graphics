#include <iostream>
#include <GL/glut.h>
#include "3D.h"
#include "Reset.hh"
#include "Util.hh"


Reset::Reset(){}

Reset::~Reset(){}


std::string
Reset::reset(std::vector<std::string> paramList){


	if(paramList.size()<1){
		glClear(GL_COLOR_BUFFER_BIT);
		glRasterPos2i(0,0);
		glFlush();
		top = 0;
		static matrix_unit i;
		Copy_mat(&I, &i);

		stack[0] = &i;
		Util::debug_head("Reset.cpp");
			std::cout<<"stack["<<top<<"]:"<<std::endl;
			Util::print_mat(stack[top]);
			std::cout<<"I:"<<std::endl;
			Util::print_mat(&I);
		Util::debug_tail();

		return "Reset Done";
	}
	else{
		return  "too many parameters";
	}

}
