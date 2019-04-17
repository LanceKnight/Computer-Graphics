#include <iostream>
#include <vector>
#include "Plane.hh"
#include "Trace.hh"
#include "Util.hh"

int Plane::counter_ = 0;


Plane::Plane(){

}

Plane::~Plane(){

}

Plane::Plane(float ux, float uy, float uz,
		 float vx, float vy, float vz,
		 float ar, float ag, float ab,
		 float rr, float rg, float rb){
	std::vector<float> geo_params;
	std::vector<float> color_params;
	geo_params.push_back(ux);
	geo_params.push_back(uy);
	geo_params.push_back(uz);
	geo_params.push_back(vx);
	geo_params.push_back(vy);
	geo_params.push_back(vz);
	color_params.push_back(ar);
	color_params.push_back(ag);
	color_params.push_back(ab);
	color_params.push_back(rr);
	color_params.push_back(rg);
	color_params.push_back(rb);

	this-> name_ = "plane" + counter_;
	this-> type_ = "plane";
	this-> geo_params_ = geo_params;
	this-> color_params_ = color_params;

	this-> counter_++;
}


std::string
Plane::plane(std::vector<std::string> paramList){


	if(paramList.size()<=12){
		if(paramList.size() <12){
				return "too few parameters";
		}
		float ux = 0;
		float uy = 0;
		float uz = 0;
		float vx = 0;
		float vy = 0;
		float vz = 0;
		float ar = 0;
		float ag = 0;
		float ab = 0;
		float rr = 0;
		float rg = 0;
		float rb = 0;
		try{
			ux = std::stod(paramList[0]);
			uy = std::stod(paramList[1]);
			uz = std::stod(paramList[2]);
			vx = std::stod(paramList[3]);
			vy = std::stod(paramList[4]);
			vz = std::stod(paramList[5]);
			ar = std::stod(paramList[6]);
			ag = std::stod(paramList[7]);
			ab = std::stod(paramList[8]);
			rr = std::stod(paramList[9]);
			rg = std::stod(paramList[10]);
			rb = std::stod(paramList[11]);
		}
		catch(...){
			return "illegal params";
		}
		Plane * new_Plane = new Plane(ux, uy, uz,
							    vx, vy, vz,
								ar, ag, ab,
								rr, rg, rb);


		Trace::object_list_.push_back(new_Plane);

		return "Plane Done";
	}
	else{
		return  "too many parameters";
	}

}


