#include <iostream>
#include <GL/glut.h>
#include "3D.h"
#include "Reset.hh"


Reset::Reset(){}

Reset::~Reset(){}


std::string
Reset::reset(std::vector<std::string> paramList){


	if(paramList.size()<1){
		glClear(GL_COLOR_BUFFER_BIT);
		glRasterPos2i(0,0);
		glFlush();
		top = 0;

		return "Reset Done";
	}
	else{
		return  "too many parameters";
	}

}
