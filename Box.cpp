#include <iostream>
#include <vector>
#include "Box.hh"
#include "Trace.hh"
#include "Util.hh"
#include "3D.h"

int Box::counter_ = 0;

Box::Box(){

}

Box::~Box(){

}
/*
Box::Box(float ux, float uy, float uz,
		 float vx, float vy, float vz,
		 float ar, float ag, float ab,
		 float rr, float rg, float rb){

	geo_params_.push_back(ux);
	geo_params_.push_back(uy);
	geo_params_.push_back(uz);
	geo_params_.push_back(vx);
	geo_params_.push_back(vy);
	geo_params_.push_back(vz);
	color_params_.push_back(ar);
	color_params_.push_back(ag);
	color_params_.push_back(ab);
	color_params_.push_back(rr);
	color_params_.push_back(rg);
	color_params_.push_back(rb);

	name_ = "box"+ std::to_string(counter_);
	type_ = "box";

	counter_++;
}
*/
Box::Box(float ux, float uy, float uz,
		 float vx, float vy, float vz,
		 float ar, float ag, float ab,
		 float rr, float rg, float rb,
		 float sr, float sg, float sb,
		 float pr, float pg, float pb, float p
){
	geo_params_.push_back(ux);
	geo_params_.push_back(uy);
	geo_params_.push_back(uz);
	geo_params_.push_back(vx);
	geo_params_.push_back(vy);
	geo_params_.push_back(vz);
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
	name_ = "box"+ std::to_string(counter_);
	type_ = "box";

	counter_++;
	this-> counter_++;
	if((sr!=0) || (sg !=0) || (sb !=0)){
		reflected_=true;

	}
}


std::string
Box::box(std::vector<std::string> paramList){


	if(paramList.size()<=19){
		if(paramList.size() <12 || (paramList.size() != 15 && paramList.size()!=19)){
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
			ar = std::stod(paramList[6]);
			ag = std::stod(paramList[7]);
			ab = std::stod(paramList[8]);
			rr = std::stod(paramList[9]);
			rg = std::stod(paramList[10]);
			rb = std::stod(paramList[11]);
			if(paramList.size()==15){
				sr = std::stod(paramList[12]);
				sg = std::stod(paramList[13]);
				sb = std::stod(paramList[14]);
			}
			if(paramList.size()==19){
				pr = std::stod(paramList[15]);
				pg = std::stod(paramList[16]);
				pb = std::stod(paramList[17]);
				p = std::stod(paramList[18]);
			}
		}
		catch(...){
			return "illegal params";
		}
/*
		if(paramList.size()==12){
			Box * new_Box = new Box(ux, uy, uz,
					vx, vy, vz,
					ar, ag, ab,
					rr, rg, rb);
			Trace::object_list_.push_back(new_Box);
		}
		else{
			Box * new_Box = new Box(ux, uy, uz,
					vx, vy, vz,
					ar, ag, ab,
					rr, rg, rb,
					sr, sg, sb);
			Trace::object_list_.push_back(new_Box);
		}*/

		Box * new_Box = new Box(ux, uy, uz,
				vx, vy, vz,
				ar, ag, ab,
				rr, rg, rb,
				sr, sg, sb,
				pr, pg, pb, p
		);
		Trace::object_list_.push_back(new_Box);

		return "Box Done";
	}
	else{
		return  "too many parameters";
	}

}


