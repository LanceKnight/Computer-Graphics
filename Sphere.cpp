#include <iostream>
#include <cstring>
#include "Sphere.hh"
#include "Trace.hh"

#include "Util.hh"

//float t_ = -1;

int Sphere::counter_ = 0;

Sphere::Sphere(){

}

Sphere::~Sphere(){

}


Sphere::Sphere(float R, float cx, float cy, float cz,
						float ar, float ag, float ab,
						float rr, float rg, float rb,
						float sr, float sg, float sb){
	geo_params_.push_back(R);
	geo_params_.push_back(cx);
	geo_params_.push_back(cy);
	geo_params_.push_back(cz);
	color_params_.push_back(ar);
	color_params_.push_back(ag);
	color_params_.push_back(ab);
	color_params_.push_back(rr);
	color_params_.push_back(rg);
	color_params_.push_back(rb);
	color_params_.push_back(sr);
	color_params_.push_back(sg);
	color_params_.push_back(sb);

	name_ = "sphere "  + std::to_string(counter_);
	type_ = "sphere";

	this-> counter_++;

	if((sr!=0) || (sg !=0) || (sb !=0)){
		reflected_=true;

	}

}




std::string
Sphere::sphere(std::vector<std::string> paramList){


	if(paramList.size()<=13){
		if(paramList.size() <10){
			return "too few parameters";
		}
		float R = 0;
		float cx = 0;
		float cy = 0;
		float cz = 0;
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
			R = std::stod(paramList[0]);
			cx = std::stod(paramList[1]);
			cy = std::stod(paramList[2]);
			cz = std::stod(paramList[3]);
			ar = std::stod(paramList[4]);
			ag = std::stod(paramList[5]);
			ab = std::stod(paramList[6]);
			rr = std::stod(paramList[7]);
			rg = std::stod(paramList[8]);
			rb = std::stod(paramList[9]);
			if(paramList.size() ==13){
				sr = std::stod(paramList[10]);
				sg = std::stod(paramList[11]);
				sb = std::stod(paramList[12]);
			}
			else if(paramList.size()!=10){
				return "parameters numbers should be either 10 or 13";
			}
		}
		catch(...){
			return "illegal params";

		}



		Sphere * new_sphere = new Sphere(R,  cx, cy, cz,
				ar, ag, ab,
				rr, rg, rb,
				sr, sg, sb);


		Trace::object_list_.push_back(new_sphere);

		return "Sphere Done";
	}
	else{
		return  "too many parameters";
	}

}


