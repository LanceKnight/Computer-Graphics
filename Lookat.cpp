#include <iostream>
#include <string>
#include <vector>
#include "Lookat.hh"
#include "3D.h"


Lookat::Lookat(){}

Lookat::~Lookat(){}

std::string
Lookat::lookat(std::vector<std::string> paramList){
	if(paramList.size()<=9){
		if(paramList.size() <9){
			return "too few parameters";
		}
		float fx = 0;
		float fy = 0;
		float fz = 0;
		float ax = 0;
		float ay = 0;
		float az = 0;
		float ux = 0;
		float uy = 0;
		float uz = 0;
		try{
			fx = std::stod(paramList[0]);
			fy = std::stod(paramList[1]);
			fz = std::stod(paramList[2]);
			ax = std::stod(paramList[3]);
			ay = std::stod(paramList[4]);
			az = std::stod(paramList[5]);
			ux = std::stod(paramList[6]);
			uy = std::stod(paramList[7]);
			uz = std::stod(paramList[8]);
		}
		catch(...){
			return "illegal params";

		}
		//extern void gtLookAt( float fx, float fy, float fz, float atx, float aty,
			      // float atz, float upx, float upy, float upz);
		gtLookAt(fx,fy,fz,ax,ay,az,ux,uy,uz);


		return "Lookat Done";
	}
	else{
		return "too many parameters";

	}

}



