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
			std::string x0 = paramList[1];
			std::string y0 = paramList[2];
			std::string xc = paramList[3];
			std::string yc = paramList[4];

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
				if(TiffRead::is_gray_image_){
					unsigned char ifd_num[2] ={0x00,0xb};

				}
				else{//rgb image
					unsigned char ifd_num[2] ={0x00,0xc};
					out_file.write((char*)ifd_num, 2);
					if(TiffRead::photo_metric_==3){//Palette Color

					}
					else if(TiffRead::photo_metric_==2){//full rgb imag
						unsigned char offset_value_address_bytes[4] = {0x00,0x00,0x00,0x9a};
						int offset_value_address =*((int*)offset_value_address_bytes);
						std::cout<<"address::"<<offset_value_address<<std::endl;

						//image length
						unsigned char* ifd_image_width = IFD_encode(256, 3, 1, TiffRead::image_width_);
						out_file.write((char*)ifd_image_width,12);	
						//image width
						unsigned char* ifd_image_length = IFD_encode(257, 3, 1, TiffRead::image_length_);	
						out_file.write((char*)ifd_image_length,12);	
						//bits per sample
						unsigned char bits_per_sample_address[4] ={0x00,0x00,0x00,0x00};
						unsigned char* ifd_bits_per_sample = IFD_encode(258, 3, 3, *((int*)bits_per_sample_address));	
						out_file.write((char*)ifd_bits_per_sample,12);	
						//compression 
						unsigned char* ifd_compression = IFD_encode(259, 3, 1, 1);	
						out_file.write((char*)ifd_compression,12);
						//photometric_intepretation					
						unsigned char* ifd_photo_metric = IFD_encode(262, 3, 1, TiffRead::photo_metric_);	
						out_file.write((char*)ifd_photo_metric,12);
						//strip_offset
						unsigned strip_offset_address[4]={0x00,0x00,0x00,0x00};
						unsigned char* ifd_strip_offset = IFD_encode(273, 3, 1, *((int*)strip_offset_address));	
						out_file.write((char*)ifd_strip_offset,12);
						//samples_per_pixel
						unsigned char* ifd_samples_per_pixel = IFD_encode(277, 3, 1, 3);	
						out_file.write((char*)ifd_samples_per_pixel,12);
						//rows_per_strip //needs to change
						unsigned char* ifd_rows_per_strip = IFD_encode(278, 3, 1, 1);	
						out_file.write((char*)ifd_rows_per_strip,12);
						//strip_byte_counts //needs to change
						unsigned char* ifd_strip_byte_counts = IFD_encode(279, 3, 1, 1);	
						out_file.write((char*)ifd_strip_byte_counts,12);
						//x resolution 
						unsigned char* ifd_x_resolution = IFD_encode(282, 5, 1, TiffRead::x_resolution_);	
						out_file.write((char*)ifd_x_resolution,12);
						//y resolution 
						unsigned char* ifd_y_resolution = IFD_encode(283, 5, 1, TiffRead::y_resolution_);	
						out_file.write((char*)ifd_y_resolution,12);
						//resolution unit
						unsigned char* ifd_resolution_unit = IFD_encode(296, 3, 1, TiffRead::resolution_unit_);	
						out_file.write((char*)ifd_resolution_unit,12);

						#ifdef DEBUG	
							std::cout<<"=====DEBUG  INFO====="<<std::endl;
							std::cout<<"ifd:0x ";
							for(int i =0; i< 12; i++){

								std::cout<<std::hex<<std::setfill('0')<<std::setw(2)<<(int)(ifd_image_width[i])<<" ";

							}
							std::cout<<std::endl;

							std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;	
						#endif
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
	std::cout<<"encoded ifd:0x";
	for(int i =0; i< 12; i++){

		std::cout<<std::hex<<std::setfill('0')<<std::setw(2)<<(int)encoded_ifd[i]<<" ";

	}
	std::cout<<std::endl;

	return encoded_ifd;
}

int
TiffWrite::write_on_address(std::ofstream file, int address, unsigned char* content, int n){

	return 1;
}

