#ifndef TRACE_HH
#define TRACE_HH

#include <string>
#include <vector>

#include "Ilight.hh"
#include "Ray.hh"
#include "Surface.hh"



class Trace{

public:


	std::string static trace(std::vector<std::string> paramList);

	//public data
	static std::vector<Surface *> object_list_;
	static std::vector<Ilight *> light_list_;

private:
	static vec3 getColorFromRay(Ray ray, float t0, float t, int i, int j);

	static Ray buildCameraRay(int i,int j);

	static vec3 buildLightDir(const Ilight* ilight, const vec3 intersect_point);

	static Ray buildReflectedRay(Ray ray, vec3 norm, vec3 point, int i ,int j);

	static float intersect(Ray ray, Surface* shape, float t0, float t1, int i, int j);

	static float intersectTriangle(Surface* shape, Ray ray, float t0, float t1);

	static float intersectSphere(Surface* shape, Ray ray, float t0, float t1);

	static float intersectBox(Surface* shape, Ray ray, float t0, float t1, int i, int j);

	static float intersectPlane( Surface* shape, Ray ray);

	//private data

	static int reflect_counter_;

	
};


#endif
