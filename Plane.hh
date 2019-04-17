#ifndef PLANE_HH
#define PLANE_HH

#include <string>
#include <vector>

#include "3D.h"
#include "Shape.hh"



class Plane:Shape{

public:
	Plane();

	Plane(float nx, float ny, float nz,
		  float px, float py, float pz,
		  float ar, float ag, float ab,
		  float rr, float rg, float rb,
		  float sr, float sg, float sb
	);

	~Plane();

	std::string static plane(std::vector<std::string> paramList);

	static vec3 norm_;

private:
	//private data
	static int counter_;

	

};


#endif
