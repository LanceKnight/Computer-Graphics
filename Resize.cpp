#include <vector>
#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include <string>
#include "main.hh"
#include "Resize.hh"
#include "TiffRead.hh"
#include "TiffWrite.hh"
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


		if(!is_scale_x_less_than_zero){// scale_x >0			
			
			float M_x = std::min(scale_x, (float)1);
			float M_y = std::min(scale_y, (float)1);
			GLubyte temp_img[TiffRead::image_length_][new_width][3];		
	//		GLubyte temp_img[TiffRead::image_length_][TiffRead::image_width_][3];
			GLubyte temp_img2[new_length][new_width][3];		



			for(int i=0;i<TiffRead::image_length_;i++){
				for(int n =0; n<new_width; n++){
					float r = 0;
					float g = 0;
					float b = 0;
					int k_lower_limit = std::max(0, (int) floor(((n/scale_x)-(2/M_x))));
					int k_upper_limit = std::min(TiffRead::image_width_-1, (int)ceil(((n/scale_x)+(2/M_x))) );
					float norm2(0.0);
					for(int k=k_lower_limit; k<=k_upper_limit; k++){
						float lh = h(M_x*(n/scale_x-k));
						r+= float(checkImage[TiffRead::image_length_-i-1][k][0])*lh;
						g+= float(checkImage[TiffRead::image_length_-i-1][k][1])*lh;
						b+= float(checkImage[TiffRead::image_length_-i-1][k][2])*lh;
						norm2+=lh;
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

			if(is_scale_y_less_than_zero){//scale_x >0 scale_y <0
				for(int j=0;j<new_width;j++){
					for(int m =0; m<new_length; m++){
						float r = 0;
						float g = 0;
						float b = 0;

						int k_lower_limit = std::max(0, (int)floor(((m/scale_y)-(2/M_y))));
						int k_upper_limit = std::min(TiffRead::image_length_-1, (int)ceil(((m/scale_y)+(2/M_y))));
						float norm2(0.0);
						for(int k=k_lower_limit; k<=k_upper_limit; k++){
							float lh = h(M_y*(m/scale_y-k));
							r+= float(temp_img[k][j][0])*lh;
							g+= float(temp_img[k][j][1])*lh;
							b+= float(temp_img[k][j][2])*lh;
							norm2+=lh;
						}

						r = M_y*r/norm2;
						g = M_y*g/norm2;
						b = M_y*b/norm2;

						r = std::min(255, std::max(int(r),0));
						g = std::min(255, std::max(int(g),0));
						b = std::min(255, std::max(int(b),0));

						temp_img2[new_length-m-1][j][0] = r;
						temp_img2[new_length-m-1][j][1] = g;
						temp_img2[new_length-m-1][j][2] = b;
					}		
				}

			}//scale_x>0 scale_y<0
			else{//scale_x>0 scale_y>0

				for(int j=0;j<new_width;j++){
					for(int m =0; m<new_length; m++){
						float r = 0;
						float g = 0;
						float b = 0;
						int k_lower_limit = std::max(0,(int)floor(((m/scale_y)-(2/M_y))));
						int k_upper_limit = std::min(TiffRead::image_length_-1,(int)ceil(((m/scale_y)+(2/M_y))));
						float norm2(0.0);
						for(int k=k_lower_limit; k<=k_upper_limit; k++){
							float lh = h(M_y*((new_length-m-1)/scale_y-(TiffRead::image_length_-k-1)));
							r+= float(temp_img[TiffRead::image_length_-k-1][j][0])*lh;
							g+= float(temp_img[TiffRead::image_length_-k-1][j][1])*lh;
							b+= float(temp_img[TiffRead::image_length_-k-1][j][2])*lh;
							norm2+=lh;

							if(j == 55 && m == 55){
								std::cout<<"ln:"<<lh<<" k:"<<k<<std::endl;

								std::cout<<"tem_img:" <<int(temp_img[TiffRead::image_length_-(int)k-1][j][0])<<std::endl;
							}
						}
						//std::cout <<"norm2:"<<norm2<<std::endl;
						r = r/norm2;
						g = g/norm2;
						b = b/norm2;

						if(j == 55 && m == 55){
							std::cout<<"r: "<<r<<std::endl;
						}
						r = std::min(255, std::max((int)r,0));
						g = std::min(255, std::max((int)g,0));
						b = std::min(255, std::max((int)b,0));
	
						temp_img2[new_length-m-1][j][0] = r;
						temp_img2[new_length-m-1][j][1] = g;
						temp_img2[new_length-m-1][j][2] = b;

					}
				}

				std::cout<<"shit"<<std::endl;

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

		}
		else{//scale_x<0	
			
			float M_x = std::min(scale_x, (float)1);
			float M_y = std::min(scale_y, (float)1);
			GLubyte temp_img[TiffRead::image_length_][new_width][3];		
			GLubyte temp_img2[new_length][new_width][3];		


			for(int i=0;i<TiffRead::image_length_;i++){
				for(int n =0; n<new_width; n++){
					float r = 0;
					float g = 0;
					float b = 0;
					int k_lower_limit = std::max(0, (int)floor(((n/scale_x)-(2/M_x))));
					int k_upper_limit = std::min(TiffRead::image_width_-1,(int)ceil(((n/scale_x)+(2/M_x))));

               float norm2(0.0);
					for(int k=k_lower_limit; k<=k_upper_limit; k++){
							float lh = h(M_x*((new_width-n-1)/scale_x-(TiffRead::image_width_-k-1)));
							r+= float(checkImage[TiffRead::image_length_-i-1][TiffRead::image_width_-k-1][0])*lh;
							g+= float(checkImage[TiffRead::image_length_-i-1][TiffRead::image_width_-k-1][1])*lh;
							b+= float(checkImage[TiffRead::image_length_-i-1][TiffRead::image_width_-k-1][2])*lh;
							norm2+=lh;
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

			if(is_scale_y_less_than_zero){//scale_x<0 scale_y<0;
				for(int j=0;j<new_width;j++){
					for(int m =0; m<new_length; m++){
						float r = 0;
						float g = 0;
						float b = 0;
					
						int k_lower_limit = std::max(0, (int)floor(((m/scale_y)-(2/M_y))));	
						int k_upper_limit = std::min(TiffRead::image_length_-1,(int)ceil(((m/scale_y)+(2/M_y))));
                  float norm2(0.0);
						for(int k=k_lower_limit; k<=k_upper_limit; k++){
                     float lh =h(M_y*(m/scale_y-k));
							r+= float(temp_img[k][j][0])*lh;
							g+= float(temp_img[k][j][1])*lh;
							b+= float(temp_img[k][j][2])*lh;
                     norm2 += lh;
						}

						r = r/norm2;
						g = g/norm2;
						b = b/norm2;

						r = std::min(255, std::max(int(r),0));
						g = std::min(255, std::max(int(g),0));
						b = std::min(255, std::max(int(b),0));

						temp_img2[new_length-m-1][j][0] = r;
						temp_img2[new_length-m-1][j][1] = g;
						temp_img2[new_length-m-1][j][2] = b;
					}		
				}

			}//scale_x <0 scale_y>0
			else{//scale_x <0 scale_y>0
				for(int j=0;j<new_width;j++){
					for(int m =0; m<new_length; m++){
						float r = 0;
						float g = 0;
						float b = 0;

						int k_lower_limit = std::max(0,(int)floor(((m/scale_y)-(2/M_y))));
						int k_upper_limit = std::min(TiffRead::image_length_-1,(int)ceil(((m/scale_y)+(2/M_y))));
                  float norm2(0.0);
						for(int k=k_lower_limit; k<=k_upper_limit; k++){
							float lh = h(M_y*((new_length-m-1)/scale_y-(TiffRead::image_length_-k-1)));
							r+= float(temp_img[TiffRead::image_length_-k-1][j][0])*lh;
							g+= float(temp_img[TiffRead::image_length_-k-1][j][1])*lh;
							b+= float(temp_img[TiffRead::image_length_-k-1][j][2])*lh;
							norm2+=lh;
						}
						

						r = r/norm2;
						g = g/norm2;
						b = b/norm2;

						r = std::min(255, std::max(int(r),0));
						g = std::min(255, std::max(int(g),0));
						b = std::min(255, std::max(int(b),0));

						temp_img2[new_length-m-1][j][0] = r;
						temp_img2[new_length-m-1][j][1] = g;
						temp_img2[new_length-m-1][j][2] = b;
					}		
				}
			}//if scale_y >0


			 
			for (int i = 0; i < new_length; i++) {
				for (int j = 0; j < new_width; j++) {
					checkImage[new_length-i-1][j][0] = temp_img2[new_length-i-1][j][0];
					checkImage[new_length-i-1][j][1] = temp_img2[new_length-i-1][j][1];
					checkImage[new_length-i-1][j][2] = temp_img2[new_length-i-1][j][2];
				}
			}

			TiffRead::image_width_ = new_width;
			TiffRead::image_length_ = new_length;

		}//if scale_x <0;



		display();
/*
		std::string param1 = std::to_string(0);
		std::string param2 = std::to_string(0);
		std::string param3 = std::to_string(new_width);
		std::string param4 = std::to_string(new_length);

		std::vector<std::string> write_param_list;
		TiffWrite::tiff_write(write_param_list);	
*/


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
	if((x<=2)||x>=-2){
		return sinc(x)*sinc(x/2);
	}
	else{
		return 0;

	}
}


