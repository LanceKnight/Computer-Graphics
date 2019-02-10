#include <cstring>
#include <fstream>
#include <GL/glut.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "Dispatcher.hh"
#include "TiffRead.hh"
#include "TiffWrite.hh"
#include "Util.hh"
#include "main.hh"

int TiffWrite::image_length_=0;
int TiffWrite::image_width_=0;
int TiffWrite::rows_per_strip_=0;
std::vector<int> TiffWrite::strip_offsets_(1,0);
std::vector<int> TiffWrite::strip_byte_counts_(1,0);
int TiffWrite::strips_per_image_ = 0;
std::vector<int> TiffWrite::bits_per_sample_(1,0);
int TiffWrite::photo_metric_ =0;
std::vector<int> TiffWrite::r_color_map_(256,0);
std::vector<int> TiffWrite::g_color_map_(256,0);
std::vector<int> TiffWrite::b_color_map_(256,0);

TiffWrite::TiffWrite(){}

TiffWrite::~TiffWrite(){}



std::string
TiffWrite::tiff_write(std::vector<std::string> paramList){
	std::string result;

	if(paramList.size()<=5){// params <=5
		if(paramList.size()==5){
			
			std::string filename = *paramList.begin();
			std::string x0_str = paramList[1];
			std::string y0_str = paramList[2];
			std::string xc_str = paramList[3];
			std::string yc_str = paramList[4];
			
			int x0 = std::stoi(x0_str);
			int xc = std::stoi(xc_str);
			int y0 = std::stoi(y0_str);
			int yc = std::stoi(yc_str);

			std::ofstream out_file(filename.c_str(), std::ios::binary);
		  	if (out_file.fail()) {
				result = "Unable to write to "+ filename;
			}
			else{
	
				#ifdef DEBUG
			
					std::cout<<"=====DEBUG  INFO====="<<std::endl;
					std::cout<<"length:"<<TiffRead::image_length_<<std::endl;
					std::cout<<"is_gray?"<<TiffRead::is_gray_image_<<std::endl;

					std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;			
				#endif
				unsigned char buffer[2] ={0x4d,0x4d};
				unsigned char magicno[2] = {0x00, 0x2a};
				unsigned char ifd_address[4] = {0x00, 0x00, 0x00, 0x08};
				out_file.write((char*)buffer,2);
				out_file.write((char*)magicno,2);
				out_file.write((char*)ifd_address,4);
				if(TiffRead::is_gray_image_){//gray image
					std::cout<<"This is gray image writing route"<<std::endl;
					unsigned char ifd_num[2] ={0x00,0xb};
					out_file.write((char*)ifd_num, 2);

					unsigned char starting_offset_value_address_bytes[4] = {0x8e,0x00,0x00,0x00};

					//image width
					int image_width = xc-x0+1;
					unsigned char* ifd_image_width = IFD_encode(256, 4, 1, image_width);
					out_file.write((char*)ifd_image_width,12);	

					//image length
					int image_length = yc-y0+1;
					unsigned char* ifd_image_length = IFD_encode(257, 4, 1, image_length);	
					out_file.write((char*)ifd_image_length,12);	

					//bits per sample
					char bits_per_sample[4] = {0x00, 0x00,0x08,0x00};	
					unsigned char* ifd_bits_per_sample = IFD_encode(258, 3, 1, *((int*)bits_per_sample));	
					out_file.write((char*)ifd_bits_per_sample,12);	

					//compression
					unsigned char compression_content[4] =  {0x00, 0x00, ((unsigned char*)&TiffRead::compression_)[0], 0x00};
					unsigned char* ifd_compression = IFD_encode(259, 3, 1, *((int*)compression_content));	
					out_file.write((char*)ifd_compression,12);

					//photometric_intepretation					
					unsigned char photo_metric_content[4] =  {0x00, 0x00, 0x01, 0x00};
					unsigned char* ifd_photo_metric = IFD_encode(262, 3, 1, *((int*)photo_metric_content));	
					out_file.write((char*)ifd_photo_metric,12);

					//hold the space for strip_offset
					int strip_offset_address = out_file.tellp();
					unsigned char space_holder[4] = {0x00,0x00,0x00,0x00};						
					unsigned char* ifd_strip_offset = IFD_encode(273, 3, 1, *((int*) space_holder) );	
					out_file.write((char*)ifd_strip_offset,12);

					//rows_per_strip 
					unsigned char* ifd_rows_per_strip = IFD_encode(278, 4, 1, image_length);	
					out_file.write((char*)ifd_rows_per_strip,12);

					//strip_byte_counts 
					unsigned char* ifd_strip_byte_counts = IFD_encode(279, 4, 1, image_length*image_width);	
					out_file.write((char*)ifd_strip_byte_counts,12);

					//x resolution 
					unsigned char ifd_x_resolution_content[8] = {
						((unsigned char*)&TiffRead::x_resolution_numerator_)[3], 
						((unsigned char*)&TiffRead::x_resolution_numerator_)[2], 
						((unsigned char*)&TiffRead::x_resolution_numerator_)[1],
						((unsigned char*)&TiffRead::x_resolution_numerator_)[0],
						
						((unsigned char*)&TiffRead::x_resolution_denominator_)[3], 
						((unsigned char*)&TiffRead::x_resolution_denominator_)[2], 
						((unsigned char*)&TiffRead::x_resolution_denominator_)[1],
						((unsigned char*)&TiffRead::x_resolution_denominator_)[0],
					};
					unsigned char* ifd_x_resolution_address = IFD_encode(282, 5, 1, *((int*)starting_offset_value_address_bytes));	
					out_file.write((char*)ifd_x_resolution_address,12);
					int next_offset_value_address = write_on_address(out_file, *((int*)starting_offset_value_address_bytes), ifd_x_resolution_content, 8);

					//y resolution 
					unsigned char ifd_y_resolution_content[8] = {
						((unsigned char*)&TiffRead::y_resolution_numerator_)[3], 
						((unsigned char*)&TiffRead::y_resolution_numerator_)[2], 
						((unsigned char*)&TiffRead::y_resolution_numerator_)[1],
						((unsigned char*)&TiffRead::y_resolution_numerator_)[0],
						
						((unsigned char*)&TiffRead::y_resolution_denominator_)[3], 
						((unsigned char*)&TiffRead::y_resolution_denominator_)[2], 
						((unsigned char*)&TiffRead::y_resolution_denominator_)[1],
						((unsigned char*)&TiffRead::y_resolution_denominator_)[0],
					};
					unsigned char* ifd_y_resolution_address = IFD_encode(283, 5, 1, next_offset_value_address);	
					out_file.write((char*)ifd_y_resolution_address,12);
					next_offset_value_address = write_on_address(out_file, next_offset_value_address, ifd_y_resolution_content, 8);
					int end_of_jumped_address = next_offset_value_address;
					//resolution unit

					unsigned char ifd_resolution_unit_content[4] =  {0x00, 0x00, ((unsigned char*)&TiffRead::resolution_unit_)[0], 0x00};
					unsigned char* ifd_resolution_unit = IFD_encode(296, 3, 1, *((int*)ifd_resolution_unit_content));	
					out_file.write((char*)ifd_resolution_unit,12);

					//go back and fill strip_offset ifd

					char strip_offset_content[4] = {
						((char*)&end_of_jumped_address)[0], 
						((char*)&end_of_jumped_address)[1],
						((char*)&end_of_jumped_address)[2],
						((char*)&end_of_jumped_address)[3]};	
					ifd_strip_offset = IFD_encode(273, 4, 1, *((int*)strip_offset_content) );
					write_on_address(out_file, strip_offset_address, ifd_strip_offset, 12);
					#ifdef DEBUG	
				/*	
						std::cout<<"=====DEBUG  INFO====="<<std::endl;
						std::cout<<"ifd:0x ";
						for(int i =0; i< 12; i++){

							std::cout<<std::hex<<std::setfill('0')<<std::setw(2)<<(int)(ifd_image_width[i])<<" ";

						}
						std::cout<<std::endl;
						std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;	
				*/	
					#endif

					//========writing of gray image data==========

					//std::cout<<"end_of_jumped_address:"<<std::hex<<end_of_jumped_address<<std::endl;	
					out_file.seekp(end_of_jumped_address);
					int intensity;
					for(int i = y0; i<yc; i++){
						for(int j = x0; j<xc; j++){
							intensity =  ((int)checkImage[TiffRead::image_length_-i-1][j][0] + (int)checkImage[TiffRead::image_length_-i-1][j][1]+ (int)checkImage[TiffRead::image_length_-i-1][j][2])/3;
							
							char intensity_byte = ((char*)&intensity)[0];

							if((i==y0)&&(j==x0)){
								std::cout<<"intensity:"<<std::hex<<(int)intensity_byte<<std::endl;
							}
							out_file.write(&intensity_byte,1);
						}
					}	


				}
				else{//rgb image
					unsigned char ifd_num[2] ={0x00,0xc};
					out_file.write((char*)ifd_num, 2);
					if(TiffRead::photo_metric_==3){//Palette Color


						unsigned char starting_offset_value_address_bytes[4] = {0x9a,0x00,0x00,0x00};

						//image width
						int image_width = xc-x0+1;
						unsigned char* ifd_image_width = IFD_encode(256, 4, 1, image_width);
						out_file.write((char*)ifd_image_width,12);	

						//image length
						int image_length = yc-y0+1;
						unsigned char* ifd_image_length = IFD_encode(257, 4, 1, image_length);	
						out_file.write((char*)ifd_image_length,12);	

						//bits per sample
						char bits_per_sample[4] = {0x00, 0x00,0x08,0x00};	
						unsigned char* ifd_bits_per_sample = IFD_encode(258, 3, 1, *((int*)bits_per_sample));	
						out_file.write((char*)ifd_bits_per_sample,12);	

						//compression
						unsigned char compression_content[4] =  {0x00, 0x00, ((unsigned char*)&TiffRead::compression_)[0], 0x00};
						unsigned char* ifd_compression = IFD_encode(259, 3, 1, *((int*)compression_content));	
						out_file.write((char*)ifd_compression,12);

						//photometric_intepretation					
						unsigned char photo_metric_content[4] =  {0x00, 0x00, ((unsigned char*)&TiffRead::photo_metric_)[0], 0x00};
						unsigned char* ifd_photo_metric = IFD_encode(262, 3, 1, *((int*)photo_metric_content));	
						out_file.write((char*)ifd_photo_metric,12);

						//hold the space for strip_offset
						int strip_offset_address = out_file.tellp();
						unsigned char space_holder[4] = {0x00,0x00,0x00,0x00};						
						unsigned char* ifd_strip_offset = IFD_encode(273, 3, 1, *((int*) space_holder) );	
						out_file.write((char*)ifd_strip_offset,12);

						//rows_per_strip 
						unsigned char* ifd_rows_per_strip = IFD_encode(278, 4, 1, image_length);	
						out_file.write((char*)ifd_rows_per_strip,12);

						//strip_byte_counts 
						unsigned char* ifd_strip_byte_counts = IFD_encode(279, 4, 1, image_length*image_width);	
						out_file.write((char*)ifd_strip_byte_counts,12);

						//x resolution 
						/*
						unsigned char ifd_x_resolution_content[8] = {
							((unsigned char*)&TiffRead::x_resolution_numerator_)[3], 
							((unsigned char*)&TiffRead::x_resolution_numerator_)[2], 
							((unsigned char*)&TiffRead::x_resolution_numerator_)[1],
							((unsigned char*)&TiffRead::x_resolution_numerator_)[0],
							
							((unsigned char*)&TiffRead::x_resolution_denominator_)[3], 
							((unsigned char*)&TiffRead::x_resolution_denominator_)[2], 
							((unsigned char*)&TiffRead::x_resolution_denominator_)[1],
							((unsigned char*)&TiffRead::x_resolution_denominator_)[0],
						};*/
						unsigned char ifd_x_resolution_content[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x01};
						unsigned char* ifd_x_resolution_address = IFD_encode(282, 5, 1, *((int*)starting_offset_value_address_bytes));	
						out_file.write((char*)ifd_x_resolution_address,12);
						int next_offset_value_address = write_on_address(out_file, *((int*)starting_offset_value_address_bytes), ifd_x_resolution_content, 8);

						//y resolution
						/* 
						unsigned char ifd_y_resolution_content[8] = {
							((unsigned char*)&TiffRead::y_resolution_numerator_)[3], 
							((unsigned char*)&TiffRead::y_resolution_numerator_)[2], 
							((unsigned char*)&TiffRead::y_resolution_numerator_)[1],
							((unsigned char*)&TiffRead::y_resolution_numerator_)[0],
							
							((unsigned char*)&TiffRead::y_resolution_denominator_)[3], 
							((unsigned char*)&TiffRead::y_resolution_denominator_)[2], 
							((unsigned char*)&TiffRead::y_resolution_denominator_)[1],
							((unsigned char*)&TiffRead::y_resolution_denominator_)[0],
						};*/

						unsigned char ifd_y_resolution_content[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x01};
						unsigned char* ifd_y_resolution_address = IFD_encode(283, 5, 1, next_offset_value_address);	
						out_file.write((char*)ifd_y_resolution_address,12);
						next_offset_value_address = write_on_address(out_file, next_offset_value_address, ifd_y_resolution_content, 8);
						//resolution unit
						unsigned char ifd_resolution_unit_content[4] =  {0x00, 0x00, ((unsigned char*)&TiffRead::resolution_unit_)[0], 0x00};
						unsigned char* ifd_resolution_unit = IFD_encode(296, 3, 1, *((int*)ifd_resolution_unit_content));	
						out_file.write((char*)ifd_resolution_unit,12);

						//color map
						unsigned char* ifd_color_map_address = IFD_encode(320, 3, 768, next_offset_value_address);
						out_file.write((char*)ifd_color_map_address, 12);
						for (int i=0; i<256;i++){
							//char value_byte = ((char*)&(TiffRead::r_color_map[i]))[1]
							//std::cout<<"r color map:"<<TiffRead::r_color_map_[i]
							next_offset_value_address = write_on_address(out_file, next_offset_value_address, &(((unsigned char*)&(TiffRead::r_color_map_[i]))[0]), 2);
						}
						for (int i=0; i<256;i++){
							next_offset_value_address = write_on_address(out_file, next_offset_value_address, &(((unsigned char*)&(TiffRead::g_color_map_[i]))[0]), 2);
						}
						for (int i=0; i<256;i++){
							next_offset_value_address = write_on_address(out_file, next_offset_value_address, &(((unsigned char*)&(TiffRead::b_color_map_[i]))[0]), 2);
						}


						int end_of_jumped_address = next_offset_value_address;
						std::cout<<"end_of_jumped_address:"<<end_of_jumped_address<<std::endl;

						//go back and fill strip_offset ifd

						char strip_offset_content[4] = {
							((char*)&end_of_jumped_address)[0], 
							((char*)&end_of_jumped_address)[1],
							((char*)&end_of_jumped_address)[2],
							((char*)&end_of_jumped_address)[3]};	
						ifd_strip_offset = IFD_encode(273, 4, 1, *((int*)strip_offset_content) );
						write_on_address(out_file, strip_offset_address, ifd_strip_offset, 12);

						#ifdef DEBUG	
						/*
							std::cout<<"=====DEBUG  INFO====="<<std::endl;
							std::cout<<"ifd:0x ";
							for(int i =0; i< 12; i++){

								std::cout<<std::hex<<std::setfill('0')<<std::setw(2)<<(int)(ifd_image_width[i])<<" ";

							}
							std::cout<<std::endl;
							std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;	
						*/
						#endif

						//========writing of color map image data==========

						std::cout<<"end_of_jumped_address:"<<std::hex<<end_of_jumped_address<<std::endl;	
						out_file.seekp(end_of_jumped_address);

						std::string buffer_file = "imageData.dat";//"imageData.dat";
						int length =// 3;
										TiffRead::image_length_;
						int width = //4;
										TiffRead::image_width_;
						int i_min = //0;
										y0;
						int i_max = //2;
										yc; 
						int j_min = //0;
										x0;
						int j_max = //3;
										xc;
				
						char image_data[length][width];
						char byte;
						std::ifstream image_data_file(buffer_file,std::ios::binary); 
						for(int i = 0; i<length; i++){
							for(int j = 0; j<width; j++){
								image_data_file.read(&byte,1);
								image_data[i][j]=byte;
							}
						}
	
						for(int i = 0; i<length; i++){
							for(int j = 0; j<width; j++){
								//std::cout<<"data:"<<std::hex<<(int)image_data[i][j]<<std::endl;;
							}
						}	
						for(int i = i_max; i>=i_min; i--){
							for(int j = j_min; j<=j_max; j++){
						//		std::cout<<"i:"<<i<<",j:"<<j<<", out data:"<<std::hex<<(int)image_data[length-1-i][j]<<std::endl;
								byte = image_data[length-1-i][j];
								out_file.write(&byte,1);
							}
						}
						image_data_file.close();
												
/*
						char image_data[TiffRead::image_length_][TiffRead::image_width_];
						char byte;
						std::ifstream image_data_file("imageData.dat",std::ios::binary); 
						for(int i = 0; i<TiffRead::image_length_; i++){
							for(int j = 0; j<TiffRead::image_width_; j++){
								image_data_file.read(&byte,1);
								image_data[i][j]=byte;
							}
						}

						for(int i = yc;i>=y0;i--){
							for(int j = x0; j<=xc; j++){
								byte = image_data[TiffRead::image_length_-1-i][j];
								out_file.write(&byte,1);
							}
						}
						image_data_file.close();
*/


/*
//						std::vector<int[]> combined_color_map;
						for(int i =0; i<256; i++){
					//		combined_color_map.push_back([1,1,1]);

						}
	
						for(int i = y0; i<yc; i++){
							for(int j = x0; j<xc; j++){
			
								int r = checkImage[TiffRead::image_length_-i][j][0];
								int g = checkImage[TiffRead::image_length_-i][j][0];
								int b = checkImage[TiffRead::image_length_-i][j][0];
								//int index = Util::find_index(TiffRead::r_color_map_,r*257);
								//unsigned char value = ((unsigned char*)&index)[0];
*/								
//	
							//	if((i==y0)&&(j==x0)){
								   
									/*	
									std::vector<int> r_index_vector = Util::find_index(TiffRead::r_color_map_,r*257);
									std::vector<int> g_index_vector = Util::find_index(TiffRead::r_color_map_,g*257);
									std::vector<int> b_index_vector = Util::find_index(TiffRead::r_color_map_,b*257);
							
									std::vector<int> first_common_vector;
									std::vector<int> second_common_vector;
									for(std::vector<int>::iterator r_it = r_index_vector.begin(); r_it!= r_index_vector.end(); ++r_it){
										first_common_vector = Util::find_index(g_index_vector,*r_it);
										for(std::vector<int>::iterator first_it = first_common_vector.begin(); first_it!= first_common_vector.end();++first_it){
											//second_common_vector = Util::find_index(b_index_vector,*first_it);
										}
									}*/
								//std::cout<<"final index is: "<<second_common_vector[0]<<",vector size:"<<second_common_vector.size()<<std::endl;

		//Util::find_common_index(r_vector,g_vector, b_vector);
	//								std::cout<<"index for "<<std::dec<< (int)(r*257) << " is: "<<Util::find_common_index(r_vector, g_vector, b_vector)<<std::endl;
							//	}
								//out_file.write((char*)&value,1);
							//}
						//}	

	

					}
					else if(TiffRead::photo_metric_==2){//full rgb imag
						unsigned char starting_offset_value_address_bytes[4] = {0x9a,0x00,0x00,0x00};

						//image width
						int image_width = xc-x0+1;
						unsigned char* ifd_image_width = IFD_encode(256, 4, 1, image_width);
						out_file.write((char*)ifd_image_width,12);	

						//image length
						int image_length = yc-y0+1;
						unsigned char* ifd_image_length = IFD_encode(257, 4, 1, image_length);	
						out_file.write((char*)ifd_image_length,12);	

						//bits per sample
						unsigned char* ifd_bits_per_sample = IFD_encode(258, 3, 3, *((int*)starting_offset_value_address_bytes));	
						out_file.write((char*)ifd_bits_per_sample,12);	
						unsigned char bits_per_sample_content[6] = {0x00,0x08,0x00,0x08,0x00,0x08};
						int next_offset_value_address = write_on_address(out_file, *((int*)starting_offset_value_address_bytes), bits_per_sample_content, 6);
						//compression
						unsigned char compression_content[4] =  {0x00, 0x00, ((unsigned char*)&TiffRead::compression_)[0], 0x00};
						unsigned char* ifd_compression = IFD_encode(259, 3, 1, *((int*)compression_content));	
						out_file.write((char*)ifd_compression,12);

						//photometric_intepretation					
						unsigned char photo_metric_content[4] =  {0x00, 0x00, ((unsigned char*)&TiffRead::photo_metric_)[0], 0x00};
						unsigned char* ifd_photo_metric = IFD_encode(262, 3, 1, *((int*)photo_metric_content));	
						out_file.write((char*)ifd_photo_metric,12);

						//hold the space for strip_offset
						int strip_offset_address = out_file.tellp();
						unsigned char space_holder[4] = {0x00,0x00,0x00,0x00};						
						unsigned char* ifd_strip_offset = IFD_encode(273, 3, 1, *((int*) space_holder) );	
						out_file.write((char*)ifd_strip_offset,12);

						//samples_per_pixel
						unsigned char samples_per_pixel_content[4] = {0x00,0x00,0x03,0x00};
						unsigned char* ifd_samples_per_pixel = IFD_encode(277, 3, 1, *((int*)samples_per_pixel_content));	
						out_file.write((char*)ifd_samples_per_pixel,12);

						//rows_per_strip 
						unsigned char* ifd_rows_per_strip = IFD_encode(278, 4, 1, image_length);	
						out_file.write((char*)ifd_rows_per_strip,12);

						//strip_byte_counts 
						unsigned char* ifd_strip_byte_counts = IFD_encode(279, 4, 1, image_length*image_width);	
						out_file.write((char*)ifd_strip_byte_counts,12);

						//x resolution 
						unsigned char ifd_x_resolution_content[8] = {
							((unsigned char*)&TiffRead::x_resolution_numerator_)[3], 
							((unsigned char*)&TiffRead::x_resolution_numerator_)[2], 
							((unsigned char*)&TiffRead::x_resolution_numerator_)[1],
							((unsigned char*)&TiffRead::x_resolution_numerator_)[0],
							
							((unsigned char*)&TiffRead::x_resolution_denominator_)[3], 
							((unsigned char*)&TiffRead::x_resolution_denominator_)[2], 
							((unsigned char*)&TiffRead::x_resolution_denominator_)[1],
							((unsigned char*)&TiffRead::x_resolution_denominator_)[0],
						};
						unsigned char* ifd_x_resolution_address = IFD_encode(282, 5, 1, next_offset_value_address);	
						out_file.write((char*)ifd_x_resolution_address,12);
						next_offset_value_address = write_on_address(out_file, next_offset_value_address, ifd_x_resolution_content, 8);

						//y resolution 
						unsigned char ifd_y_resolution_content[8] = {
							((unsigned char*)&TiffRead::y_resolution_numerator_)[3], 
							((unsigned char*)&TiffRead::y_resolution_numerator_)[2], 
							((unsigned char*)&TiffRead::y_resolution_numerator_)[1],
							((unsigned char*)&TiffRead::y_resolution_numerator_)[0],
							
							((unsigned char*)&TiffRead::y_resolution_denominator_)[3], 
							((unsigned char*)&TiffRead::y_resolution_denominator_)[2], 
							((unsigned char*)&TiffRead::y_resolution_denominator_)[1],
							((unsigned char*)&TiffRead::y_resolution_denominator_)[0],
						};
						unsigned char* ifd_y_resolution_address = IFD_encode(283, 5, 1, next_offset_value_address);	
						out_file.write((char*)ifd_y_resolution_address,12);
						next_offset_value_address = write_on_address(out_file, next_offset_value_address, ifd_y_resolution_content, 8);
						int end_of_jumped_address = next_offset_value_address;
						//resolution unit

						unsigned char ifd_resolution_unit_content[4] =  {0x00, 0x00, ((unsigned char*)&TiffRead::resolution_unit_)[0], 0x00};
						unsigned char* ifd_resolution_unit = IFD_encode(296, 3, 1, *((int*)ifd_resolution_unit_content));	
						out_file.write((char*)ifd_resolution_unit,12);

						//go back and fill strip_offset ifd

						char strip_offset_content[4] = {
							((char*)&end_of_jumped_address)[0], 
							((char*)&end_of_jumped_address)[1],
							((char*)&end_of_jumped_address)[2],
							((char*)&end_of_jumped_address)[3]};	
						ifd_strip_offset = IFD_encode(273, 4, 1, *((int*)strip_offset_content) );
						write_on_address(out_file, strip_offset_address, ifd_strip_offset, 12);
						#ifdef DEBUG	
						/*
							std::cout<<"=====DEBUG  INFO====="<<std::endl;
							std::cout<<"ifd:0x ";
							for(int i =0; i< 12; i++){

								std::cout<<std::hex<<std::setfill('0')<<std::setw(2)<<(int)(ifd_image_width[i])<<" ";

							}
							std::cout<<std::endl;
							std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;	
						*/
						#endif

						//========writing of full rgb image data==========

						//std::cout<<"end_of_jumped_address:"<<std::hex<<end_of_jumped_address<<std::endl;	
						out_file.seekp(end_of_jumped_address);
						unsigned char r;
						unsigned char g;
						unsigned char b;

						for(int i = y0; i<yc; i++){
							for(int j = x0; j<xc; j++){
								r = checkImage[TiffRead::image_length_-i-1][j][0];
								g = checkImage[TiffRead::image_length_-i-1][j][1];
								b = checkImage[TiffRead::image_length_-i-1][j][2];
								

								unsigned char triplet[3] = {r,g,b};
	
								if((i==y0)&&(j==x0)){
									std::cout<<"triplet:"<<std::hex<<*((int*)triplet)<<std::endl;
								}
								out_file.write((char*)triplet,3);
							}
						}	

	
					}
				}		


				out_file.close();
				std::cout<<"Write to <" <<filename<<"> successfully"<<std::endl;
			}
		}
		else{//params <5 but !=5
			result = "too few params!";
		}
		
	}
	else{//params >5

		result =  "too many parameters";
	}
	return result;

}

unsigned char*
TiffWrite::IFD_encode(short tag_no, short type, int count, int value_offset){
	static unsigned char encoded_ifd[12]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};	
	encoded_ifd[0] = ((unsigned char*)&tag_no)[1];
	encoded_ifd[1] = ((unsigned char*)&tag_no)[0];
	encoded_ifd[2] = ((unsigned char*)&type)[1];
	encoded_ifd[3] = ((unsigned char*)&type)[0];
	encoded_ifd[4] = ((unsigned char*)&count)[3];
	encoded_ifd[5] = ((unsigned char*)&count)[2];
	encoded_ifd[6] = ((unsigned char*)&count)[1];
	encoded_ifd[7] = ((unsigned char*)&count)[0];
	encoded_ifd[8] = ((unsigned char*)&value_offset)[3];
	encoded_ifd[9] = ((unsigned char*)&value_offset)[2];	
	encoded_ifd[10] = ((unsigned char*)&value_offset)[1];
	encoded_ifd[11] = ((unsigned char*)&value_offset)[0];

	#ifdef DEBUG
	
		std::cout<<"=====DEBUG  INFO====="<<std::endl;
		std::cout<<"encoded ifd:0x";
		for(int i =0; i< 12; i++){

			std::cout<<std::hex<<std::setfill('0')<<std::setw(2)<<(int)encoded_ifd[i]<<" ";

		}
		std::cout<<std::endl;

		std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;	

	#endif
	return encoded_ifd;
}

int
TiffWrite::write_on_address(std::ofstream& file, int address, unsigned char* content, int n){
	int previous_address = file.tellp();
	file.seekp(address);
	file.write((char*)content, n);
	int after_address = file.tellp();
	file.seekp(previous_address);
	return after_address;
}

