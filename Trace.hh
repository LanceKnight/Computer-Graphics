#ifndef TRACE_HH
#define TRACE_HH

#include <string>
#include <vector>

#include "Ilight.hh"
#include "Ray.hh"
#include "Shape.hh"



class Trace{

public:


	std::string static trace(std::vector<std::string> paramList);

	//public data
	static std::vector<Shape *> object_list_;
	static std::vector<Ilight *> light_list_;

private:
	static Ray buildCameraRay(int i,int j);

	static Ray buildReflectedRay(Ray ray, vec3 norm, vec3 point, int i ,int j);

	static vec3 getColorFromRay(Ray ray, int i, int j);

	static float intersect(Ray ray, Shape* shape, int i, int j);

	static float intersectSquare( vec3 vec3_e, vec3 vec3_d, float ax, float xy, float az, float bx, float by, float bz);

	static float intersectPlane( vec3 vec3_e, vec3 vec3_d, float nx, float ny, float nz, float px, float py, float pz);

	//private data

	static int reflect_counter_;

	
};


#endif
