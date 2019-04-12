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
#include "Shape.hh"
#include "Sphere.hh"
#include "Trace.hh"
#include "Triangle.hh"

#include "3D.h"
#include "main.hh"
#include "Util.hh"



std::vector<Shape *> Trace::object_list_;
std::vector<Ilight *> Trace::light_list_;

#define debug_i 700
#define debug_j 460

int Trace::reflect_counter_=0;

std::string
Trace::trace(std::vector<std::string> paramList){


	if(paramList.size()==0){

		if(Orthocamera::ortho_){

			for(int i = 0; i < Screen::nx_; i++){
				for(int j = 0; j < Screen::ny_; j++){

					Ray ray = buildCameraRay(i,j);
					vec3 color = getColorFromRay(ray, i, j);
					checkImage[i][j][0]= std::min((float)255, std::max((float)0, color.mat[0]));
					checkImage[i][j][1]= std::min((float)255, std::max((float)0, color.mat[1]));
					checkImage[i][j][2]= std::min((float)255, std::max((float)0, color.mat[2]));
/*
					bool hit = false;
					float smallest_t = std::numeric_limits<float>::infinity();
					Triangle tri(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

					Shape* render_object = &tri;
					for(std::vector<Shape*>::iterator it = Trace::object_list_.begin();it != object_list_.end();++it){

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
						float total_light_r = 0;
						float total_light_g = 0;
						float total_light_b = 0;
						vec3 intersect_point = {{ray.ori_x_+smallest_t*ray.dir_x_,ray.ori_y_+smallest_t*ray.dir_y_,ray.ori_z_+smallest_t*ray.dir_z_,}};
						for(std::vector<Ilight *>::iterator light_it = Trace::light_list_.begin();light_it != Trace::light_list_.end();++light_it){
							vec3 light ={{(*light_it)->dx_, (*light_it)->dy_, (*light_it)->dz_}};

							vec3 vec3_l = vec3Minus(light, intersect_point);
							vec3 normalized_light = normalize(vec3_l);


							float rr = 0;
							float rg = 0;
							float rb = 0;
							std::string flag;
							if(!strcmp(render_object->type_.c_str(),"triangle")){
								flag = "tri";
								rr = render_object->color_params_[3];
								rg = render_object->color_params_[4];
								rb = render_object->color_params_[5];
							}
							else if (!strcmp(render_object->type_.c_str(),"sphere")){
								flag = "sphere";
								rr = render_object->color_params_[3];
								rg = render_object->color_params_[4];
								rb = render_object->color_params_[5];
							}

							//std::cout<<flag<<std::endl;
							//std::cout<<"rr:"<<rr<<",rg:"<<rg<<",rb:"<<rb<<std::endl;

							if(i==debug_i && j ==debug_j ){
								Util::debug_head("Trace.cpp");
								std::cout<<"-------i "<<i<<"---j "<< j<<"-----"<<std::endl;
								std::cout<<"------Light Info----"<<std::endl;
								std::cout<<"light name:"<<(*light_it)->name_<<std::endl;
								std::cout<<"light position:"<<(*light_it)->dx_<<","<<(*light_it)->dy_<<","<< (*light_it)->dz_<<std::endl;
								std::cout<<"light color:"<<(*light_it)->lr_<<","<<(*light_it)->lg_<<","<< (*light_it)->lb_<<std::endl;
								std::cout<<"cos:"<<vec3Mul(render_object->norm_, normalized_light)<<std::endl;
								std::cout<<"render_object norm:"<<(render_object->norm_).mat[0]<<","<<(render_object->norm_).mat[1]<<","<<(render_object->norm_).mat[2]<<std::endl;

								Util::debug_tail();
							}

							total_light_r +=  ((*light_it)->lr_  *  vec3Mul(render_object->norm_, normalized_light));
							total_light_g +=  ((*light_it)->lg_  *  vec3Mul(render_object->norm_, normalized_light));
							total_light_b +=  ((*light_it)->lb_  *  vec3Mul(render_object->norm_, normalized_light));



						}

						//reflection
						float total_reflected_r = 0;
						float total_reflected_g = 0;
						float total_reflected_b = 0;

						if(render_object->reflected_){
							Ray ray = buildReflectedRay(ray, render_object->norm_, intersect_point);

						}


						checkImage[i][j][0]= std::min((float)255, std::max((float)0, render_object->color_params_[0] + total_light_r + total_reflected_r));
						checkImage[i][j][1]= std::min((float)255, std::max((float)0, render_object->color_params_[1] + total_light_g + total_reflected_g));
						checkImage[i][j][2]= std::min((float)255, std::max((float)0, render_object->color_params_[2] + total_light_b + total_reflected_b));

						if(i==debug_i && j ==debug_j ){
							std::cout<<std::endl;
							Util::debug_head("Trace.cpp");
							std::cout<<"-------i "<<i<<"---j "<< j<<"-----"<<std::endl;
							std::cout<<"------Shape Info----"<<std::endl;
							std::cout<<"shape name:"<<render_object->name_<<std::endl;
							std::cout<<"shape color:"<< render_object-> color_params_[0]<<"," <<render_object->color_params_[1]<<","<<render_object->color_params_[2]<<std::endl;
							std::cout<<"shape geometry:";
							for(std::vector<float>::iterator k=(render_object->geo_params_).begin(); k!=(render_object->geo_params_).end(); k++){
								std::cout<<*k<<",";
							}
							std::cout<<std::endl;

							std::cout<<std::endl;
							std::cout<<"total light:"<<total_light_r<<","<<total_light_g<<","<<total_light_b<<std::endl;

							std::cout<<std::endl;
							std::cout<<"checkImage:"<<(int)checkImage[i][j][0]<<","<<(int)checkImage[i][j][1]<<","<<(int)checkImage[i][j][2]<<std::endl;
							Util::debug_tail();
						}

					}
					else
					{
						checkImage[i][j][0]= Background::r_;
						checkImage[i][j][1]= Background::g_;
						checkImage[i][j][2]= Background::b_;
					}

					*/
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

Ray
Trace::buildReflectedRay(Ray ray, vec3 norm, vec3 point){

	vec3 d = {{ray.ori_x_+ray.dir_x_,ray.ori_y_+ray.dir_y_,ray.ori_z_+ray.dir_z_}};
	vec3 r = vec3Minus(d, vec3NumMul(2*vec3Mul(d, norm), norm)  );

	Ray reflected_ray(point.mat[0],point.mat[1],point.mat[2],r.mat[0], r.mat[1],r.mat[2]);
	return reflected_ray;
}

vec3
Trace::getColorFromRay(Ray ray, int i, int j){
	float smallest_t = std::numeric_limits<float>::infinity();
	Triangle tri(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

	Shape* render_object = &tri;
	bool hit = false;
	for(std::vector<Shape*>::iterator it = Trace::object_list_.begin();it != object_list_.end();++it){

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
		float total_light_r = 0;
		float total_light_g = 0;
		float total_light_b = 0;
		vec3 intersect_point = {{ray.ori_x_+smallest_t*ray.dir_x_,ray.ori_y_+smallest_t*ray.dir_y_,ray.ori_z_+smallest_t*ray.dir_z_,}};
		for(std::vector<Ilight *>::iterator light_it = Trace::light_list_.begin();light_it != Trace::light_list_.end();++light_it){
			vec3 light ={{(*light_it)->dx_, (*light_it)->dy_, (*light_it)->dz_}};

			vec3 vec3_l = vec3Minus(light, intersect_point);
			vec3 normalized_light = normalize(vec3_l);

			float rr = 0;
			float rg = 0;
			float rb = 0;
			std::string flag;
			if(!strcmp(render_object->type_.c_str(),"triangle")){
				flag = "tri";
				rr = render_object->color_params_[3];
				rg = render_object->color_params_[4];
				rb = render_object->color_params_[5];
			}
			else if (!strcmp(render_object->type_.c_str(),"sphere")){
				flag = "sphere";
				rr = render_object->color_params_[3];
				rg = render_object->color_params_[4];
				rb = render_object->color_params_[5];
			}

			if(i==debug_i && j ==debug_j ){
				Util::debug_head("Trace.cpp");
				std::cout<<"-------i "<<i<<"---j "<< j<<"-----"<<std::endl;
				std::cout<<"------Light Info----"<<std::endl;
				std::cout<<"light name:"<<(*light_it)->name_<<std::endl;
				std::cout<<"light position:"<<(*light_it)->dx_<<","<<(*light_it)->dy_<<","<< (*light_it)->dz_<<std::endl;
				std::cout<<"light color:"<<(*light_it)->lr_<<","<<(*light_it)->lg_<<","<< (*light_it)->lb_<<std::endl;
				std::cout<<"cos:"<<vec3Mul(render_object->norm_, normalized_light)<<std::endl;
				std::cout<<"render_object norm:"<<(render_object->norm_).mat[0]<<","<<(render_object->norm_).mat[1]<<","<<(render_object->norm_).mat[2]<<std::endl;

				Util::debug_tail();
			}
			total_light_r +=  ((*light_it)->lr_  *  vec3Mul(render_object->norm_, normalized_light));
			total_light_g +=  ((*light_it)->lg_  *  vec3Mul(render_object->norm_, normalized_light));
			total_light_b +=  ((*light_it)->lb_  *  vec3Mul(render_object->norm_, normalized_light));

		}

		//reflection
		float total_reflected_r = 0;
		float total_reflected_g = 0;
		float total_reflected_b = 0;

		if(render_object->reflected_){
			//std::cout<<"i:"<<i<<",j:"<<j<<std::endl;
			if(i==debug_i && j ==debug_j ){
				Util::debug_head("Trace.cpp");
				std::cout<<"-------i "<<i<<"---j "<< j<<"-----"<<std::endl;
				std::cout<<"------Reflection Info----"<<std::endl;
				std::cout<<"object name:"<<render_object->name_<<std::endl;
			}
			Ray reflected_ray = buildReflectedRay(ray, render_object->norm_, intersect_point);
			if(reflect_counter_<4){
				std::cout<<"reflect_counter:"<<reflect_counter_<<std::endl;
				vec3 color = getColorFromRay(reflected_ray,i,j);
				total_reflected_r = render_object->color_params_[0]*color.mat[0];
				total_reflected_g = render_object->color_params_[1]*color.mat[1];
				total_reflected_b = render_object->color_params_[2]*color.mat[2];


				std::cout<<"color:"<<color.mat[0]<<","<<color.mat[1]<<","<<color.mat[2]<<std::endl;
				reflect_counter_++;
				return color;
			}
		}
		vec3 color ={{

				std::min((float)255, std::max((float)0, render_object->color_params_[0] + total_light_r + total_reflected_r)),
				std::min((float)255, std::max((float)0, render_object->color_params_[1] + total_light_g + total_reflected_g)),
				std::min((float)255, std::max((float)0, render_object->color_params_[2] + total_light_b + total_reflected_b))
/*
				std::min((float)255, std::max((float)0, render_object->color_params_[0] + total_light_r )),
				std::min((float)255, std::max((float)0, render_object->color_params_[1] + total_light_g )),
				std::min((float)255, std::max((float)0, render_object->color_params_[2] + total_light_b ))
*/
		}};

		//checkImage[i][j][0]=
		//checkImage[i][j][1]= std::min((float)255, std::max((float)0, render_object->color_params_[1] + total_light_g + total_reflected_g));
		//checkImage[i][j][2]= std::min((float)255, std::max((float)0, render_object->color_params_[2] + total_light_b + total_reflected_b));

		if(i==debug_i && j ==debug_j ){
			std::cout<<std::endl;
			Util::debug_head("Trace.cpp");
			std::cout<<"-------i "<<i<<"---j "<< j<<"-----"<<std::endl;
			std::cout<<"------Shape Info----"<<std::endl;
			std::cout<<"shape name:"<<render_object->name_<<std::endl;
			std::cout<<"shape color:"<< render_object-> color_params_[0]<<"," <<render_object->color_params_[1]<<","<<render_object->color_params_[2]<<std::endl;

			std::cout<<"shape geometry:";
			for(std::vector<float>::iterator k=(render_object->geo_params_).begin(); k!=(render_object->geo_params_).end(); k++){
				std::cout<<*k<<",";
			}
			std::cout<<std::endl;

			std::cout<<"shape reflected:"<<render_object->reflected_<<std::endl;

			std::cout<<std::endl;
			std::cout<<"total light:"<<total_light_r<<","<<total_light_g<<","<<total_light_b<<std::endl;

			std::cout<<std::endl;
			std::cout<<"checkImage:"<<(int)checkImage[i][j][0]<<","<<(int)checkImage[i][j][1]<<","<<(int)checkImage[i][j][2]<<std::endl;
			Util::debug_tail();
		}

		return color;
	}
	else
	{
		vec3 color ={{Background::r_, Background::g_, Background::b_}};
		return color;
//		checkImage[i][j][0]= Background::r_;
//		checkImage[i][j][1]= Background::g_;
//		checkImage[i][j][2]= Background::b_;
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
		vec3 vertex_a = {{ax,ay,az}};
		vec3 vertex_b = {{bx,by,bz}};
		vec3 vertex_c = {{cx,cy,cz}};

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


		vec3 norm = vec3Cross(vec3Minus(vertex_a, vertex_b),vec3Minus(vertex_a, vertex_c));
		vec3 normalized_norm = normalize(norm);
		//std::cout<<"norm:"<<normalized_norm.mat[0]<<","<<normalized_norm.mat[1]<<","<<normalized_norm.mat[2]<<std::endl;

		shape->norm_ = normalized_norm ;
		return t;

	}
	else if(!strcmp(shape->type_.c_str(), "sphere")){
		float R =  shape->geo_params_[0];
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
			vec3 p = {{ex+t*dx, ey+t*dy, ez+t*dz}};
			vec3 p_minus_c = vec3Minus(p, vec3_c);
			vec3 normlized_p_minus_c = {{p_minus_c.mat[0]/R, p_minus_c.mat[1]/R, p_minus_c.mat[2]/R}};
			shape->norm_ = normlized_p_minus_c;
			return t;
		}

	}
	else if(!strcmp(shape->type_.c_str(), "box")){
		float ux = shape->geo_params_[0];
		float uy = shape->geo_params_[1];
		float uz = shape->geo_params_[2];
		float vx = shape->geo_params_[3];
		float vy = shape->geo_params_[4];
		float vz = shape->geo_params_[5];


	}

	return 0;
}


