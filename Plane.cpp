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

Plane::Plane(float nx, float ny, float nz,
		  float px, float py, float pz,
		  float ar, float ag, float ab,
		  float rr, float rg, float rb,
		  float sr, float sg, float sb
	){
	std::vector<float> geo_params;
	std::vector<float> color_params;
	geo_params.push_back(nx);
	geo_params.push_back(ny);
	geo_params.push_back(nz);
	geo_params.push_back(px);
	geo_params.push_back(py);
	geo_params.push_back(pz);
	color_params.push_back(ar);
	color_params.push_back(ag);
	color_params.push_back(ab);
	color_params.push_back(rr);
	color_params.push_back(rg);
	color_params.push_back(rb);
	color_params.push_back(sr);
	color_params.push_back(sg);
	color_params.push_back(sb);
//
//	Util::debug_head("Plane.cpp");
//	std::cout<<"sr:"<<sr<<" sg:"<<sg<<" sb:"<<sb<<std::endl;
//	Util::debug_tail();
	this-> name_ = "plane" + std::to_string(counter_);
	this-> type_ = "plane";
	this-> geo_params_ = geo_params;
	this-> color_params_ = color_params;
	this-> norm_ = {{nx, ny, nz}};

	this-> counter_++;
	this-> counter_++;
	if((sr!=0) || (sg !=0) || (sb !=0)){
		reflected_=true;

	}
}


std::string
Plane::plane(std::vector<std::string> paramList){


	if(paramList.size()<=15){
		if(paramList.size() <15){
				return "too few parameters";
		}
		float nx = 0;
		float ny = 0;
		float nz = 0;
		float px = 0;
		float py = 0;
		float pz = 0;
		float ar = 0;
		float ag = 0;
		float ab = 0;
		float rr = 0;
		float rg = 0;
		float rb = 0;
		float sr = 0;
		float sg = 0;
		float sb = 0;
		try{
			nx = std::stod(paramList[0]);
			ny = std::stod(paramList[1]);
			nz = std::stod(paramList[2]);
			px = std::stod(paramList[3]);
			py = std::stod(paramList[4]);
			pz = std::stod(paramList[5]);
			ar = std::stod(paramList[6]);
			ag = std::stod(paramList[7]);
			ab = std::stod(paramList[8]);
			rr = std::stod(paramList[9]);
			rg = std::stod(paramList[10]);
			rb = std::stod(paramList[11]);
			sr = std::stod(paramList[12]);
			sg = std::stod(paramList[13]);
			sb = std::stod(paramList[14]);
		}
		catch(...){
			return "illegal params";
		}
		Plane * new_Plane = new Plane(nx, ny, nz,
							    px, py, pz,
								ar, ag, ab,
								rr, rg, rb,
								sr, sg, sb
		);


		Trace::object_list_.push_back(new_Plane);

		return "Plane Done";
	}
	else{
		return  "too many parameters";
	}

}


