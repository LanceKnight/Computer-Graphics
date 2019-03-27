#include <iostream>
#include <string>
#include <vector>
#include "Vertex.hh"
#include "3D.h"


Vertex::Vertex(){}

Vertex::~Vertex(){}

std::string
Vertex::vertex(std::vector<std::string> paramList){
	if(paramList.size()<=3){
		if(paramList.size() <3){
			return "too few parameters";
		}
		double x = 0;
		double y = 0;
		double z = 0;

		try{
			x = std::stod(paramList[0]);
			y = std::stod(paramList[1]);
			z = std::stod(paramList[2]);

		}
		catch(...){
			return "illegal params";

		}
		//extern void gtLookAt( float fx, float fy, float fz, float atx, float aty,
			      // float atz, float upx, float upy, float upz);
		gtVertex3f(x,y,z);


		return "Vertex Done";
	}
	else{
		return "too many parameters";

	}

}



