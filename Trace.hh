#ifndef TRACE_HH
#define TRACE_HH

#include <string>
#include <vector>
#include "Ray.hh"
#include "Shape.hh"



class Trace{

public:
	Trace();

	~Trace();

	std::string static trace(std::vector<std::string> paramList);

	//public data
	static std::vector<Shape *> object_list_;

private:
	static Ray buildCameraRay(int i,int j);

	static float intersect(Ray ray, Shape* shape);
	//private data

	
};


#endif
