#ifndef PLANE_HH
#define PLANE_HH

#include <string>
#include <vector>

#include "3D.h"
#include "Shape.hh"



class Plane:Shape{

public:
	Plane();

	Plane(float ux, float uy, float uz,
		float vx, float vy, float vz,
		float ar, float ag, float ab,
		float rr, float rg, float rb);

	~Plane();

	std::string static plane(std::vector<std::string> paramList);

	static vec3 norm_;

private:
	//private data
	static int counter_;

	

};


#endif
