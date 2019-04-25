#include <algorithm>
#include <cstring>
#include <cmath>
#include <GL/glut.h>
#include <iostream>
#include <limits>

#include "TiffRead.hh"

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

#define debug_i 650
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
		TiffRead::has_image_=true;
		TiffRead::is_gray_image_ = false;
		TiffRead::image_width_ = Screen::nx_;
		TiffRead::image_length_ = Screen::ny_;
		TiffRead::photo_metric_=2;
		TiffRead::compression_ =1;
		TiffRead::x_resolution_numerator_ = 1;
		TiffRead::y_resolution_numerator_ = 1;
		TiffRead::x_resolution_denominator_ = 1;
		TiffRead::y_resolution_denominator_ = 1;
		TiffRead::resolution_unit_ = 1;
		return "Trace Done";
	}
	else{
		return  "too many parameters";
	}
}


vec3
Trace::getColorFromRay(Ray ray, float t0, float t1, bool is_reflect_ray, int i, int j){
	float smallest_t = std::numeric_limits<float>::infinity();
	Triangle tri(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

	Surface* render_object = &tri;
	bool hit = false;

	for(std::vector<Surface*>::iterator it = Trace::object_list_.begin();it != object_list_.end();++it){
		float t = intersect(ray, *it, t0, t1, i, j);

		if(t){
			hit = true;
			if(t<smallest_t){
				smallest_t = t;
				render_object = *it;
			}
		}
	}

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
			vec3 normalized_l =buildLightDir(*light_it, intersect_point);

			Ray shadow_ray(intersect_point.mat[0], intersect_point.mat[1],intersect_point.mat[2], normalized_l.mat[0], normalized_l.mat[1],normalized_l.mat[2]);

			for(std::vector<Surface*>::iterator other_it = Trace::object_list_.begin();other_it != object_list_.end();++other_it){
				if(strcmp((*other_it)->name_.c_str(),render_object->name_.c_str())){

					if(intersect(shadow_ray, *other_it, epsilon, T1, i, j)){
						in_shadow = true;
					}
				}
			}

			if(!in_shadow){
				float rr = render_object->color_params_[3];
				float rg = render_object->color_params_[4];
				float rb = render_object->color_params_[5];
				float pr = render_object->color_params_[9];
				float pg = render_object->color_params_[10];
				float pb = render_object->color_params_[11];
				float p = render_object->color_params_[12];
				float lr = (*light_it)->lr_;
				float lg = (*light_it)->lg_;
				float lb = (*light_it)->lb_;

				float cosine = vec3Mul(render_object->norm_, normalized_l);
				vec3 v = {{-ray.dir_x_, -ray.dir_y_, -ray.dir_z_}};
				vec3 normal_v = normal(v);
				vec3 h = normal(vec3Add(normal_v, normalized_l));

//				if(i == debug_i && j == debug_j){
//					std::cout<<"normal_v:"<<normal_v.mat[0]<<" "<<normal_v.mat[1]<<" "<<normal_v.mat[2]<<std::endl;
//					std::cout<<"l:"<<normalized_l.mat[0]<<" "<<normalized_l.mat[1]<<" "<<normalized_l.mat[2]<<std::endl;
//					std::cout<<"h:"<<h.mat[0]<<" "<<h.mat[1]<<" "<<h.mat[2]<<std::endl;
//				}
				float cosine_h = std::max((float)0, vec3Mul(render_object->norm_, h));
				if(strcmp(render_object->type_.c_str(),"triangle") && strcmp(render_object->type_.c_str(),"plane") ){

					total_light_r +=  (rr * lr * std::max((float)0, cosine) + pr * lr * pow(cosine_h, p));
					total_light_g +=  (rg * lg * std::max((float)0, cosine) + pg * lg * pow(cosine_h,p));
					total_light_b +=  (rb * lb * std::max((float)0, cosine) + pb * lb * pow(cosine_h,p));
				}
				else{

					total_light_r += (*light_it)->lr_  * rr * fabs(cosine);
					total_light_g += (*light_it)->lg_  * rg * fabs(cosine);
					total_light_b += (*light_it)->lb_  * rb * fabs(cosine);
				}

				//reflection
				if((render_object->reflected_) && (reflect_counter_<MAX_REFLECT)){

					Ray reflected_ray = buildReflectedRay(ray, render_object->norm_, intersect_point, i,j);
					reflect_counter_++;
					vec3 color = getColorFromRay(reflected_ray, epsilon, t1, true,i, j);

					total_reflected_r = render_object->color_params_[6]*color.mat[0];
					total_reflected_g = render_object->color_params_[7]*color.mat[1];
					total_reflected_b = render_object->color_params_[8]*color.mat[2];
				}
			}

		}

		color.mat[0] = std::min((float)1, std::max((float)0, color.mat[0]+ total_light_r + total_reflected_r));
		color.mat[1] = std::min((float)1, std::max((float)0, color.mat[1]+ total_light_g + total_reflected_g));
		color.mat[2] = std::min((float)1, std::max((float)0, color.mat[2]+ total_light_b + total_reflected_b));

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
		dir = normal(dir);


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


	return reflected_ray;
}

float
Trace::intersect(Ray ray, Surface* shape, float t0, float t1, int i, int j){// i, j are just for debugging

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

	float disciminant = pow(d_mul_e_minus_c,2) - sq_d *(sq_e_c - pow(R,2));

	if(disciminant<=1){
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


			int temp = 0;
			int* winner_flag = &temp;

			t = assignT(x_min, y_min, true, winner_flag, t0, t1);
			shape->norm_ = assignNorm(1,true, 2, true, winner_flag, ax,ay,az);
			int first_winner = *winner_flag;

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

			if(t == 0){
				t = assignT(x_max, y_max, false, winner_flag, t0, t1);
				shape->norm_ = assignNorm(1,false, 2, false, winner_flag, ax,ay,az);

				t = assignT(y_max, z_max, false, winner_flag,t0, t1);
				shape->norm_ = assignNorm(2,false, 3, false, winner_flag, ax,ay,az);
			}
		}
	}
	return t;
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

