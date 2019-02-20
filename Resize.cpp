#include <vector>
#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include "main.hh"
#include "Resize.hh"
#include "TiffRead.hh"

Resize::Resize(){}

Resize::~Resize(){}


std::string
Resize::resize(std::vector<std::string> paramList){


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
		std::cout<<scale_x<<std::endl;
		std::cout<<scale_y<<std::endl;	
		if((scale_x<=0)||(scale_y <=0)){
			return "params should be bigger than 0";
		}
		
		int new_width = floor(TiffRead::image_width_*scale_x);
		int new_length = floor(TiffRead::image_length_*scale_y);
	
		if((new_length >1024)||(new_width>1024)){
			return "scale factor is too big";
		}
		

		float M_x = std::min(scale_x, (float)1);
		GLubyte temp_img[TiffRead::image_length_][new_width][3];		
      float norm = 0.0;
      for(int k=0; k<TiffRead::image_width_; k++){
        norm += h(M_x*k);
		}
      std::cout << norm << std::endl;
		for(int i=0;i<TiffRead::image_length_;i++){
			for(int n =0; n<new_width; n++){
				int r = 0;
				int g = 0;
				int b = 0;
				for(int k=0; k<TiffRead::image_width_; k++){
					r+= float(checkImage[TiffRead::image_length_-i-1][k][0])*h(M_x*(n/scale_x-k));
					g+= float(checkImage[TiffRead::image_length_-i-1][k][1])*h(M_x*(n/scale_x-k));
					b+= float(checkImage[TiffRead::image_length_-i-1][k][2])*h(M_x*(n/scale_x-k));
				}
				r = M_x*r/norm;
				g = M_x*g/norm;
				b = M_x*b/norm;
				if(r>255){
					r =255;
				}
				if(g>255){
					g =255;
				}
				if(b>255){
					b =255;
				}
				//std::cout<<"r:"<<r<<std::endl;
				temp_img[TiffRead::image_length_-i-1][n][0] = r;
				temp_img[TiffRead::image_length_-i-1][n][1] = g;
				temp_img[TiffRead::image_length_-i-1][n][2] = b;
				
			}
		}
		 
		for (int i = 0; i < TiffRead::image_length_; i++) {
			for (int j = 0; j < new_width; j++) {
				checkImage[TiffRead::image_length_-i-1][j][0] = temp_img[TiffRead::image_length_-i-1][j][0];
				checkImage[TiffRead::image_length_-i-1][j][1] = temp_img[TiffRead::image_length_-i-1][j][1];
				checkImage[TiffRead::image_length_-i-1][j][2] = temp_img[TiffRead::image_length_-i-1][j][2];
			}
		}
		display();
		std::cout<<"resizing"<<std::endl;

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

float Resize::h(float x){
	if((x<2)||x>-2){
		return sinc(x)*sinc(x/2);
	}
	else{
		return 0;

	}
}


