#ifndef SPHERE_HH
#define SPHERE_HH

#include <string>
#include <vector>

#include "Surface.hh"



class Sphere:public Surface{

public:
	Sphere();


	Sphere(float R, float cx, float cy, float cz,
					float ar, float ag, float ab,
					float rr, float rg, float rb,
					float sr, float sg, float sb,
					float pr, float pg, float pb, float p
					);



	~Sphere();

	std::string static sphere(std::vector<std::string> paramList);



	//data
	static int counter_;

	

};


#endif
