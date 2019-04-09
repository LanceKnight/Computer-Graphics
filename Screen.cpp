#include <iostream>
extern "C"{
#include <GL/glut.h>
}
#include "Screen.hh"
#include "Util.hh"

float Screen::nx_ = 0;
float Screen::ny_ = 0;

std::string
Screen::screen(std::vector<std::string> paramList){


	if(paramList.size()<=2){
		if(paramList.size() <2){
				return "too few parameters";
		}


		try{
			Screen::nx_ = std::stod(paramList[0]);
			Screen::ny_ = std::stod(paramList[1]);
		}
		catch(...){
			return "illegal params";
		}
		glutReshapeWindow(Screen::nx_, Screen::ny_);
		glutPostRedisplay();
		return "Screen Done";
	}
	else{
		return  "too many parameters";
	}

}


