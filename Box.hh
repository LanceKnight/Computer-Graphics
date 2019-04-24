#ifndef BOX_HH
#define BOX_HH

#include <string>
#include <vector>

#include "3D.h"
#include "Surface.hh"



class Box:Surface{

public:
	Box();

	Box(float ux, float uy, float uz,
		float vx, float vy, float vz,
		float ar, float ag, float ab,
		float rr, float rg, float rb);

	Box(float ux, float uy, float uz,
		float vx, float vy, float vz,
		float ar, float ag, float ab,
		float rr, float rg, float rb,
		float sr, float sg, float sb);

	~Box();

	std::string static box(std::vector<std::string> paramList);



	static int counter_;

	

};


#endif
