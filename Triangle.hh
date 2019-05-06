#ifndef TRIANGLE_HH
#define TRIANGLE_HH

#include <string>
#include <vector>

#include "3D.h"
#include "Surface.hh"



class Triangle:public Surface{

public:
	Triangle();
/*
	Triangle(float ux, float uy, float uz,
			 float vx, float vy, float vz,
			 float wx, float wy, float wz,
			 float ar, float ag, float ab,
			 float rr, float rg, float rb);
*/
	Triangle(float ux, float uy, float uz,
			 float vx, float vy, float vz,
			 float wx, float wy, float wz,
			 float ar, float ag, float ab,
			 float rr, float rg, float rb,
			 float sr, float sg, float sb,
			 float pr, float pg, float pb, float p
	);

	~Triangle();

	std::string static triangle(std::vector<std::string> paramList);


	static int counter_;

	

};


#endif
