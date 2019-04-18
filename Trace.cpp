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

#define debug_i 250
#define debug_j 400
#define MAX_REFLECT 1
#define t0 0
#define t1 std::numeric_limits<float>::infinity()

int Trace::reflect_counter_ =0;

std::string
Trace::trace(std::vector<std::string> paramList){
	if(paramList.size()==0){


		if(Orthocamera::ortho_){

			for(int i = 0; i < Screen::nx_; i++){
				for(int j = 0; j < Screen::ny_; j++){
					Ray ray = buildCameraRay(i,j);
					reflect_counter_ = 0;
					vec3 color = getColorFromRay(ray, i, j);
					checkImage[i][j][0]= std::min((float)255, std::max((float)0, 255*color.mat[0]));
					checkImage[i][j][1]= std::min((float)255, std::max((float)0, 255*color.mat[1]));
					checkImage[i][j][2]= std::min((float)255, std::max((float)0, 255*color.mat[2]));

					checkImage[debug_i][debug_j][0]= 0;
					checkImage[debug_i][debug_j][1]= 0;
					checkImage[debug_i][debug_j][2]= 0;
				}
			}
		}
		else if (Camera::perspect_){

			for(int i = 0; i < Screen::nx_; i++){
				for(int j = 0; j < Screen::ny_; j++){
					Ray ray = buildCameraRay(i,j);
					reflect_counter_ = 0;
					vec3 color = getColorFromRay(ray, i, j);
					checkImage[i][j][0]= std::min((float)255, std::max((float)0, 255*color.mat[0]));
					checkImage[i][j][1]= std::min((float)255, std::max((float)0, 255*color.mat[1]));
					checkImage[i][j][2]= std::min((float)255, std::max((float)0, 255*color.mat[2]));

					checkImage[debug_i][debug_j][0]= 0;
					checkImage[debug_i][debug_j][1]= 0;
					checkImage[debug_i][debug_j][2]= 0;
				}
			}


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


vec3
Trace::getColorFromRay(Ray ray, int i, int j){
	float smallest_t = std::numeric_limits<float>::infinity();
	Triangle tri(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

	Shape* render_object = &tri;
	bool hit = false;

	for(std::vector<Shape*>::iterator it = Trace::object_list_.begin();it != object_list_.end();++it){

		float t = intersect(ray, *it, i, j);


		if(t){
			hit = true;
			if(t<smallest_t){
				smallest_t = t;
				render_object = *it;
			}
		}
	}

	if(i==debug_i && j ==debug_j){
		std::cout<<"hit:"<<hit<<std::endl;
	}

	if(hit){
		float total_light_r = 0;
		float total_light_g = 0;
		float total_light_b = 0;
		vec3 intersect_point = {{ray.ori_x_+smallest_t*ray.dir_x_,ray.ori_y_+smallest_t*ray.dir_y_,ray.ori_z_+smallest_t*ray.dir_z_,}};
		for(std::vector<Ilight *>::iterator light_it = Trace::light_list_.begin();light_it != Trace::light_list_.end();++light_it){
			vec3 light ={{(*light_it)->dx_, (*light_it)->dy_, (*light_it)->dz_}};

			vec3 vec3_l = vec3Minus(light, intersect_point);
			vec3 normalized_light = normal(vec3_l);

			float rr = 0;
			float rg = 0;
			float rb = 0;
			rr = render_object->color_params_[3];
			rg = render_object->color_params_[4];
			rb = render_object->color_params_[5];
//			std::string flag;
//			if(!strcmp(render_object->type_.c_str(),"triangle")){
//				flag = "tri";
//				rr = render_object->color_params_[3];
//				rg = render_object->color_params_[4];
//				rb = render_object->color_params_[5];
//			}
//			else if (!strcmp(render_object->type_.c_str(),"sphere")){
//				flag = "sphere";
//				rr = render_object->color_params_[3];
//				rg = render_object->color_params_[4];
//				rb = render_object->color_params_[5];
//			}
/*
			if(i==debug_i && j ==debug_j ){
				Util::debug_head("Trace.cpp");
				std::cout<<"-------i "<<i<<"---j "<< j<<"-----"<<std::endl;
				std::cout<<"------Light Info----"<<std::endl;
				std::cout<<"light name:"<<(*light_it)->name_<<std::endl;
				std::cout<<"light position:"<<(*light_it)->dx_<<","<<(*light_it)->dy_<<","<< (*light_it)->dz_<<std::endl;
				std::cout<<"light color:"<<(*light_it)->lr_<<","<<(*light_it)->lg_<<","<< (*light_it)->lb_<<std::endl;
				std::cout<<"cos:"<<vec3Mul(render_object->norm_, normalized_light)<<std::endl;
				std::cout<<"render_object norm:"<<(render_object->norm_).mat[0]<<","<<(render_object->norm_).mat[1]<<","<<(render_object->norm_).mat[2]<<std::endl;
				std::cout<<"render_object reflected:"<<render_object->reflected_<<std::endl;
				Util::debug_tail();
			}*/
			total_light_r +=  ((*light_it)->lr_  * rr * vec3Mul(render_object->norm_, normalized_light));
			total_light_g +=  ((*light_it)->lg_  * rg * vec3Mul(render_object->norm_, normalized_light));
			total_light_b +=  ((*light_it)->lb_  * rb * vec3Mul(render_object->norm_, normalized_light));

		}

		//reflection
		float total_reflected_r = 0;
		float total_reflected_g = 0;
		float total_reflected_b = 0;

	/*
		if((render_object->reflected_) && (reflect_counter_<MAX_REFLECT)){

			Ray reflected_ray = buildReflectedRay(ray, render_object->norm_, intersect_point, i,j);
			if(i==debug_i && j==debug_j){
				Util::debug_head("Trace.cpp");
				std::cout<<"first reflection counter:" <<reflect_counter_<<std::endl;
				std::cout<<"norm:"<<render_object->norm_.mat[0]<<","<<render_object->norm_.mat[1]<<","<<render_object->norm_.mat[2]<<std::endl;
				std::cout<<std::endl;
				std::cout<<"original Ray:"<<std::endl;
				std::cout<<"ori:"<<ray.ori_x_<<","<<ray.ori_y_<<","<<ray.ori_z_<<std::endl;
				std::cout<<"dir:"<<ray.dir_x_<<","<<ray.dir_y_<<","<<ray.dir_z_<<std::endl;

				std::cout<<std::endl;
				std::cout<<"reflected Ray:"<<std::endl;
				std::cout<<"ori:"<<reflected_ray.ori_x_<<","<<reflected_ray.ori_y_<<","<<reflected_ray.ori_z_<<std::endl;
				std::cout<<"dir:"<<reflected_ray.dir_x_<<","<<reflected_ray.dir_y_<<","<<reflected_ray.dir_z_<<std::endl;
				Util::debug_tail();
			}
			reflect_counter_++;
			vec3 color = getColorFromRay(reflected_ray, i, j);

			total_reflected_r = render_object->color_params_[6]*color.mat[0];
			total_reflected_g = render_object->color_params_[7]*color.mat[1];
			total_reflected_b = render_object->color_params_[8]*color.mat[2];


		}
*/
		vec3 color ={{

				std::min((float)1, std::max((float)0, render_object->color_params_[0] + total_light_r + total_reflected_r)),
				std::min((float)1, std::max((float)0, render_object->color_params_[1] + total_light_g + total_reflected_g)),
				std::min((float)1, std::max((float)0, render_object->color_params_[2] + total_light_b + total_reflected_b))

		}};


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
	}
}




Ray
Trace::buildCameraRay(int i, int j){
	if(Orthocamera::ortho_){
		Ray ray(j, i, -1, 0, 0 , -1 );
		return ray;
	}
	else{
		vec3 gz = {{Camera::ex_-Camera::gx_,Camera::ey_-Camera::gy_,Camera::ez_-Camera::gz_}};

		vec3 up = {{Camera::ux_, Camera::uy_, Camera::uz_}};

		vec3 v = normal(up);

		vec3 u = normal(vec3Cross(v, gz));

		vec3 w = normal(vec3Cross(u, v));

		float x_convert = j*(Camera::bu_-Camera::au_)/Screen::nx_+Camera::au_;
		float y_convert = i*(Camera::bv_-Camera::av_)/Screen::ny_+Camera::av_;

		         vec3 dir = vec3Add(vec3Add(vec3NumMul(x_convert, u), vec3NumMul( y_convert,v)), vec3NumMul(-Camera::s_, w));
		//vec3 dir = vec3Add(vec3Add(vec3NumMul(j-Camera::gx_, u), vec3NumMul(i-Camera::gy_, v)), vec3NumMul(-Camera::s_, w));

//		if(i==499 && j ==499){
//			 std::cout<<"gz:"<<gz.mat[0]<<","<<gz.mat[1]<<","<<gz.mat[2]<<std::endl;
//			 std::cout<<"v:"<<v.mat[0]<<","<<v.mat[1]<<","<<v.mat[2]<<std::endl;
//			 std::cout<<"u:"<<u.mat[0]<<","<<u.mat[1]<<","<<u.mat[2]<<std::endl;
//			 std::cout<<"w:"<<w.mat[0]<<","<<w.mat[1]<<","<<w.mat[2]<<std::endl;
//			 std::cout<<"perspective: eye:"<<Camera::ex_<<","<< Camera::ey_<<","<<Camera::ez_<<std::endl;
//			 std::cout<<"             dir:"<<dir.mat[0]<<","<<dir.mat[1]<<","<<dir.mat[2]<<std::endl;
//		}

		Ray ray( Camera::ex_, Camera::ey_, Camera::ez_ ,dir.mat[0], dir.mat[1] , dir.mat[2]);
		return ray;
	}
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

	if(i == debug_i && j == debug_j){
		Util::debug_head("Trace.cpp");
		std::cout<<"d_n:"<<d_n<<std::endl;
		std::cout<<"norm:"<<norm.mat[0]<<","<<norm.mat[1]<<","<<norm.mat[2]<<std::endl;
		std::cout<<"second term:"<<second_term.mat[0]<<","<<second_term.mat[1]<<","<<second_term.mat[2]<<std::endl;
		Util::debug_tail();

	}

	return reflected_ray;


}

float
Trace::intersect(Ray ray, Shape* shape, int i, int j){// i, j are just for debugging

	if(!strcmp(shape->type_.c_str(), "triangle")){
		return intersectTriangle(shape, ray );
	}
	else if(!strcmp(shape->type_.c_str(), "sphere")){
		return intersectSphere(shape, ray);
	}
	else if(!strcmp(shape->type_.c_str(), "box")){
		return intersectBox(shape, ray, i, j );


	}
	else if(!strcmp(shape->type_.c_str(), "plane")){
		return intersectPlane(shape, ray);

	}

	return 0;
}


float
Trace::intersectTriangle(Shape* shape, Ray ray){
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
Trace::intersectSphere(Shape* shape, Ray ray){
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
Trace::intersectBox(Shape* shape, Ray ray, int i, int j ){// i, j for debug
	float ux = shape->geo_params_[0];
	float uy = shape->geo_params_[1];
	float uz = shape->geo_params_[2];
	float vx = shape->geo_params_[3];
	float vy = shape->geo_params_[4];
	float vz = shape->geo_params_[5];

	Triangle frontA(ux, uy, uz, ux, vy, uz, vx, uy, uz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0);
	float frontA_t = intersectTriangle(&frontA, ray);

	Triangle frontB(vx, vy, uz, ux, vy, uz, vx, uy, uz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0);
	float frontB_t = intersectTriangle(&frontB, ray);

	Triangle backA(ux, uy, vz, ux, vy, vz, vx, uy, vz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0);
	float backA_t = intersectTriangle(&backA, ray);

	Triangle backB(vx, vy, vz, ux, vy, vz, vx, uy, vz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0);
	float backB_t = intersectTriangle(&backB, ray);

	Triangle leftA(ux, uy, uz, ux, vy, uz, ux, uy, vz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0);
	float leftA_t = intersectTriangle(&leftA, ray);

	Triangle leftB(ux, vy, vz, ux, vy, uz, ux, uy, vz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0);
	float leftB_t = intersectTriangle(&leftB, ray);

	Triangle rightA(vx, uy, uz, vx, vy, uz, vx, uy, vz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0);
	float rightA_t = intersectTriangle(&rightA, ray);

	Triangle rightB(vx, vy, vz, vx, vy, uz, vx, uy, vz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0);
	float rightB_t = intersectTriangle(&rightB, ray);

	Triangle topA(ux, vy, uz, ux, vy, vz, vx, vy, uz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0);
	float topA_t = intersectTriangle(&topA, ray);

	Triangle topB(vx, vy, vz, ux, vy, vz, vx, vy, uz,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0);
	float topB_t = intersectTriangle(&topB, ray);

	Triangle bottomA(ux, uy, uz, ux, uy, vz, vx, uy, uz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0);
	float bottomA_t = intersectTriangle(&bottomA, ray);

	Triangle bottomB(vx, uy, vz, ux, uy, vz, vx, uy, uz, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0);
	float bottomB_t = intersectTriangle(&bottomB, ray);

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


}


float
Trace::intersectPlane(Shape* shape, Ray ray){
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

			return t;

}

