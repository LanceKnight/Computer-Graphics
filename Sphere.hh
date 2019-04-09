#ifndef SPHERE_HH
#define SPHERE_HH

#include <string>
#include <vector>
#include "Shape.hh"



class Sphere:Shape{

public:
	Sphere();


	Sphere(float R, float cx, float cy, float cz,
					  float ar, float ag, float ab,
					  float sr, float sg, float sb);

	~Sphere();

	std::string static sphere(std::vector<std::string> paramList);



	//data
	//float t_;
private:
	//private data
	static int counter_;
	

};


#endif
