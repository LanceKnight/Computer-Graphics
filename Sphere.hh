#ifndef SPHERE_HH
#define SPHERE_HH

#include <string>
#include <vector>
#include "Shape.hh"



class Sphere:public Shape{

public:
	Sphere();


	Sphere(float R, float cx, float cy, float cz,
					float ar, float ag, float ab,
					float rr, float rg, float rb,
					float sr, float sg, float sb);



	~Sphere();

	std::string static sphere(std::vector<std::string> paramList);



	//data
	//float t_;

	static int counter_;

	

};


#endif
