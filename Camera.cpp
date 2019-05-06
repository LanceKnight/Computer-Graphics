#include <iostream>
#include "Camera.hh"
#include "Orthocamera.hh"
#include "Util.hh"


bool Camera::perspect_ = false;
float Camera::ex_=0;
float Camera::ey_=0;
float Camera::ez_=0;
float Camera::gx_=0;
float Camera::gy_=0;
float Camera::gz_=0;
float Camera::ux_=0;
float Camera::uy_=0;
float Camera::uz_=0;
float Camera::s_ =0;
float Camera::au_=0;
float Camera::av_=0;
float Camera::bu_=0;
float Camera::bv_=0;

std::string
Camera::camera(std::vector<std::string> paramList){


	if(paramList.size()<=14){
		if(paramList.size() <14){
				return "too few parameters";
			}


			try{
				ex_ = std::stod(paramList[0]);
				ey_ = std::stod(paramList[1]);
				ez_ = std::stod(paramList[2]);
				gx_ = std::stod(paramList[3]);
				gy_ = std::stod(paramList[4]);
				gz_ = std::stod(paramList[5]);
				ux_ = std::stod(paramList[6]);
				uy_ = std::stod(paramList[7]);
				uz_ = std::stod(paramList[8]);
				s_  = std::stod(paramList[9]);
				au_ = std::stod(paramList[10]);
				av_ = std::stod(paramList[11]);
				bu_ = std::stod(paramList[12]);
				bv_ = std::stod(paramList[13]);

			}
			catch(...){
				return "illegal params";

			}

			perspect_ = true;
			Orthocamera::ortho_ = false;


		return "Camera Done";
	}
	else{
		return  "too many parameters";
	}

}


