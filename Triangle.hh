#ifndef TRIANGLE_HH
#define TRIANGLE_HH

#include <string>
#include <vector>
#include "Shape.hh"



class Triangle:Shape{

public:
	Triangle();

	Triangle(float ux, float uy, float uz,
			 float vx, float vy, float vz,
			 float wx, float wy, float wz,
			 float ar, float ag, float ab,
			 float sr, float sg, float sb);

	~Triangle();

	std::string static triangle(std::vector<std::string> paramList);


private:
	//private data
	static int counter_;

	

};


#endif
