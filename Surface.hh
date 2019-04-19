#ifndef SURFACE_HH
#define SURFACE_HH

#include <string>
#include <vector>

#include "3D.h"


class Surface{

public:
	Surface();

	~Surface();
/*
	virtual std::vector<float> get_geo_params();

	virtual std::vector<float> get_color_params();

	virtual std::string get_type();

	virtual void set_geo_params(std::vector<float> param_list);

	virtual void set_color_params(std::vector<float> param_list);
	*/
	//data

	std::string type_;
	std::string name_;
	std::vector<float> geo_params_;
	std::vector<float> color_params_;
	bool reflected_ = false;
	vec3 norm_;

};


#endif
