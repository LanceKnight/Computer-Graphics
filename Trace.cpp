#include <algorithm>
#include <cstring>
#include <cmath>
#include <GL/glut.h>
#include <iostream>
#include <limits>

#include "Background.hh"
#include "Camera.hh"
#include "Ilight.hh"
#include "Orthocamera.hh"
#include "Ray.hh"
#include "Screen.hh"
#include "Sphere.hh"
#include "Trace.hh"
#include "Triangle.hh"

#include "3D.h"
#include "main.hh"
#include "Surface.hh"
#include "Util.hh"



std::vector<Surface *> Trace::object_list_;
std::vector<Ilight *> Trace::light_list_;

#define debug_i 350
#define debug_j 450
#define MAX_REFLECT 4
#define T0 0
#define T1 std::numeric_limits<float>::infinity(  )
#define epsilon 1
#define ROUNDING 0.03

int Trace::reflect_counter_ =0;

std::string
Trace::trace(std::vector<std::string> paramList){
	if(paramList.size()==0){

		for(int i = 0; i < Screen::nx_; i++){
			for(int j = 0; j < Screen::ny_; j++){
				Ray ray = buildCameraRay(i,j);
				reflect_counter_ = 0;

				vec3 color = getColorFromRay(ray, T0, T1, false, i, j);
				checkImage[i][j][0]= std::min((float)255, std::max((float)0, 255*color.mat[0]));
				checkImage[i][j][1]= std::min((float)255, std::max((float)0, 255*color.mat[1]));
				checkImage[i][j][2]= std::min((float)255, std::max((float)0, 255*color.mat[2]));

				checkImage[debug_i][debug_j][0]= 255;
				checkImage[debug_i][debug_j][1]= 255;
				checkImage[debug_i][debug_j][2]= 50;
			}
		}

		display();
		return "Trace Done";
	}
	else{
		return  "too many parameters";
	}
}


vec3
Trace::getColorFromRay(Ray ray, float t0, float t1, bool is_reflect_ray, int i, int j){
	float smallest_t = std::numeric_limits<float>::infinity();
	Triangle tri(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0.0, 0.0, 0.0);

	Surface* render_object = &tri;
	bool hit = false;

	for(std::vector<Surface*>::iterator it = Trace::object_list_.begin();it != object_list_.end();++it){

		//if(i==debug_i && j==debug_j){
			//std::cout<<"trapped:"<<i <<" "<<j<<std::endl;
//		std::cout<<" Ray:"<<std::endl;
//		std::cout<<"ori:"<<ray.ori_x_<<","<<ray.ori_y_<<","<<ray.ori_z_<<std::endl;
//		std::cout<<"dir:"<<ray.dir_x_<<","<<ray.dir_y_<<","<<ray.dir_z_<<std::endl;
		//}

		float t = intersect(ray, *it, t0, t1, i, j);


		if(t){
			hit = true;
			if(t<smallest_t){
				smallest_t = t;
				render_object = *it;
			}
		}
	}

//	if(i==debug_i && j ==debug_j){
//		std::cout<<"hit:"<<(bool)hit<<std::endl;
//		std::cout<<"current object:"<<render_object->name_<<std::endl;
//		std::cout<<"hit t:"<<smallest_t<<std::endl;
//	}

	if(hit){
		vec3 intersect_point = {{ray.ori_x_+smallest_t*ray.dir_x_,ray.ori_y_+smallest_t*ray.dir_y_,ray.ori_z_+smallest_t*ray.dir_z_,}};

		vec3 color ={{

				std::min((float)1, std::max((float)0, render_object->color_params_[0] )),
				std::min((float)1, std::max((float)0, render_object->color_params_[1] )),
				std::min((float)1, std::max((float)0, render_object->color_params_[2] ))

		}};


		float total_light_r = 0;
		float total_light_g = 0;
		float total_light_b = 0;
		float total_reflected_r = 0;
		float total_reflected_g = 0;
		float total_reflected_b = 0;
		bool in_shadow = false;

		for(std::vector<Ilight *>::iterator light_it = Trace::light_list_.begin();light_it != Trace::light_list_.end();++light_it){
			in_shadow = false;
			vec3 normalized_light =buildLightDir(*light_it, intersect_point);
			//reflect_counter_ = 0;
			Ray shadow_ray(intersect_point.mat[0], intersect_point.mat[1],intersect_point.mat[2], normalized_light.mat[0], normalized_light.mat[1],normalized_light.mat[2]);

			for(std::vector<Surface*>::iterator other_it = Trace::object_list_.begin();other_it != object_list_.end();++other_it){
				if(strcmp((*other_it)->name_.c_str(),render_object->name_.c_str())){

					if(intersect(shadow_ray, *other_it, epsilon, T1, i, j)){
						in_shadow = true;
					}
//					if(i==debug_i && j==debug_j){
//						std::cout<<"light:"<<(*light_it)->name_<<std::endl;
//						std::cout<<"other object:"<<(*other_it)->name_<<std::endl;
//						std::cout<<"shadow ray ori:"<<shadow_ray.ori_x_<<" "<<shadow_ray.ori_y_<<" "<< shadow_ray.ori_z_<<std::endl;
//						std::cout<<"shadow ray dir:"<<shadow_ray.dir_x_<<" "<<shadow_ray.dir_y_<<" "<< shadow_ray.dir_z_<<std::endl;
//						std::cout<<"single in shadow:"<<in_shadow<<std::endl;
//					}
				}
			}
			//if(i==debug_i && j ==debug_j ){
				//std::cout<<"total in_shadow:"<<in_shadow<<std::endl;
			//}

			if(!in_shadow){
				float rr = render_object->color_params_[3];
				float rg = render_object->color_params_[4];
				float rb = render_object->color_params_[5];
//				if(i==debug_i && j ==debug_j ){
//					std::cout<<"norm:            "<<render_object->norm_.mat[0]<<" "<<render_object->norm_.mat[1]<<" "<<render_object->norm_.mat[2]<<std::endl;
//					std::cout<<"normalized light:"<<normalized_light.mat[0]<<" "<<normalized_light.mat[1]<<" "<<normalized_light.mat[2]<<std::endl;
//					std::cout<<"object:"<<render_object->name_<<std::endl;
//					std::cout<<"type:"<<render_object->type_<<std::endl;
//					 std::cout<<"strcmp(render_object->type_.c_str(),'plane'):"<< strcmp(render_object->type_.c_str(),"plane")<<std::endl;
//				}
				float cosine = vec3Mul(render_object->norm_, normalized_light);
				if(strcmp(render_object->type_.c_str(),"triangle") && strcmp(render_object->type_.c_str(),"plane") ){
//					if(i==debug_i && j ==debug_j ){
//						std::cout<<"not triangle, not plane"<<std::endl;
//					}
					total_light_r +=  std::max((float)0,((*light_it)->lr_  * rr * cosine));
					total_light_g +=  std::max((float)0,((*light_it)->lg_  * rg * cosine));
					total_light_b +=  std::max((float)0,((*light_it)->lb_  * rb * cosine));
				}
				else{
//					if(i==debug_i && j ==debug_j ){
//						std::cout<<"It is triangle or plane"<<std::endl;
//					}
					total_light_r += (*light_it)->lr_  * rr * fabs(cosine);
					total_light_g += (*light_it)->lg_  * rg * fabs(cosine);
					total_light_b += (*light_it)->lb_  * rb * fabs(cosine);
				}

				//reflection
//				if(i==debug_i && j ==debug_j ){
//					std::cout<<"reflected?:"<<render_object->reflected_<<std::endl;
//				}

				if((render_object->reflected_) && (reflect_counter_<MAX_REFLECT)){

					Ray reflected_ray = buildReflectedRay(ray, render_object->norm_, intersect_point, i,j);
					reflect_counter_++;

					vec3 color = getColorFromRay(reflected_ray, epsilon, t1, true,i, j);

//					if(i==debug_i && j==debug_j){
//						Util::debug_head("Trace.cpp-reflection");
//						std::cout<<"first reflection counter:" <<reflect_counter_<<std::endl;
//						std::cout<<"intersect point:"<<intersect_point.mat[0]<<","<<intersect_point.mat[1]<<","<<intersect_point.mat[2]<<std::endl;
//						std::cout<<"norm:"<<render_object->norm_.mat[0]<<","<<render_object->norm_.mat[1]<<","<<render_object->norm_.mat[2]<<std::endl;
//						std::cout<<std::endl;
//						std::cout<<"original Ray:"<<std::endl;
//						std::cout<<"ori:"<<ray.ori_x_<<","<<ray.ori_y_<<","<<ray.ori_z_<<std::endl;
//						std::cout<<"dir:"<<ray.dir_x_<<","<<ray.dir_y_<<","<<ray.dir_z_<<std::endl;
//
//						std::cout<<std::endl;
//						std::cout<<"reflected Ray:"<<std::endl;
//						std::cout<<"ori:"<<reflected_ray.ori_x_<<","<<reflected_ray.ori_y_<<","<<reflected_ray.ori_z_<<std::endl;
//						std::cout<<"dir:"<<reflected_ray.dir_x_<<","<<reflected_ray.dir_y_<<","<<reflected_ray.dir_z_<<std::endl;
//
//						std::cout<<std::endl;
//						std::cout<<"color:"<<color.mat[0]<<","<<color.mat[1]<<","<<color.mat[2]<<std::endl;
//						Util::debug_tail();
//					}


					total_reflected_r = render_object->color_params_[6]*color.mat[0];
					total_reflected_g = render_object->color_params_[7]*color.mat[1];
					total_reflected_b = render_object->color_params_[8]*color.mat[2];
				}
			}

		}

		color.mat[0] = std::min((float)1, std::max((float)0, color.mat[0]+ total_light_r + total_reflected_r));
		color.mat[1] = std::min((float)1, std::max((float)0, color.mat[1]+ total_light_g + total_reflected_g));
		color.mat[2] = std::min((float)1, std::max((float)0, color.mat[2]+ total_light_b + total_reflected_b));


//		if(i==debug_i && j ==debug_j ){
//			std::cout<<std::endl;
//			Util::debug_head("Trace.cpp------Shape Info----");
//			std::cout<<"-------i "<<i<<"---j "<< j<<"-----"<<std::endl;
//
//			std::cout<<"shape name:"<<render_object->name_<<std::endl;
//			std::cout<<"shape color:"<< render_object-> color_params_[0]<<"," <<render_object->color_params_[1]<<","<<render_object->color_params_[2]<<std::endl;
//
//			std::cout<<"shape geometry:";
//			for(std::vector<float>::iterator k=(render_object->geo_params_).begin(); k!=(render_object->geo_params_).end(); k++){
//				std::cout<<*k<<",";
//			}
//			std::cout<<std::endl;
//
//			std::cout<<"shape reflected:"<<(bool)render_object->reflected_<<std::endl;
//
//			std::cout<<std::endl;
//			std::cout<<"total light:"<<total_light_r<<","<<total_light_g<<","<<total_light_b<<std::endl;
//
//			std::cout<<std::endl;
//			std::cout<<"color:"<<color.mat[0]<<","<<color.mat[1]<<","<<color.mat[2]<<std::endl;
//
//			std::cout<<std::endl;
//			std::cout<<"checkImage:"<<(int)checkImage[i][j][0]<<","<<(int)checkImage[i][j][1]<<","<<(int)checkImage[i][j][2]<<std::endl;
//			Util::debug_tail();
//		}

		return color;
	}
	else
	{
		vec3 color ={{0, 0, 0}};
		if(!is_reflect_ray){
			 color ={{Background::r_, Background::g_, Background::b_}};
		}else{
			reflect_counter_ = MAX_REFLECT;
		}

		return color;
	}
}




Ray
Trace::buildCameraRay(int i, int j){

	if(Camera::perspect_){
		vec3 gz = {{Camera::ex_-Camera::gx_,Camera::ey_-Camera::gy_,Camera::ez_-Camera::gz_}};

		vec3 up = {{Camera::ux_, Camera::uy_, Camera::uz_}};

		vec3 v = normal(up);

		vec3 u = normal(vec3Cross(v, gz));

		vec3 w = normal(vec3Cross(u, v));

		float x_convert = (float)(j)*((float)Camera::bu_-(float)Camera::au_)/(float)(Screen::nx_-1)+(float)Camera::au_;
		float y_convert = (float)(i)*((float)Camera::bv_-(float)Camera::av_)/(float)(Screen::ny_-1)+(float)Camera::av_;

		vec3 dir = vec3Add(vec3Add(vec3NumMul(x_convert, u), vec3NumMul( y_convert,v)), vec3NumMul(-Camera::s_, w));
		//vec3 dir = vec3Add(vec3Add(vec3NumMul(j-Camera::gx_, u), vec3NumMul(i-Camera::gy_, v)), vec3NumMul(-Camera::s_, w));
		dir = normal(dir);
//
//		if(i==debug_i && j ==debug_j){
//			std::cout<<"x_convert:"<<x_convert<<std::endl;
//			std::cout<<"y_convert:"<<y_convert<<std::endl;
//			std::cout<<"gz:"<<gz.mat[0]<<","<<gz.mat[1]<<","<<gz.mat[2]<<std::endl;
//			std::cout<<"v:"<<v.mat[0]<<","<<v.mat[1]<<","<<v.mat[2]<<std::endl;
//			std::cout<<"u:"<<u.mat[0]<<","<<u.mat[1]<<","<<u.mat[2]<<std::endl;
//			std::cout<<"w:"<<w.mat[0]<<","<<w.mat[1]<<","<<w.mat[2]<<std::endl;
//			std::cout<<"perspective: eye:"<<Camera::ex_<<","<< Camera::ey_<<","<<Camera::ez_<<std::endl;
//			std::cout<<"             dir:"<<dir.mat[0]<<","<<dir.mat[1]<<","<<dir.mat[2]<<std::endl;
//		}

		Ray ray( Camera::ex_, Camera::ey_, Camera::ez_ ,dir.mat[0], dir.mat[1] , dir.mat[2]);
		return ray;
	}
	else{//default is orthographic view
		Ray ray(j, i, -1, 0, 0 , -1 );
		return ray;
	}

}

vec3
Trace::buildLightDir(const Ilight* ilight, const vec3 intersect_point){
	vec3 light ={{-ilight->dx_, -ilight->dy_, -ilight->dz_}};
	//vec3 vec3_l = vec3Minus(light, intersect_point);
	//vec3 normalized_light = normal(vec3_l);
	vec3 normalized_light = normal(light);
	return normalized_light;

}



Ray
Trace::buildReflectedRay(Ray ray, vec3 norm, vec3 point, int i, int j){// i, j are just for debugging

	vec3 d = {{ray.dir_x_,ray.dir_y_,ray.dir_z_}};

	vec3 normalized_d = normal(d);

	float d_n = 2*vec3Mul(normalized_d, norm);

	vec3 second_term = vec3NumMul(d_n, norm);

	vec3 r = vec3Minus(normalized_d,  second_term );

	r = normal(r);
	Ray reflected_ray(point.mat[0],point.mat[1],point.mat[2],r.mat[0], r.mat[1],r.mat[2]);

//	if(i == debug_i && j == debug_j){
//		Util::debug_head("Trace.cpp-buildReflectedRay()");
//		std::cout<<"d_n:"<<d_n<<std::endl;
//		std::cout<<"norm:"<<norm.mat[0]<<","<<norm.mat[1]<<","<<norm.mat[2]<<std::endl;
//		std::cout<<"second term:"<<second_term.mat[0]<<","<<second_term.mat[1]<<","<<second_term.mat[2]<<std::endl;
//		std::cout<<"reflected Ray:"<<std::endl;
//		std::cout<<"ori:"<<reflected_ray.ori_x_<<","<<reflected_ray.ori_y_<<","<<reflected_ray.ori_z_<<std::endl;
//		std::cout<<"dir:"<<reflected_ray.dir_x_<<","<<reflected_ray.dir_y_<<","<<reflected_ray.dir_z_<<std::endl;
//		Util::debug_tail();
//	}

	return reflected_ray;
}

float
Trace::intersect(Ray ray, Surface* shape, float t0, float t1, int i, int j){// i, j are just for debugging

//	if(i==debug_i && j==debug_j){
//		std::cout<<" Ray:"<<std::endl;
//		std::cout<<"ori:"<<ray.ori_x_<<","<<ray.ori_y_<<","<<ray.ori_z_<<std::endl;
//		std::cout<<"dir:"<<ray.dir_x_<<","<<ray.dir_y_<<","<<ray.dir_z_<<std::endl;
//	}

	if(!strcmp(shape->type_.c_str(), "triangle")){
		return intersectTriangle(shape, ray, t0, t1 );
	}
	else if(!strcmp(shape->type_.c_str(), "sphere")){
		return intersectSphere(shape, ray, t0, t1, i, j);
	}
	else if(!strcmp(shape->type_.c_str(), "box")){
		return intersectBox(shape, ray, t0, t1, i, j );
	}
	else if(!strcmp(shape->type_.c_str(), "plane")){
		return intersectPlane(shape, ray, t0, t1);
	}
	return 0;
}


float
Trace::intersectTriangle(Surface* shape, Ray ray, float t0, float t1){
	float dx = ray.dir_x_;
	float dy = ray.dir_y_;
	float dz = ray.dir_z_;
	float ex = ray.ori_x_;
	float ey = ray.ori_y_;
	float ez = ray.ori_z_;


	float ax = shape->geo_params_[0];
	float ay = shape->geo_params_[1];
	float az = shape->geo_params_[2];
	float bx = shape->geo_params_[3];
	float by = shape->geo_params_[4];
	float bz = shape->geo_params_[5];
	float cx = shape->geo_params_[6];
	float cy = shape->geo_params_[7];
	float cz = shape->geo_params_[8];

	vec3 vertex_a = {{ax,ay,az}};
	vec3 vertex_b = {{bx,by,bz}};
	vec3 vertex_c = {{cx,cy,cz}};

	mat3 A = {
			{ {ax-bx    , ax-cx     , dx     },
					{ay-by    , ay-cy     , dy     },
					{az-bz    , az-cz     , dz     }  },
	};
	float deter_A = determinant(A);
	mat3 t_mat = {{
			     {ax-bx    , ax-cx     , ax-ex     },
			     {ay-by    , ay-cy     , ay-ey     },
			     {az-bz    , az-cz     , az-ez     }
	             },};

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


	vec3 norm = vec3Cross(vec3Minus(vertex_a, vertex_b),vec3Minus(vertex_a, vertex_c));
	vec3 normalized_norm = normal(norm);
	//std::cout<<"norm:"<<normalized_norm.mat[0]<<","<<normalized_norm.mat[1]<<","<<normalized_norm.mat[2]<<std::endl;

	shape->norm_ = normalized_norm ;
	return t;

}

float
Trace::intersectSphere(Surface* shape, Ray ray, float t0, float t1, int i, int j){
	float dx = ray.dir_x_;
	float dy = ray.dir_y_;
	float dz = ray.dir_z_;
	float ex = ray.ori_x_;
	float ey = ray.ori_y_;
	float ez = ray.ori_z_;
	vec3 vec3_e = {{ex, ey, ez}};
	vec3 vec3_d = {{dx, dy, dz}};

	float R =  shape->geo_params_[0];
	float cx = shape->geo_params_[1];
	float cy = shape->geo_params_[2];
	float cz = shape->geo_params_[3];


	vec3 vec3_c ={{cx, cy, cz}};

	vec3 e_minus_c = vec3Minus(vec3_e, vec3_c);
	float d_mul_e_minus_c = vec3Mul(vec3_d, e_minus_c);
	float sq_d = vec3Mul(vec3_d, vec3_d);
	float sq_e_c = vec3Mul(e_minus_c, e_minus_c);
	//std::cout<<"d_mul_e_minus_c:"<<d_mul_e_minus_c<<",sq d:"<<sq_d<<",sq_e_c:"<<sq_e_c<<std::endl;

	float disciminant = pow(d_mul_e_minus_c,2) - sq_d *(sq_e_c - pow(R,2));
//
//	if(i == debug_i && j == debug_j &&  !strcmp(shape->name_.c_str(), "sphere 14")){
//		std::cout<<"===============Sphere=============="<<std::endl;
//		std::cout<<"shape:"<<shape->name_<<std::endl;
//		std::cout<<"R:"<<R<<",cx:"<<cx<<",cy:"<<cy<<",cz:"<<cz<<std::endl;
//		std::cout <<"ray----------------ex:"<<ex<<" ey:"<<ey<<" ez:"<<ez<<" dx:"<<dx<<" dy:"<<dy<<" dz:"<<dz<<std::endl;
//		std::cout<<"e_minus_c"<<e_minus_c.mat[0]<<" "<<e_minus_c.mat[1]<<" "<<e_minus_c.mat[2]<<std::endl;
//		std::cout<<"discriminant:"<<disciminant<<std::endl;
//
//	}
	if(disciminant<=1){
		//std::cout<<"false"<<std::endl;
		return 0;

	}else{

		float t;
		float minus_term = (-d_mul_e_minus_c- sqrt(disciminant))/sq_d;
		float add_term = (-d_mul_e_minus_c+ sqrt(disciminant))/sq_d;

		if( minus_term>t0){

			t = std::min( minus_term, add_term);
		}
		else if(add_term>t0){

			t = add_term;
		}
		else{
			return 0;
		}
		vec3 p = {{ex+t*dx, ey+t*dy, ez+t*dz}};
		vec3 p_minus_c = vec3Minus(p, vec3_c);
		vec3 normlized_p_minus_c = {{p_minus_c.mat[0]/R, p_minus_c.mat[1]/R, p_minus_c.mat[2]/R}};
		shape->norm_ = normlized_p_minus_c;
//		if(i == debug_i && j == debug_j){
//			std::cout<<"hit object:"<<shape->name_<<std::endl;
//		}
		return t;
	}

}

float
Trace::intersectBox(Surface* shape, Ray ray, float t0, float t1, int i, int j ){// i, j for debug
	float ux = shape->geo_params_[0];
	float uy = shape->geo_params_[1];
	float uz = shape->geo_params_[2];
	float vx = shape->geo_params_[3];
	float vy = shape->geo_params_[4];
	float vz = shape->geo_params_[5];

	float x_min=0;
	float x_max=0;
	float y_min=0;
	float y_max=0;
	float z_min=0;
	float z_max=0;
	float ax = 1.0/ray.dir_x_;
	float ay = 1.0/ray.dir_y_;
	float az = 1.0/ray.dir_z_;
//	if(i == debug_i && j == debug_j){
//		std::cout<<"dir_x:"<<ray.dir_x_<<std::endl;
//		std::cout<<"dir_y:"<<ray.dir_y_<<std::endl;
//		std::cout<<"dir_z:"<<ray.dir_z_<<std::endl;
//		std::cout<<"ax:"<<ax<<std::endl;
//		std::cout<<"ay:"<<ay<<std::endl;
//		std::cout<<"az:"<<az<<std::endl;
//	}

	float t = std::numeric_limits<float>::infinity();
	if(ax >=0){
		x_min = ax*(ux-ray.ori_x_);
		x_max = ax*(vx-ray.ori_x_);
	}
	else{
		x_min = ax*(vx-ray.ori_x_);
		x_max = ax*(ux-ray.ori_x_);
	}

	if(ay >=0){
		y_min = ay*(uy-ray.ori_y_);
		y_max = ay*(vy-ray.ori_y_);
	}
	else{
		y_min = ay*(vy-ray.ori_y_);
		y_max = ay*(uy-ray.ori_y_);
	}

	if(az <=0){
		z_min = az*(uz-ray.ori_z_);
		z_max = az*(vz-ray.ori_z_);
	}
	else{
		z_min = az*(vz-ray.ori_z_);
		z_max = az*(uz-ray.ori_z_);
	}


	if(x_min>y_max || y_min>x_max){
		return false;
	}
	else{
		if(y_min>z_max || z_min>y_max){
			return false;
		}
		else{
			if(x_min>z_max || z_min>x_max){
				return false;
			}

//			if(i == debug_i && j == debug_j){
//				std::cout<<"+++++++++++++++++ARENA++++++++++++++++++++++++++++++:"<<t<<std::endl;
//
//				std::cout<<"x_min:"<<x_min<<" x_max:"<<x_max<<std::endl;
//				std::cout<<"y_min:"<<y_min<<" y_max:"<<y_max<<std::endl;
//				std::cout<<"z_min:"<<z_min<<" z_max:"<<z_max<<std::endl;
//			}

			int temp = 0;
			int* winner_flag = &temp;

			t = assignT(x_min, y_min, true, winner_flag, t0, t1);
			shape->norm_ = assignNorm(1,true, 2, true, winner_flag, ax,ay,az);
			int first_winner = *winner_flag;


//			if(i == debug_i && j == debug_j){
//				std::cout<<"t:"<<t<<std::endl;
//				std::cout<<"norm_flag:"<<*winner_flag<<std::endl;
//				std::cout<<"norm:"<<shape->norm_.mat[0]<<" "<<shape->norm_.mat[1]<<" "<<shape->norm_.mat[2]<<std::endl;
//			}

			t = assignT(t, z_min, true, winner_flag, t0, t1);
			if(*winner_flag==1){
				if(first_winner==1){
					shape->norm_ = assignNorm(1,true, 3, true, winner_flag, ax,ay,az);
				}
				else if(first_winner ==2){
					shape->norm_ = assignNorm(2,true, 3, true, winner_flag, ax,ay,az);
				}
			}
			else if(*winner_flag == 2){
				shape->norm_ = assignNorm(2,true, 3, true, winner_flag, ax,ay,az);
			}
//			if(i == debug_i && j == debug_j){
//				std::cout<<"--------------------"<<std::endl;
//					//				t = 1246.12;
//				std::cout<<"t:"<<t<<std::endl;
//				std::cout<<"norm_flag:"<<*winner_flag<<std::endl;
//				std::cout<<"norm:"<<shape->norm_.mat[0]<<" "<<shape->norm_.mat[1]<<" "<<shape->norm_.mat[2]<<std::endl;
//			}

			if(t == 0){
				t = assignT(x_max, y_max, false, winner_flag, t0, t1);
				shape->norm_ = assignNorm(1,false, 2, false, winner_flag, ax,ay,az);
//				if(i == debug_i && j == debug_j){
//					std::cout<<"t:"<<t<<std::endl;
//					std::cout<<"norm_flag:"<<*winner_flag<<std::endl;
//					std::cout<<"norm:"<<shape->norm_.mat[0]<<" "<<shape->norm_.mat[1]<<" "<<shape->norm_.mat[2]<<std::endl;
//				}

				t = assignT(y_max, z_max, false, winner_flag,t0, t1);
				shape->norm_ = assignNorm(2,false, 3, false, winner_flag, ax,ay,az);
//				if(i == debug_i && j == debug_j){
//					std::cout<<"t:"<<t<<std::endl;
//					std::cout<<"norm_flag:"<<*winner_flag<<std::endl;
//					std::cout<<"norm:"<<shape->norm_.mat[0]<<" "<<shape->norm_.mat[1]<<" "<<shape->norm_.mat[2]<<std::endl;
//				}
			}

			float intersect_x = ray.ori_x_+t*ray.dir_x_;
			float intersect_y = ray.ori_y_+t*ray.dir_y_;
			float intersect_z = ray.ori_z_+t*ray.dir_z_;
//			if(i == debug_i && j == debug_j){
//				std::cout<<"intersect_x:"<<intersect_x<<std::endl;
//				std::cout<<"intersect_y:"<<intersect_y<<std::endl;
//				std::cout<<"intersect_z:"<<intersect_z<<std::endl;
//
//
//				std::cout<<"t:"<<t<<std::endl;
//			}


/*
			float tx = 0;
			float ty = 0;
			float tz = 0;
			if(x_min>0){
				tx = x_min;
			}
			else if(x_max>0){
				tx = x_max;
			}
			else{
				tx = -1;
			}

			if(y_min>0){
				ty = y_min;
			}
			else if(y_max>0){
				ty = y_max;
			}
			else{
				ty = -1;
			}

			if(z_min>0){
				tz = z_min;
			}
			else if(z_max>0){
				tz = z_max;
			}
			else{
				tz = -1;
			}

			if(i == debug_i && j == debug_j){
				std::cout<<"x_min:"<<x_min<<" x_max:"<<x_max<<std::endl;
				std::cout<<"y_min:"<<y_min<<" y_max:"<<y_max<<std::endl;
				std::cout<<"z_min:"<<z_min<<" z_max:"<<z_max<<std::endl;
				std::cout<<"tx:"<<tx<<std::endl;
				std::cout<<"ty:"<<ty<<std::endl;
				std::cout<<"tz:"<<tz<<std::endl;
			}

			if(tx>ty && ty >0){
				t = ty;
				if(ty>=0){
					shape->norm_ = {{0,0,1}};
				}
				else{
					shape->norm_ = {{0,0,-1}};
				}
			}
			else if(tx>0){

				t= tx;
				if(tx>=0){
					shape->norm_ = {{1,0,0}};
				}
				else{
					shape->norm_ = {{-1,0,0}};
				}

			}
			if(t>tz && tz>0){
				t= tz;
				if(tz>=0){
					shape->norm_ = {{0,0,1}};
				}
				else{
					shape->norm_ = {{0,0,-1}};
				}
				return t;

			}
			else if(t<tz){
				return t;
			}
			else{
				return false;
			}
			*/

		}
	}
	return t;

/*
	Triangle frontA(ux, uy, uz, ux, vy, uz, vx, uy, uz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0);
	float frontA_t = intersectTriangle(&frontA, ray, t0, t1);

	Triangle frontB(vx, vy, uz, ux, vy, uz, vx, uy, uz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0);
	float frontB_t = intersectTriangle(&frontB, ray, t0, t1);

	Triangle backA(ux, uy, vz, ux, vy, vz, vx, uy, vz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0);
	float backA_t = intersectTriangle(&backA, ray, t0, t1);

	Triangle backB(vx, vy, vz, ux, vy, vz, vx, uy, vz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0);
	float backB_t = intersectTriangle(&backB, ray, t0, t1);

	Triangle leftA(ux, uy, uz, ux, vy, uz, ux, uy, vz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0);
	float leftA_t = intersectTriangle(&leftA, ray, t0, t1);

	Triangle leftB(ux, vy, vz, ux, vy, uz, ux, uy, vz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0);
	float leftB_t = intersectTriangle(&leftB, ray, t0, t1);

	Triangle rightA(vx, uy, uz, vx, vy, uz, vx, uy, vz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0);
	float rightA_t = intersectTriangle(&rightA, ray, t0, t1);

	Triangle rightB(vx, vy, vz, vx, vy, uz, vx, uy, vz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0);
	float rightB_t = intersectTriangle(&rightB, ray, t0, t1);

	Triangle topA(ux, vy, uz, ux, vy, vz, vx, vy, uz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0);
	float topA_t = intersectTriangle(&topA, ray, t0, t1);

	Triangle topB(vx, vy, vz, ux, vy, vz, vx, vy, uz,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0);
	float topB_t = intersectTriangle(&topB, ray, t0, t1);

	Triangle bottomA(ux, uy, uz, ux, uy, vz, vx, uy, uz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0);
	float bottomA_t = intersectTriangle(&bottomA, ray, t0, t1);

	Triangle bottomB(vx, uy, vz, ux, uy, vz, vx, uy, uz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0);
	float bottomB_t = intersectTriangle(&bottomB, ray, t0, t1);

	//A and B has at most one non-negative value, the following codes will get the positive value if it exists.
	float front_t = frontB_t?frontB_t:frontA_t;
	float back_t = backB_t?backB_t:backA_t;
	float left_t = leftB_t?leftB_t:leftA_t;
	float right_t = rightB_t?rightB_t:rightA_t;
	float top_t = topB_t?topB_t:topA_t;
	float bottom_t = bottomB_t?bottomB_t:bottomA_t;


//
//	if(i == debug_i && j == debug_j){
//		std::cout<< "front_t:"<<front_t<<std::endl;
//		std::cout<< "back_t:"<<back_t<<std::endl;
//		std::cout<< "left_t:"<<left_t<<std::endl;
//		std::cout<< "right_t:"<<right_t<<std::endl;
//		std::cout<< "top_t:"<<top_t<<std::endl;
//		std::cout<< "bottom_t:"<<bottom_t<<std::endl;
//	}


	float t = std::numeric_limits<float>::infinity();
	bool hit = false;
	if(front_t!=0 && t > front_t){
		t = front_t;
		shape->norm_ = {{0,0,1}};
		hit = true;
	}

	if(back_t!=0 && t > back_t){
		t = back_t;
		shape->norm_ = {{0,0,-1}};
		hit = true;
	}
	if(left_t!=0 && t > left_t){
		t = left_t;
		shape->norm_ = {{-1,0,0}};
		hit = true;
	}
	if(right_t!=0 && t > right_t){
		t = right_t;
		shape->norm_ = {{1,0,0}};
		hit = true;
	}
	if(top_t!=0 && t > top_t){
		t = top_t;
		shape->norm_ = {{0,1,0}};
		hit = true;
	}
	if(bottom_t!=0 && t > bottom_t){
		t = bottom_t;
		shape->norm_ = {{0,-1,0}};
		hit = true;
	}
	if(i == debug_i && j == debug_j){
		std::cout<<"final t:"<<t<<std::endl;
	}
	if(	hit == true){
		return t;
	}
	else{
		return 0;
	}
*/

}

float
Trace::assignT(float a, float b, bool is_min, int* norm_flag, float t0, float t1){
	if((a<=t0 && b<=t0 )||(a>=t1&& b>=t1)){
		*norm_flag = 0;
		return 0;
	}
	else{
		if(a<=t0){
			*norm_flag = 2;
			return b;
		}
		else if(b<=t0){
			*norm_flag = 1;
			return a;
		}
		else{

			if((a<b) == is_min){
				*norm_flag = 2;
				return b;
			}
			else{
				*norm_flag = 1;
				return a;
			}
		}
	}
}

vec3
Trace::assignNorm(int xyz_1, bool is_min_1, int xyz_2, bool is_min_2, int * norm_flag, float ax, float ay, float az){
	vec3 result = {{0,0,0}};

	switch(*norm_flag){
	case 0://no norm
		break;
	case 1://get former norm
		result = assignNorm2(xyz_1, is_min_1, ax,ay,az);
		break;
	case 2:// get later norm
		result = assignNorm2(xyz_2, is_min_2, ax,ay,az);
		break;
	default:
		std::cout<<"error input in switch assignNorm!"<<std::endl;
	}
	return result;
}


vec3
Trace::assignNorm2(int xyz, bool is_min, float ax, float ay, float az){
	vec3 result = {{0,0,0}};
	switch(xyz){
	case 1:
		if(is_min != (ax>=0)){
			result = {{1,0,0}};
		}
		else{
			result = {{-1,0,0}};
		}
		break;
	case 2:
		if(is_min != (ay>=0)){
			result = {{0,1,0}};
		}
		else{
			result = {{0,-1,0}};
		}
		break;
	case 3:
		if(is_min == true){// in case of min
			if(az<=0){
				result = {{0,0, 1}};
			}
			else{
				result = {{0,0,-1}};
			}
		}
		else{ // in case of max
			if(az<=0){
				result = {{0,0,-1}};
			}
			else{
				result = {{0,0, 1}};
			}
		}

		break;
	default:
		std::cout<<"error in assignNorm2"<<std::endl;

	}
	return result;
}

float
Trace::intersectPlane(Surface* shape, Ray ray, float t0, float t1){
	float dx = ray.dir_x_;
	float dy = ray.dir_y_;
	float dz = ray.dir_z_;
	float ex = ray.ori_x_;
	float ey = ray.ori_y_;
	float ez = ray.ori_z_;
	vec3 vec3_e = {{ex, ey, ez}};
	vec3 vec3_d = {{dx, dy, dz}};

	float nx = shape->geo_params_[0];
	float ny = shape->geo_params_[1];
	float nz = shape->geo_params_[2];
	float px = shape->geo_params_[3];
	float py = shape->geo_params_[4];
	float pz = shape->geo_params_[5];

	vec3 p1 = {{px,py,pz}};
	vec3 n = {{nx,ny,nz}};
	vec3 p1_e = vec3Minus(p1, vec3_e);
	float up_term = vec3Mul(p1_e, n);
	float down_term = vec3Mul(vec3_d, n);
	float t = up_term/down_term;

	if(t>t0 && t<t1){
		return t;
	}

	return 0;

}

