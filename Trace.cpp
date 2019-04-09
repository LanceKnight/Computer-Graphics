#include <algorithm>
#include <cstring>
#include <cmath>
#include <GL/glut.h>
#include <iostream>
#include <limits>

#include "Background.hh"
#include "Camera.hh"
#include "Orthocamera.hh"
#include "Ray.hh"
#include "Screen.hh"
#include "Trace.hh"

#include "3D.h"
#include "main.hh"
#include "Util.hh"



std::vector<Shape *> Trace::object_list_;

std::string
Trace::trace(std::vector<std::string> paramList){


	if(paramList.size()==0){

		if(Orthocamera::ortho_){




			for(int i = 0; i < Screen::nx_; i++){
				for(int j = 0; j < Screen::ny_; j++){
					float smallest_t = std::numeric_limits<float>::infinity();
					Shape* render_object;
					bool hit = false;
					for(std::vector<Shape*>::iterator it = Trace::object_list_.begin();it != object_list_.end();++it){
						Ray ray = buildCameraRay(i,j);
						float t = intersect(ray, *it);
						if(t){
							hit = true;
							if(t<smallest_t){
								smallest_t = t;
								render_object = *it;
							}
						}

					}
					if(hit){
						checkImage[i][j][0]= render_object->color_params_[0];
						checkImage[i][j][1]= render_object->color_params_[1];
						checkImage[i][j][2]= render_object->color_params_[2];
					}
					else
					{
						checkImage[i][j][0]= Background::r_;
						checkImage[i][j][1]= Background::g_;
						checkImage[i][j][2]= Background::b_;
					}
				}
			}
		}
		else if (Camera::perspect_){

		}
		else{
			return "You need to specify one view method: orthographic or perspective.";
		}
		display();
		return "Trace Done";
	}
	else{
		return  "too many parameters";
	}

}

Ray
Trace::buildCameraRay(int i, int j){
	if(Orthocamera::ortho_){
		Ray ray(0, 0 , -1, j, i, -1);
		return ray;
	}
	else{
		Ray ray(0, 0 , -1, j, i, -1);
		return ray;
	}
}

float
Trace::intersect(Ray ray, Shape* shape){
	float t0 = 0;
	float dx = ray.dir_x_;
	float dy = ray.dir_y_;
	float dz = ray.dir_z_;
	float ex = ray.ori_x_;
	float ey = ray.ori_y_;
	float ez = ray.ori_z_;
	vec3 vec3_e = {{ex, ey, ez}};
	vec3 vec3_d = {{dx, dy, dz}};
	if(!strcmp(shape->type_.c_str(), "triangle")){
		//Util::debug_head("Trace.cpp");
			//std::cout<<"testing triangle intersection"<<std::endl;
		//Util::debug_tail();
		float ax = shape->geo_params_[0];
		float ay = shape->geo_params_[1];
		float az = shape->geo_params_[2];
		float bx = shape->geo_params_[3];
		float by = shape->geo_params_[4];
		float bz = shape->geo_params_[5];
		float cx = shape->geo_params_[6];
		float cy = shape->geo_params_[7];
		float cz = shape->geo_params_[8];

		mat3 A = {
				   { {ax-bx    , ax-cx     , dx     },
					 {ay-by    , ay-cy     , dy     },
					 {az-bz    , az-cz     , dz     }  },
				  };
		float deter_A = determinant(A);
		mat3 t_mat = {
				{ {ax-bx    , ax-cx     , ax-ex     },
				  {ay-by    , ay-cy     , ay-ey     },
				  {az-bz    , az-cz     , az-ez     }  },
				};

		float t = determinant(t_mat)/deter_A;
		if(t<t0){
			return 0;
		}

		mat3 gamma_mat = {
					{ {ax-bx    , ax-ex     , dx     },
					  {ay-by    , ay-ey     , dy     },
					  {az-bz    , az-ez     , dz     }  },
					};
		float gamma = determinant(gamma_mat)/deter_A;

		if(gamma<0 ||gamma >1){
			return 0;
		}

		mat3 beta_mat = {
				   { {ax-ex    , ax-cx     , dx     },
					 {ay-ey    , ay-cy     , dy     },
					 {az-ez    , az-cz     , dz     }  },
				   };
		float beta = determinant(beta_mat)/deter_A;
		if(beta<0 || beta>(1-gamma)){
			return 0;
		}
		return t;

	}
	else if(!strcmp(shape->type_.c_str(), "sphere")){
		float R = shape->geo_params_[0];
		float cx = shape->geo_params_[1];
		float cy = shape->geo_params_[2];
		float cz = shape->geo_params_[3];
		//std::cout<<"R:"<<R<<",cx:"<<cx<<",cy:"<<cy<<",cz:"<<cz<<std::endl;
		vec3 vec3_c ={{cx, cy, cz}};

		vec3 e_minus_c = vec3Minus(vec3_e, vec3_c);
		float d_mul_e_minus_c = vec3Mul(vec3_d, e_minus_c);
		float sq_d = vec3Mul(vec3_d, vec3_d);
		float sq_e_c = vec3Mul(e_minus_c, e_minus_c);
		//std::cout<<"d_mul_e_minus_c:"<<d_mul_e_minus_c<<",sq d:"<<sq_d<<",sq_e_c:"<<sq_e_c<<std::endl;

		float disciminant = pow(d_mul_e_minus_c,2) - sq_d *(sq_e_c - pow(R,2));

		if(disciminant<=1){
			//std::cout<<"false"<<std::endl;
			return 0;

		}else{
			float t = std::min( (-d_mul_e_minus_c- sqrt(disciminant))/sq_d,(-d_mul_e_minus_c+ sqrt(disciminant))/sq_d);

			return t;
		}

	}

	return 0;
}


