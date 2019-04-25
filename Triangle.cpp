#include <iostream>
#include <cstring>
#include <vector>
#include "Triangle.hh"
#include "Trace.hh"
#include "Util.hh"

int Triangle::counter_ = 0;


Triangle::Triangle(){

}

Triangle::~Triangle(){

}
/*
Triangle::Triangle(float ux, float uy, float uz,
				   float vx, float vy, float vz,
				   float wx, float wy, float wz,
			   	   float ar, float ag, float ab,
				   float rr, float rg, float rb){
	std::vector<float> geo_params;
	std::vector<float> color_params;
	geo_params_.push_back(ux);
	geo_params_.push_back(uy);
	geo_params_.push_back(uz);
	geo_params_.push_back(vx);
	geo_params_.push_back(vy);
	geo_params_.push_back(vz);
	geo_params_.push_back(wx);
	geo_params_.push_back(wy);
	geo_params_.push_back(wz);
	color_params_.push_back(ar);
	color_params_.push_back(ag);
	color_params_.push_back(ab);
	color_params_.push_back(rr);
	color_params_.push_back(rg);
	color_params_.push_back(rb);

	this-> name_ = "triangle "+ std::to_string(counter_);
	this-> type_ = "triangle";
	//this->geo_params_ = geo_params;
	//this->color_params_= color_params;

	this-> counter_++;
}
*/
Triangle::Triangle(float ux, float uy, float uz,
				   float vx, float vy, float vz,
				   float wx, float wy, float wz,
			   	   float ar, float ag, float ab,
				   float rr, float rg, float rb,
				   float sr, float sg, float sb,
				   float pr, float pg, float pb, float p
){
	std::vector<float> geo_params;
	std::vector<float> color_params;
	geo_params_.push_back(ux);
	geo_params_.push_back(uy);
	geo_params_.push_back(uz);
	geo_params_.push_back(vx);
	geo_params_.push_back(vy);
	geo_params_.push_back(vz);
	geo_params_.push_back(wx);
	geo_params_.push_back(wy);
	geo_params_.push_back(wz);
	color_params_.push_back(ar);
	color_params_.push_back(ag);
	color_params_.push_back(ab);
	color_params_.push_back(rr);
	color_params_.push_back(rg);
	color_params_.push_back(rb);
	color_params_.push_back(sr);
	color_params_.push_back(sg);
	color_params_.push_back(sb);
	color_params_.push_back(pr);
	color_params_.push_back(pg);
	color_params_.push_back(pb);
	color_params_.push_back(p);
	this-> name_ = "triangle "+ std::to_string(counter_);
	this-> type_ = "triangle";
	//this->geo_params_ = geo_params;
	//this->color_params_= color_params;

	this-> counter_++;
	if((sr!=0) || (sg !=0) || (sb !=0)){
		reflected_=true;

	}

}


std::string
Triangle::triangle(std::vector<std::string> paramList){


	if(paramList.size()<=22){
		if(paramList.size() <15 || ( paramList.size() !=18 && paramList.size() !=22) ){
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
		float rr = 0;
		float rg = 0;
		float rb = 0;
		float sr = 0;
		float sg = 0;
		float sb = 0;
		float pr = 0;
		float pg = 0;
		float pb = 0;
		float p = 0;
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
			rr = std::stod(paramList[12]);
			rg = std::stod(paramList[13]);
			rb = std::stod(paramList[14]);
			if(paramList.size()==18){
				sr = std::stod(paramList[15]);
				sg = std::stod(paramList[16]);
				sb = std::stod(paramList[17]);
			}
			if(paramList.size()==22){
				pr = std::stod(paramList[18]);
				pg = std::stod(paramList[19]);
				pb = std::stod(paramList[20]);
				p  = std::stod(paramList[21]);
			}

		}
		catch(...){
			return "illegal params";
		}
		Triangle * new_triangle = new Triangle(ux, uy, uz,
										       vx, vy, vz,
										       wx, wy, wz,
										       ar, ag, ab,
										       rr, rg, rb,
											   sr, sg, sb,
											   pr, pg, pb, p
		);


		Trace::object_list_.push_back(new_triangle);

		return "Triangle Done";
	}
	else{
		return  "too many parameters";
	}

}


