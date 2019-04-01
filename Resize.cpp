#include <vector>
#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include <string>
#include "main.hh"
#include "Border.hh"
#include "Resize.hh"
#include "Select.hh"
#include "TiffRead.hh"
#include "TiffWrite.hh"


Resize::Resize(){}

Resize::~Resize(){}


std::string
Resize::resize(std::vector<std::string> paramList){

	#ifdef DEBUG
	
		std::cout<<"=====DEBUG  INFO====="<< std::endl;
			std::cout<<"Resize.cpp:"<<std::endl;
			std::cout<<"filter:";
			switch(Select::filter_){
				case Select::lanczos:
					std::cout<<"lanczos"<<std::endl;
					break;
				case Select::gaussian:
					std::cout<<"gaussian"<<std::endl;
					break;
				case Select::mitchell:
					std::cout<<"mitchell"<<std::endl;
					break;
				case Select::triangle:
					std::cout<<"triangle"<<std::endl;
					break;
				case Select::box:
					std::cout<<"box"<<std::endl;
					break;
				case Select::hemming:
					std::cout<<"hemming"<<std::endl;
					break;
			}
			std::cout<<"border:";
			switch(Border::border_){
				case Border::zero:
					std::cout<<"zero"<<std::endl;
					break;
				case Border::freeze:
					std::cout<<"freeze"<<std::endl;
					break;
				case Border::circular:
					std::cout<<"circular"<<std::endl;
					break;
			}

		std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
	
	#endif
	if(paramList.size()<=2){
		if(paramList.size()<2){
			return "too few params";
			 
		}
		#ifdef DEBUG
		/*
			std::cout<<"=====DEBUG  INFO====="<< std::endl;
			for (auto it = paramList.begin(); it!= paramList.end(); it++){
				std::cout<<"param:"<<*it<<std::endl;
			}

			std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
		*/
		#endif

	   if(TiffRead::has_image_ == false){
			return "You should read an image first.";
		}
	
		std::string x_str = paramList[0];
		std::string y_str = paramList[1];
		float scale_x = 0;
		float scale_y = 0;	
		try{
			scale_x = std::stof(x_str);
			scale_y = std::stof(y_str);
		}
		catch(...){
			return "invalid params";
		}

		if( (scale_x==0) || (scale_y==0) ){
			return "params cannot be 0";

		}
	

		bool is_scale_x_less_than_zero= scale_x<0;
		bool is_scale_y_less_than_zero= scale_y <0;
		scale_x = fabs(scale_x);
		scale_y = fabs(scale_y);
		int new_width = floor(TiffRead::image_width_*scale_x);
		int new_length = floor(TiffRead::image_length_*scale_y);


		if((new_length >1024)||(new_width>1024)){
			return "scale factor is too big";
		}
		
		float r = 0;
		float g = 0;
		float b = 0;

		float M_x = std::min(scale_x, (float)1);
		float M_y = std::min(scale_y, (float)1);
		GLubyte temp_img[TiffRead::image_length_][new_width][3];		
		GLubyte temp_img2[new_length][new_width][3];	
	



		for(int i=0;i<TiffRead::image_length_;i++){
			for(int n =0; n<new_width; n++){
				r = g = b =0;
//				int k_lower_limit = std::max(0, get_lower_limit(n, scale_x, M_x));
//				int k_upper_limit = std::min(TiffRead::image_width_-1,get_upper_limit(n, scale_x, M_x));

				int k_lower_limit = get_lower_limit(n, scale_x, M_x);
				int k_upper_limit = get_upper_limit(n, scale_x, M_x);	
				float norm2(0.0);
				for(int k=k_lower_limit; k<=k_upper_limit; k++){
					int adjusted_n = is_scale_x_less_than_zero? new_width-n-1:n;
					int adjusted_k = is_scale_x_less_than_zero? TiffRead::image_width_-k-1:k;
					float lh = kernel(M_x*(adjusted_n/scale_x-adjusted_k));

					if(adjusted_k<0 || adjusted_k>TiffRead::image_width_-1){
						switch(Border::border_)	{
							case Border::freeze:
									if( adjusted_k<0 ){	
										r+= float(checkImage[TiffRead::image_length_-i-1][0][0])*lh;
										g+= float(checkImage[TiffRead::image_length_-i-1][0][1])*lh;
										b+= float(checkImage[TiffRead::image_length_-i-1][0][2])*lh;
									}
									else{
										r+= float(checkImage[TiffRead::image_length_-i-1][TiffRead::image_width_-1][0])*lh;
										g+= float(checkImage[TiffRead::image_length_-i-1][TiffRead::image_width_-1][1])*lh;
										b+= float(checkImage[TiffRead::image_length_-i-1][TiffRead::image_width_-1][2])*lh;
									}	
								break;
							case Border::circular:
									if(adjusted_k<0){	
										r+= float(checkImage[TiffRead::image_length_-i-1][-adjusted_k][0])*lh;
										g+= float(checkImage[TiffRead::image_length_-i-1][-adjusted_k][1])*lh;
										b+= float(checkImage[TiffRead::image_length_-i-1][-adjusted_k][2])*lh;
									}
									else{
										//std::cout<<"adjusted_k:"<<adjusted_k<<std::endl;
										//std::cout<<"2*width - adjusted_k"<<2*TiffRead::image_width_ - adjusted_k<<std::endl;
										r+= float(checkImage[TiffRead::image_length_-i-1][2*TiffRead::image_width_ - adjusted_k-1][0])*lh;
										g+= float(checkImage[TiffRead::image_length_-i-1][2*TiffRead::image_width_ - adjusted_k-1][1])*lh;
										b+= float(checkImage[TiffRead::image_length_-i-1][2*TiffRead::image_width_ - adjusted_k-1][2])*lh;
									}
								break;
							case Border::zero:
										
								break;
						}												

					}
					else{
						r+= float(checkImage[TiffRead::image_length_-i-1][adjusted_k][0])*lh;
						g+= float(checkImage[TiffRead::image_length_-i-1][adjusted_k][1])*lh;
						b+= float(checkImage[TiffRead::image_length_-i-1][adjusted_k][2])*lh;

					}

					norm2+=lh;
				}
				if( (i == 0 && n ==0 )  ||  ( i==50 && n ==50  )){
					//std::cout<<"i:"<<i<<", norm2:      "<<norm2<<std::endl;

				}	

				r = r/norm2;
				g = g/norm2;
				b = b/norm2;

				r = std::min(255, std::max(int(r),0));
				g = std::min(255, std::max(int(g),0));
				b = std::min(255, std::max(int(b),0));

				temp_img[TiffRead::image_length_-i-1][n][0] = r;
				temp_img[TiffRead::image_length_-i-1][n][1] = g;
				temp_img[TiffRead::image_length_-i-1][n][2] = b;
				
			}
		}



		for(int j=0;j<new_width;j++){
			for(int m =0; m<new_length; m++){
				r = g = b =0;
			//	int k_lower_limit = std::max(0,get_lower_limit(m, scale_y, M_y));
			//	int k_upper_limit = std::min(TiffRead::image_length_-1,get_upper_limit(m, scale_y, M_y));

				int k_lower_limit = get_lower_limit(m, scale_y, M_y);
				int k_upper_limit = get_upper_limit(m, scale_y, M_y);	
				float norm2(0.0);
				for(int k=k_lower_limit; k<=k_upper_limit; k++){
					int adjusted_m = is_scale_y_less_than_zero? m : new_length-m-1;
					int adjusted_k = is_scale_y_less_than_zero? k : TiffRead::image_length_-k-1;
					float lh = kernel(M_y*adjusted_m/scale_y-adjusted_k);


					if(adjusted_k<0||adjusted_k>TiffRead::image_length_-1){	
						switch(Border::border_)	{
							case Border::freeze:
								if( adjusted_k<0 ){	
									r+=float(temp_img[0][j][0])*lh;//100;// float(temp_img[TiffRead::image_length_-1][j][0])*lh;
									g+=float(temp_img[0][j][1])*lh;//100;// float(temp_img[TiffRead::image_length_-1][j][1])*lh;
									b+=float(temp_img[0][j][2])*lh;//100;// float(temp_img[TiffRead::image_length_-1][j][2])*lh;
								}
								else{
									r+= float(temp_img[TiffRead::image_length_-1][j][0])*lh;
									g+= float(temp_img[TiffRead::image_length_-1][j][1])*lh;
									b+= float(temp_img[TiffRead::image_length_-1][j][2])*lh;
								}	
								break;
							case Border::circular:
								if((adjusted_k<0)){
									r+= float(temp_img[-adjusted_k][j][0])*lh;
									g+= float(temp_img[-adjusted_k][j][1])*lh;
									b+= float(temp_img[-adjusted_k][j][2])*lh;
									//std::cout<<"adjusted_k(<0):"<<adjusted_k<<std::endl;	
								}
								else{
									//std::cout<<"adjusted_k:"<<adjusted_k<<std::endl;
									//std::cout<<"2*TiffRead::image_length_ - adjusted_k-1:" << 2*TiffRead::image_length_ - adjusted_k-1<<std::endl;

									r+= float(temp_img[2*TiffRead::image_length_ -adjusted_k-1][j][0])*lh;
									g+= float(temp_img[2*TiffRead::image_length_ -adjusted_k-1][j][1])*lh;
									b+= float(temp_img[2*TiffRead::image_length_ -adjusted_k-1][j][2])*lh;

								//	g+= float(checkImage[TiffRead::image_length_-i-1][2*TiffRead::image_width_ - adjusted_k][1])*lh;
								//	b+= float(checkImage[TiffRead::image_length_-i-1][2*TiffRead::image_width_ - adjusted_k][2])*lh;
								}
								break;
							case Border::zero:		
								break;
						}
					}
					else{
//						std::cout<<"adjusted_k: "<<adjusted_k<<", j: "<< j<<std::endl; 
						r+= float(temp_img[adjusted_k][j][0])*lh;
						g+= float(temp_img[adjusted_k][j][1])*lh;
						b+= float(temp_img[adjusted_k][j][2])*lh;
					}
					norm2+=lh;

				}
				r = r/norm2;
				g = g/norm2;
				b = b/norm2;

				r = std::min(255, std::max((int)r,0));
				g = std::min(255, std::max((int)g,0));
				b = std::min(255, std::max((int)b,0));

				temp_img2[new_length-m-1][j][0] = r;
				temp_img2[new_length-m-1][j][1] = g;
				temp_img2[new_length-m-1][j][2] = b;

			}
		}


		 
		for (int i = 0; i < new_length; i++) {
			for (int j = 0; j < new_width; j++) {
				checkImage[new_length-i-1][j][0] = temp_img2[new_length-i-1][j][0];
				checkImage[new_length-i-1][j][1] = temp_img2[new_length-i-1][j][1];
				checkImage[new_length-i-1][j][2] = temp_img2[new_length-i-1][j][2];
			}
		}

		TiffRead::image_width_ = new_width;
		TiffRead::image_length_ = new_length;




		display();

		return "image resized";
	}
	else{
		return  "too many parameters";
	}

}

float Resize::sinc(float x){
	if(x==0){

		return 1;
	}
	else{
		return sin(x)/x;
	}

}

float Resize::kernel(float x){
	switch(Select::filter_){	
		case Select::lanczos:
			if(fabs(x)<Select::alpha_){
				return sinc(x)*sinc(x/Select::alpha_);
			}
			else{
				return 0;

			}
		case Select::gaussian:
			if(fabs(x)<Select::alpha_){
				return 1/(Select::alpha_*(sqrt(2*M_PI))) * exp(-1/2 * pow(x/Select::alpha_,2));
			}
			else{
				return 0;
			}
		case Select::mitchell:
			if(x>=-1 && x<=1 ){
				return 1.0/18.0*(-21 * pow(1-fabs(x),3) + 27*pow(1-fabs(x),2) + 9 *(1-fabs(x)) +1);
			}
			else if(fabs(x) >=1 && fabs(x) <=2){
				return 1.0/18.0*(7*pow(2-fabs(x),3) - 6*pow(2-fabs(x),2));
			}
			else{
				return 0;
			}
		case Select::triangle:
			if(fabs(x/Select::alpha_)<1){
				return (1-fabs(x/Select::alpha_))/Select::alpha_;
			}
			else{
				return 0;
			}
			return 0;
		case Select::box:
			if( x>= -Select::alpha_ && x < Select::alpha_){
				return 1/(2*Select::alpha_);
			}
			else{
				return 0;
			}
		case Select::hemming:
		//testing codes
		/* 
		double scaling = 2;
		double radius = 2;
		double newpos = x/scaling;
		if(fabs(newpos)>	radius +0.1){
			return 0;
		}	
		else{
			if(fabs(newpos)<=radius +0.1){
				return (0.5+0.46*cos(M_PI*newpos/radius))/scaling;
			}else return 0;
		}
		*/
		
			if(x>=0 && x<=Select::alpha_){
				return 0.54-0.46*cos(2*M_PI*x/(Select::alpha_-1));
			}
			else{
				return 0;
			}

		case Select::b_spline:
			if(fabs(x)<=1){
				return 1.0/6.0*( -3*pow(1-fabs(x),3) + 3*pow(1-fabs(x),2) + 3*(1-fabs(x)) +1);
			}
			else if(fabs(x) >=1 && fabs(x) <=2){
				return 1.0/6.0*pow(2-fabs(x),3);
			}
			else{
				return 0;
			}
		case Select::catmull_rom:
			if(fabs(x)<=1){
				return 1.0/2.0 *pow((1-fabs(x)),3)+4*pow(1-fabs(x),2)+(1-fabs(x));
			}
			else if(fabs(x) >=1 && fabs(x) <=2){

				return 1.0/2.0* pow((2-fabs(x)),3)-pow((2-fabs(x)),2);

			}
			else{
				return 0;
			}
	}
	return -1;
}


int Resize::get_lower_limit(int m, float scale_y, float M_y){
	switch(Select::filter_){	
		case Select::lanczos:
			return floor(((m/scale_y)-(Select::alpha_/M_y)));
		case Select::gaussian:
			return floor(((m/scale_y)-(Select::alpha_/M_y)));
		case Select::mitchell:
			return floor(((m/scale_y)-(2/M_y)));
		case Select::triangle:
			return floor(((m/scale_y)-(Select::alpha_/M_y)));
		case Select::box:
			return floor(((m/scale_y)-(Select::alpha_/M_y)));
	
		case Select::hemming:
			return floor(((m/scale_y)-(Select::alpha_/M_y)));

		case Select::b_spline: 
			return floor(((m/scale_y)-(2/M_y)));

		case Select::catmull_rom:
			return floor(((m/scale_y)-(2/M_y)));
	}
	return -1;
}


int Resize::get_upper_limit(int m, float scale_y, float M_y){
	switch(Select::filter_){	
		case Select::lanczos:
			return ceil(((m/scale_y)+(Select::alpha_/M_y)));
		case Select::gaussian:
			return ceil(((m/scale_y)+(Select::alpha_/M_y)));
		case Select::mitchell:
			return ceil(((m/scale_y)+(2/M_y)));
		case Select::triangle:
			return ceil(((m/scale_y)+(Select::alpha_/M_y)));
		case Select::box:
			return ceil(((m/scale_y)+(Select::alpha_/M_y)));

		case Select::hemming:
			return ceil(((m/scale_y)+(Select::alpha_/M_y)));

		case Select::b_spline:
			return ceil(((m/scale_y)+(2/M_y)));
		case Select::catmull_rom:
			return ceil(((m/scale_y)+(2/M_y)));
	}
	return -1;
}




