#include <iostream>
#include <vector>
#include "Triangle.hh"
#include "Trace.hh"
#include "Util.hh"

int Triangle::counter_ = 0;


Triangle::Triangle(){

}

Triangle::~Triangle(){

}

Triangle::Triangle(float ux, float uy, float uz,
				   float vx, float vy, float vz,
				   float wx, float wy, float wz,
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
	geo_params.push_back(wx);
	geo_params.push_back(wy);
	geo_params.push_back(wz);
	color_params.push_back(ar);
	color_params.push_back(ag);
	color_params.push_back(ab);
	color_params.push_back(rr);
	color_params.push_back(rg);
	color_params.push_back(rb);

	this-> name_ = "triangle" + counter_;
	this-> type_ = "triangle";
	this-> geo_params_ = geo_params;
	this-> color_params_ = color_params;

	this-> counter_++;
}


std::string
Triangle::triangle(std::vector<std::string> paramList){


	if(paramList.size()<=15){
		if(paramList.size() <15){
				return "too few parameters";
		}
		float ux = 0;
		float uy = 0;
		float uz = 0;
		float vx = 0;
		float vy = 0;
		float vz = 0;
		float wx = 0;
		float wy = 0;
		float wz = 0;
		float ar = 0;
		float ag = 0;
		float ab = 0;
		float sr = 0;
		float sg = 0;
		float sb = 0;
		try{
			ux = std::stod(paramList[0]);
			uy = std::stod(paramList[1]);
			uz = std::stod(paramList[2]);
			vx = std::stod(paramList[3]);
			vy = std::stod(paramList[4]);
			vz = std::stod(paramList[5]);
			wx = std::stod(paramList[6]);
			wy = std::stod(paramList[7]);
			wz = std::stod(paramList[8]);
			ar = std::stod(paramList[9]);
			ag = std::stod(paramList[10]);
			ab = std::stod(paramList[11]);
			sr = std::stod(paramList[12]);
			sg = std::stod(paramList[13]);
			sb = std::stod(paramList[14]);
		}
		catch(...){
			return "illegal params";
		}
		Triangle * new_triangle = new Triangle(ux, uy, uz,
										       vx, vy, vz,
										       wx, wy, wz,
										       ar, ag, ab,
										       sr, sg, sb);


		Trace::object_list_.push_back(new_triangle);

		return "Triangle Done";
	}
	else{
		return  "too many parameters";
	}

}


