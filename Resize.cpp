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
		double scale_x = 0;
		double scale_y = 0;	
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
/*
		int int_scale_x;
		int int_scale_y;	
		int adjusted_scale_x;
		int adjusted_scale_y;
		if(scale_x >=1){
			if(scale_y >=1){
				int_scale_x = (int)scale_x;
				int_scale_y = (int)scale_y;	
				adjusted_y = int_scale_y;
			}
			else{
				int_scale_x = (int)scale_x;
				int_scale_y = (int)(1/(scale_y));
				adjusted_scale_y = 
			}
		}
		else{
			if(scale_y >

		}
*/	

	
		if((TiffRead::image_length_*scale_x >1024)||(TiffRead::image_width_*scale_y>1024)){
			return "scale factor is too big";
		}	
		if(scale_x >1){
			if(scale_y>1){
				//std::vector<std::vector<std::vector<int>>> temp;

				GLubyte temp[1024][1024][3];
				for(int i = 0; i<TiffRead::image_length_*scale_y; i++){
					for(int j=0; j<TiffRead::image_width_*scale_x;j++){

						temp[i][j][0] = checkImage[(int)floor(i/scale_y)][(int)floor(j/scale_x)][0];				
						temp[i][j][1] = checkImage[(int)floor(i/scale_y)][(int)floor(j/scale_x)][1];
						temp[i][j][2] = checkImage[(int)floor(i/scale_y)][(int)floor(j/scale_x)][2];
					}
				}	
				for(int i = 0; i<TiffRead::image_length_*scale_y; i++){
					for(int j=0; j<TiffRead::image_width_*scale_x;j++){
						checkImage[i][j][0] = temp[i][j][0];		
						checkImage[i][j][1] = temp[i][j][1];
						checkImage[i][j][2] = temp[i][j][2];
					}
				}

			}
			else{//if scale_y <=1

				for(int i = 0; i<TiffRead::image_length_*scale_y; i++){
					for(int j=0; j<TiffRead::image_width_*scale_x;j++){
						checkImage[i][j][0] = checkImage[(int)floor(i/scale_y)][(int)floor(j/scale_x)][0];				
						checkImage[i][j][1] = checkImage[(int)floor(i/scale_y)][(int)floor(j/scale_x)][1];
						checkImage[i][j][2] = checkImage[(int)floor(i/scale_y)][(int)floor(j/scale_x)][2];
					}
				}

			}
		}
		else{//if scale_x <=1
			if(scale_y>1){
				for(int i = 0; i<TiffRead::image_length_*scale_y; i++){
					for(int j=0; j<TiffRead::image_width_*scale_x;j++){
						checkImage[i][j][0] = checkImage[(int)floor(i/scale_y)][(int)floor(j/scale_x)][0];				
						checkImage[i][j][1] = checkImage[(int)floor(i/scale_y)][(int)floor(j/scale_x)][1];
						checkImage[i][j][2] = checkImage[(int)floor(i/scale_y)][(int)floor(j/scale_x)][2];
					}
				}
			}
			else{//if scale_x <=1 and scale_y <=1

				for(int i = 0; i<TiffRead::image_length_*scale_y; i++){
					for(int j=0; j<TiffRead::image_width_*scale_x;j++){
						checkImage[i][j][0] = checkImage[(int)floor(i/scale_y)][(int)floor(j/scale_x)][0];				
						checkImage[i][j][1] = checkImage[(int)floor(i/scale_y)][(int)floor(j/scale_x)][1];
						checkImage[i][j][2] = checkImage[(int)floor(i/scale_y)][(int)floor(j/scale_x)][2];
					}
				}

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


