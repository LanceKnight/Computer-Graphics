#include <cstring>
#include <fstream>
#include <GL/glut.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "Dispatcher.hh"
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
	char buffer[3];
	short magicno;
	int ifdaddress;
	short ifdcount;
	unsigned char IFD[13];

	if(paramList.size()<=5){// params <=5
		if(paramList.size()==5){
			std::ifstream file;
			std::string filename = *paramList.begin();
			std::string x0 = paramList[1];
			std::string y0 = paramList[2];
			std::string xc = paramList[3];
			std::string yc = paramList[4];
//			std::string command; //the command tiffstat from lines of the file.
			file.open(filename, std::ios::binary);
		  	if (!file) {
				result = "Unable to open "+ filename;
				
			}
			else{
				std::cout<<filename<<std::endl;			
				file.seekg(0,std::ios::beg);
				
				file.read(buffer,2);

				file.read((char*)&magicno,2);

				file.read((char*)&ifdaddress,4);


				const unsigned char LITTLE[3] = {0x49, 0x49,0x00};
				const unsigned char BIG[3] = {0x4d,0x4d,0x00};


				#ifdef DEBUG
/*
					std::cout<<"=====DEBUG  INFO====="<<std::endl;
					std::cout<<"Is machine little-endian? " <<std::boolalpha<< Util::isLittleEndian() <<std::endl;
					std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
*/
				#endif

				//std::cout<< "buffer: 0x"<<std::hex<<(int)buffer[0] <<(int)buffer[1];
				if(!memcmp((void*)buffer,(void*)LITTLE,2)){
					std::cout<<"<Little-Endian>"<<std::endl;
				}
				else if(!memcmp((void*)buffer,(void*)BIG,2)){	
					std::cout<<"<Big-Endian>"<<std::endl;
				}
				else{
					std::cout<<"<Invalid Endianness>"<<std::endl;
				}	

				
				if((!memcmp((void*)buffer,(void*)LITTLE,2))||(!memcmp((void*)buffer,(void*)BIG,2))){// if buffer value is little or big Endian
					bool should_reverse = (Util::isLittleEndian()) != (!memcmp((void*)buffer, (void*)LITTLE, 2));
					if(should_reverse){	
						#ifdef DEBUG
/*
							std::cout<<"=====DEBUG  INFO====="<<std::endl;
							std::cout<<"reversing"<<std::endl;
							std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
*/
						#endif
						Util::reverse((unsigned char*)&magicno,2);	
						Util::reverse((unsigned char*)&ifdaddress,4);
					}
					const unsigned char TIFF_FORMAT[3] = {0x2a, 0x00, 0x00};
					if(memcmp((void*)&magicno,(void*)TIFF_FORMAT,2)){//if magic number is not 42
						std::cout<< "magicno: 0x"<<magicno <<std::endl;
						return "This is not a TIFF file";
					}
					else{//if magic number is 42
		
						file.seekg(ifdaddress,std::ios::beg);
						
						file.read((char*)&ifdcount,2);

						if((Util::isLittleEndian()) && (memcmp((void*)buffer, (void*)LITTLE, 2))){ 
							Util::reverse((unsigned char*)&ifdcount,2);
						}

//						std::cout<< "magicno: 0x"<<magicno <<std::endl;
//						std::cout<< "IFD address: 0x" <<std::setfill('0')<<std::setw(8)<<ifdaddress <<std::endl;	
//						std::cout<< "IFD count: 0x" <<ifdcount <<std::endl;
						
						int file_position;
						for(int i =0; i<ifdcount; i++){
							file.read((char *)IFD, 12);
							file_position = file.tellg();
							IFD[12] = '\0';
							IFD_intepret(IFD, should_reverse, file);
							file.seekg(file_position);
						}

//==========================================

					#ifdef DEBUG
/*
						std::cout<<"=====DEBUG  INFO====="<<std::endl;
						std::cout<<"ImageLength:"<<std::dec<<image_length_<<std::endl;
						std::cout<<"ImageWidth:"<<image_width_<<std::endl;
						std::cout<<"RowsPerStrip:"<<rows_per_strip_<<std::endl;
						std::cout<<"StripsPerImage:"<<strips_per_image_<<std::endl;
						std::cout<<"StripOffsets:";
						for(int i=0; i<strips_per_image_;i++){
							std::cout<<strip_offsets_[i]<<" ";

						}
						std::cout<<std::endl;

						std::cout<<"StripByteCounts:";
						for(int i=0; i<strips_per_image_;i++){
							std::cout<<strip_byte_counts_[i]<<" ";

						}
						std::cout<<std::endl;

						std::cout<<"BitsPerSample:";
						for(unsigned int i=0; i<bits_per_sample_.size();i++){
							std::cout<<bits_per_sample_[i]<<" ";

						}
						std::cout<<std::endl;
					
						std::cout<<"PhotometriIntepretation:"<<photo_metric_<<std::endl;

						std::cout<<"ColorMap:";
						for(unsigned int i=0; i<r_color_map_.size();i++){
							std::cout<<std::dec<<((double)r_color_map_[i]/65535)*255<<" ";

						}
						std::cout<<std::endl;

						std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
*/
					#endif
						display_image(file, should_reverse);

						file.close();
						result = "Display file <"+filename+"> successfully";


					}
				}
				else{//if buffer value is either little nor big Endian
					result = "This is not a valid TIFF file";
				}
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

void
TiffWrite::IFD_intepret(unsigned char* IFD, bool should_reverse,  std::ifstream & file){

	#ifdef DEBUG
/*
		std::cout<<"=====DEBUG  INFO====="<<std::endl;
		std::cout <<"raw IDF: 0x ";
		for(int j = 0; j < 12; j++){			
			std::cout << std::setfill('0')<<std::setw(2)<<std::hex<<(int)IFD[j]<<" ";
		}
		std::cout<<std::endl;

		std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
*/
	#endif

	unsigned char ifd_tag[3];
	unsigned char ifd_type[3];
	unsigned char ifd_count[5];
	unsigned char ifd_value_offset[5];

	memcpy((void*)&ifd_tag, (void *)IFD, 3);
	ifd_tag[2] = '\0';
	
	memcpy((void*)ifd_type, (void *)(IFD+2), 3);
	ifd_type[2] = '\0';

	
	memcpy((void*)ifd_count, (void *)(IFD+4), 5);
	ifd_count[4] = '\0';


	memcpy((void*)ifd_value_offset, (void *)(IFD+8), 5);
	ifd_value_offset[4] = '\0';
	
	if(should_reverse){
		Util::reverse(ifd_tag, 2); 
		Util::reverse(ifd_type,2);	
		Util::reverse(ifd_count,4);
	}


	#ifdef DEBUG
/*
		std::cout<<"=====DEBUG  INFO====="<<std::endl;
		std::cout<<"Endian-ness adjusted IFD 0x ";
		for (int i = 0; i< 2;i++){
			std::cout<<std::setfill('0')<<std::setw(2)<<std::hex<<(int) ifd_tag[i] <<" ";
		}
		for (int i = 0; i< 2;i++){
			std::cout<<std::setfill('0')<<std::setw(2)<<std::hex<<(int) ifd_type[i] <<" ";
		}
		for (int i = 0; i< 4;i++){
			std::cout<<std::setfill('0')<<std::setw(2)<<std::hex<<(int) ifd_count[i] <<" ";
		}
		for (int i = 0; i< 4;i++){
			std::cout<<std::setfill('0')<<std::setw(2)<<std::hex<<(int) ifd_value_offset[i] <<" ";
		}
		std::cout<<std::endl;
		std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
*/
	#endif
		

	//std::cout<<tag_intepret(*((short*)ifd_tag))<<" ";
	//std::cout<<type_intepret(*((short*)ifd_type))<<"("<<std::dec<<* ((short*)ifd_type)<<")"<<" ";
	//std::cout<<std::dec<<*((short*)ifd_count)<<"<";

	int type_length_multip_count = (type_length_intepret(*((short*)ifd_type))*(*((short*)ifd_count)));

	if(type_length_multip_count>4){

		if(should_reverse){//if format is Big-Endness
			Util::reverse(ifd_value_offset,4);
		}
	

		int ifd_position;
		ifd_position = file.tellg();
		file.seekg(*((int *)ifd_value_offset),std::ios::beg);
		unsigned char data_array[type_length_multip_count+1]; 
		file.read((char*)data_array,type_length_multip_count);
		data_array[type_length_multip_count]='\0';
		type_output_intepret(*((short*)ifd_tag), *((short*)ifd_type), data_array, (*(short*)ifd_count), should_reverse);
		file.seekg(ifd_position, std::ios::beg);
		#ifdef DEBUG
/*
			std::cout<<"=====DEBUG  INFO====="<<std::endl;
			std::cout<<"#: "<<type_length_multip_count<<std::endl;	
			std::cout<<"length > 4"<<std::endl;
			std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
*/
		#endif

		if(*((short*)ifd_tag)==257){// if tag is ImageLength
			std::cout<<"sorry, the program only supports image smaller than size of 1024*1024."<<std::endl;
		}

		if(*((short*)ifd_tag)==256){// if tag is ImageWidth
			std::cout<<"sorry, the program only supports image smaller than size of 1024*1024."<<std::endl;
		}
		//TODO if tag is 278 RowsPerStrip
	}
	else{//if type_length_multip_count <=4
		if(*((short*)ifd_type) == 3){//type short
			if(*((short *)ifd_count)==1){//if count ==1
				unsigned char value[2];

				if(should_reverse){
					for(int i =0; i<2; i++){
						value[i] = ifd_value_offset[1-i];
					}
					//std::cout<<std::dec<<*((short*)value)<<">"<<std::endl;
				}
				else{
					for(int i =0; i<2; i++){
						value[i] = ifd_value_offset[i];
					}
					//std::cout<<std::dec<<*((short*)value)<<">"<<std::endl;
				}
		
				if(*((short*)ifd_tag)==257){// if tag is ImageLength

					image_length_=*((short*)value);
					if(rows_per_strip_!=0){
						strips_per_image_ = (image_length_+rows_per_strip_-1)/rows_per_strip_;
					}

					//std::cout<<"debug:"<<image_length_<<std::endl;	
				}
				
				if(*((short*)ifd_tag)==256){// if tag is ImageWidth
					image_width_=*((short*)value);
				}
				
				if(*((short*)ifd_tag)==258){// if tag is BitsPerSample
					bits_per_sample_.resize(1);
					bits_per_sample_[0]=*((short*)value);
				}
				
				if(*((short*)ifd_tag)==262){// if tag is PhotometricIntepretation
					photo_metric_=*((short*)value);
				}
				if(*((short*)ifd_tag)==278){// if tag is RowsPerStrip
					rows_per_strip_=*((short*)value);

					if(image_length_!=0){
						strips_per_image_ = (image_length_+rows_per_strip_-1)/rows_per_strip_;
					}
				}
			}
			else{//count ==2
				if(should_reverse){
//					unsigned char value1[2]={ifd_value_offset[1], ifd_value_offset[0]};
//					unsigned char value2[2]={ifd_value_offset[3], ifd_value_offset[2]};
					//std::cout<<std::dec<<*((short*)value1)<<" "<<*((short*)value2)<<">"<<std::endl;
				}
				else{

//					unsigned char value1[2]={ifd_value_offset[0], ifd_value_offset[1]};
//					unsigned char value2[2]={ifd_value_offset[2], ifd_value_offset[3]};
					//std::cout<<std::dec<<*((short*)value1)<<" "<<*((short*)value2)<<">"<<std::endl;
				}
			}
			
		}
		else if((*((short*)ifd_type) == 2)||(*((short*)ifd_type) == 1)){//type byte or char
		
			if(*((short*)ifd_count) ==1){
//				unsigned char value1=ifd_value_offset[0];
				//std::cout<<std::hex<<value1<<">"<<std::endl;
			}
			else if((*((short*)ifd_count) ==2)){

//				unsigned char value1=ifd_value_offset[0];
//				unsigned char value2=ifd_value_offset[1];
				//std::cout<<std::hex<<value1<<" "<<value2<<">"<<std::endl;
			}
			else if((*((short*)ifd_count) ==3)){
//				unsigned char value1=ifd_value_offset[0];
//				unsigned char value2=ifd_value_offset[1];
//				unsigned char value3=ifd_value_offset[2];
				//std::cout<<std::hex<<value1<<" "<<value2<<" "<<value3<<">"<<std::endl;
			}
			else{
//				unsigned char value1=ifd_value_offset[0];
//				unsigned char value2=ifd_value_offset[1];
//				unsigned char value3=ifd_value_offset[2];
//				unsigned char value4=ifd_value_offset[3];

				//std::cout<<std::hex<<value1<<" "<<value2<<" "<<value3<<" "<<value4 <<">"<<std::endl;
			}
		
		}
		else if((*((short*)ifd_type) == 4)){//type long
		
			unsigned char value[4];
			if(should_reverse){
				for(int i =0; i<4; i++){
					value[i] = ifd_value_offset[3-i];
				}
				//std::cout<<std::dec<<*((int*) value) <<">"<<std::endl;
			}
			else{

				for(int i =0; i<4; i++){
					value[i] = ifd_value_offset[i];
				}
				//std::cout<<std::dec<<*((int*) value) <<">"<<std::endl;
							
			}

			if(*((short*)ifd_tag)==257){// if tag is ImageLength
				image_length_=*((int*)value);
				std::cout<<"imageLength:"<<image_length_<<std::endl;
				if(rows_per_strip_!=0){
					strips_per_image_ = (image_length_+rows_per_strip_-1)/rows_per_strip_;
				}
			}
			
			if(*((short*)ifd_tag)==256){// if tag is ImageWidth
				image_width_=*((int*)value);
			}


			if(*((short*)ifd_tag) == 273){
				strip_offsets_.resize(strips_per_image_);
				strip_offsets_[0] = *((int*)value);
			}

			if(*((short*)ifd_tag)==278){// if tag is RowsPerStrip
				rows_per_strip_=*((int*)value);

				if(image_length_!=0){
					strips_per_image_ = (image_length_+rows_per_strip_-1)/rows_per_strip_;
				}
			}
		
		}
		#ifdef DEBUG
/*
			std::cout<<"=====DEBUG  INFO====="<<std::endl;
			std::cout<<"#: "<<type_length_intepret(*((short*)ifd_type))*(*((short*)ifd_count))<<std::endl;

			for (int i = 0; i< 4;i++){
				std::cout<<std::setfill('0')<<std::setw(2)<<std::hex<<(int) ifd_value_offset[i] <<" ";
			}

			std::cout<<std::endl;
			std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
*/
		#endif
	
	}

}

std::string
TiffWrite::tag_intepret(short code){

	switch(code){
		case 254:
			return "NewSubfileType(" + std::to_string(code) +")";
		case 255:
			return "SubfileType("+ std::to_string(code) +")";
		case 256:
			return "ImageWidth("+ std::to_string(code) +")";
		case 257:
			return "ImageLength("+ std::to_string(code) +")";
		case 258:
			return "BitsPerSample("+ std::to_string(code) +")";
		case 259:
			return "Compression("+ std::to_string(code) +")";
		case 262:
			return "PhotometricInterpretation("+ std::to_string(code) +")";
		case 263:
			return "Threshholding("+ std::to_string(code) +")";
		case 264:
			return "CellWidth("+ std::to_string(code) +")" ;

		case 265:
			return "CellLength("+ std::to_string(code) +")";
		case 266:
			return "FillOrder("+ std::to_string(code) +")";
		case 269:
			return "DocumentName("+ std::to_string(code) +")";
		case 270:
			return "ImageDescription("+ std::to_string(code) +")";
		case 271:
			return "Make("+ std::to_string(code) +")";
		case 272:
			return "Model("+ std::to_string(code) +")";
		case 273:
			return "StripOffsets("+ std::to_string(code) +")";
		case 274:
			return "Orientation("+ std::to_string(code) +")";
		case 277:
			return "SamplesPerPixel("+ std::to_string(code) +")";
		case 278:
			return "RowsPerStrip("+ std::to_string(code) +")";
		case 279:
			return "StripByteCounts("+ std::to_string(code) +")";
		case 280:
			return "MinSampleValue("+ std::to_string(code) +")";
		case 281:
			return "MaxSampleValue("+ std::to_string(code) +")";
		case 282:
			return "XResolution("+ std::to_string(code) +")";
		case 283:
			return "YResolution(" + std::to_string(code) +")";
		case 284:
			return "PlanarConfiguration("+ std::to_string(code) +")";
		case 285:
			return "PageName("+ std::to_string(code) +")";
		case 286:
			return "XPosition("+ std::to_string(code) +")";
		case 287:
			return "YPosition("+ std::to_string(code) +")";
		case 288:
			return "FreeOffsets("+ std::to_string(code) +")";
		case 289:
			return "FreeByteCounts("+ std::to_string(code) +")";
		case 290:
			return "GrayResponseUnit("+ std::to_string(code) +")";
		case 291:
			return "GrayResponseCurve("+ std::to_string(code) +")";
		case 292:
			return "T4Options("+ std::to_string(code) +")";
		case 293:
			return "T6Options("+ std::to_string(code) +")";
		case 296:
			return "ResolutionUnit("+ std::to_string(code) +")";
		case 297:
			return "PageNumber("+ std::to_string(code) +")";
		case 301:
			return "TransferFunction("+ std::to_string(code) +")";
		case 305:
			return "Software("+ std::to_string(code) +")";
		case 306:
			return "DateTime("+ std::to_string(code) +")";
		case 315:
			return "Artist("+ std::to_string(code) +")";
		case 316:
			return "HostComputer("+ std::to_string(code) +")";
		case 317:
			return "Predictor("+ std::to_string(code) +")";
		case 318:
			return "WhitePoint("+ std::to_string(code) +")";
		case 319:
			return "PrimaryChromaticities("+ std::to_string(code) +")";
		case 320:
			return "ColorMap("+ std::to_string(code) +")";
		case 321:
			return "HalftoneHints("+ std::to_string(code) +")";
		case 322:
			return "TileWidth("+ std::to_string(code) +")";
		case 323:
			return "TileLength("+ std::to_string(code) +")";
		case 324:
			return "TileOffsets("+ std::to_string(code) +")";
		case 325:
			return "TileByteCounts("+ std::to_string(code) +")";
		case 332:
			return "InkSet("+ std::to_string(code) +")";
		case 333:
			return "InkNames("+ std::to_string(code) +")";
		case 334:
			return "NumberOfInks("+ std::to_string(code) +")";
		case 336:
			return "DotRange("+ std::to_string(code) +")";
		case 337:
			return "TargetPrinter("+ std::to_string(code) +")";
		case 338:
			return "ExtraSamples("+ std::to_string(code) +")";
		case 339:
			return "SampleFormat("+ std::to_string(code) +")";
		case 340:
			return "SMinSampleValue("+ std::to_string(code) +")";
		case 341:
			return "SMaxSampleValue("+ std::to_string(code) +")";
		case 342:
			return "TransferRange("+ std::to_string(code) +")";
		case 512:
			return "JPEGProc("+ std::to_string(code) +")";
		case 513:
			return "JPEGInterchangeFormat("+ std::to_string(code) +")";
		case 514:
			return "JPEGInterchangeFormatLngth("+ std::to_string(code) +")";
		case 515:
			return "JPEGRestartInterval("+ std::to_string(code) +")";
		case 517:
			return "JPEGLosslessPredictors("+ std::to_string(code) +")";
		case 518:
			return "JPEGPointTransforms("+ std::to_string(code) +")";
		case 519:
			return "JPEGQTables("+ std::to_string(code) +")";
		case 520:
			return "JPEGDCTables("+ std::to_string(code) +")";
		case 521:
			return "JPEGACTables("+ std::to_string(code) +")";
		case 529:
			return "YCbCrCoefficients("+ std::to_string(code) +")";
		case 530:
			return "YCbCrSubSampling("+ std::to_string(code) +")";
		case 531:
			return "YCbCrPositioning("+ std::to_string(code) +")";
		case 532:
			return "ReferenceBlackWhite("+ std::to_string(code) +")";
		case 33432:
			return "Copyright("+ std::to_string(code) +")";
		default:
			std::cout<<std::dec<<(unsigned short)code <<"(0x"<< std::hex<<code<<")";
			return "";


	}


}


std::string 
TiffWrite::type_intepret(short code){
	switch(code){
		case 1:
			return "Byte";
		case 2:
			return "Ascii";
		case 3:
			return "Short";
		case 4:
			return "Long";
		case 5:
			return "Rational";
		default:
			return "unknown type";
	}

}

int
TiffWrite::type_length_intepret(short code){
	switch(code){
		case 1:
			return 1;
		case 2:
			return 1;
		case 3:
			return 2;
		case 4:
			return 4;
		case 5:
			return 8;
		default:
			return -1;
	}

}

void
TiffWrite::type_output_intepret(short tag, short code, unsigned char *data_array, int n, bool should_reverse){
	if(tag==258){
		bits_per_sample_.resize(n,0);
	}
	if(tag==273){
		strip_offsets_.resize(n,0);
	}
	if(tag==279){
		strip_byte_counts_.resize(n,0);
	}
	
	bool too_much= false;


	switch(code){
		case 1:
			for (int i = 0;  i < n; i++){
				//std::cout<<"0x"<<std::hex<<(int)data_array[i]<<" ";
			}
			//std::cout<<">"<<std::endl;
			return;
		case 2:
			for (int i = 0;  i < n; i++){
				//std::cout<<data_array[i];
			}
			//std::cout<<">"<<std::endl;
			return;
		case 3:
			unsigned short short_num;
			unsigned char short_bytes[2];
			for(int k =0; k<n; k++){
				if(should_reverse){
					for(int i = 0;i<2;i++){
						short_bytes[i] = data_array[2*k+1-i];
					}
				}
				else{			
					for(int i = 0;i<2;i++){
						short_bytes[i] = data_array[2*k+i];
					}
				}
				short_num =*((short*)short_bytes);
				
				if(k>5){//if tag == colorMap
					too_much = true;
				}
				else{
					//std::cout<<std::dec<< short_num<<" ";
				}
				if(tag==258){// if tag is StripOffsets
					bits_per_sample_[k] = short_num;	
				}
				if(tag==320){//if tag is colorMap
					if((k>=0) &&(k<=255)){
						r_color_map_[k]= short_num;
					}
					else if((k>=256)&&(k<=511)){
						g_color_map_[k-256]= short_num;
					}
					else if((k>=512)&&(k<=767)){
						b_color_map_[k-512]= short_num;
					}
				}

			}
			if(too_much){//if tag == colorMap
				//std::cout<<"...";
			}
			//std::cout<<">"<<std::endl;
			return;


		case 4://long type

			unsigned int long_num;
			unsigned char long_bytes[4];


			for(int k =0; k<n; k++){
				if(should_reverse){
					for(int i = 0;i<4;i++){
						long_bytes[i] = data_array[4*k+3-i];
					}
				}
				else{			
					for(int i = 0;i<4;i++){
						long_bytes[i] = data_array[4*k+i];
					}
				}
				long_num =*((long*)long_bytes);
				
				//std::cout<<std::dec<< long_num<<" ";
				
				if(tag==273){// if tag is StripOffsets
					strip_offsets_[k] = long_num;	
				}
				
				if(tag==279){// if tag is StripByteCounts
					strip_byte_counts_[k] = long_num;		
				}
				


			}
			//std::cout<<">"<<std::endl;

			return;
		case 5://TODO
			

		#ifdef DEBUG
/*
			std::cout<<"=====DEBUG  INFO====="<<std::endl;

			std::cout<<"content in remote address: 0x";

			for (int i = 0; i< 8;i++){
				std::cout<<std::setfill('0')<<std::setw(2)<<std::hex<<(int) data_array[i] <<" ";
			}

			std::cout<<std::endl;
			std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
*/
		#endif
/*			unsigned int numerator;
			unsigned char numerator_bytes[4];
			unsigned int denominator;
			unsigned char  denominator_bytes[4];
			for(int k=0; k<n; k++){
				if(should_reverse){

					for(int i = 0;i<4;i++){
						numerator_bytes[i] = data_array[8*k+3-i];
					}
				}
				else{
					for(int i = 0;i<4;i++){
						numerator_bytes[i] = data_array[8*k+i];
					}
				}

				numerator =*((long*)numerator_bytes);
				if(should_reverse){

					for(int j =0; j<4;j++){
						denominator_bytes[j] = data_array[8*k+(3-j)+4];
					}
				}
				else{
					for(int j =0; j<4;j++){
						denominator_bytes[j] = data_array[8*k+j+4];
					}
				}
				denominator = *((long*)denominator_bytes);


				//std::cout<<std::dec<< numerator/denominator<<" ";
			}*/

			//std::cout<<">"<<std::endl;
			return;		
			
		default:
			std::cout<<"error in type"<<std::endl;
	}	
}

void
TiffWrite::display_image(std::ifstream & file, bool should_reverse){
   int i, j;
	//GLubyte checkImage[1024][1024][3];
   std::cout<<"displaying image..."<<std::endl;
	
	if(bits_per_sample_.size() ==1){//gray image
		int image_address = strip_offsets_[0];
		unsigned char gray[1];
		file.seekg(image_address,std::ios::beg);
		glutReshapeWindow(image_length_, image_width_);
		for (i = 0; i < image_length_; i++) {
			for (j = 0; j < image_width_; j++) {
				if(should_reverse){
					file.read((char*)gray,1);
				}
				else{
					file.read((char*)gray,1);
				}
				if(photo_metric_==1){
					checkImage[image_length_-i][j][0] = (GLubyte) gray[0];
					checkImage[image_length_-i][j][1] = (GLubyte) gray[0];
					checkImage[image_length_-i][j][2] = (GLubyte) gray[0];
				}
				else if(photo_metric_==0){//Whie is Zero
					checkImage[image_length_-i][j][0] = (GLubyte) 255-gray[0];
					checkImage[image_length_-i][j][1] = (GLubyte) 255-gray[0];
					checkImage[image_length_-i][j][2] = (GLubyte) 255-gray[0];
				}
				else if(photo_metric_==3){//Color Map
					checkImage[image_length_-i][j][0] = (GLubyte) r_color_map_[gray[0]];
					checkImage[image_length_-i][j][1] = (GLubyte) g_color_map_[gray[0]];
					checkImage[image_length_-i][j][2] = (GLubyte) b_color_map_[gray[0]];
				}

			}
		}
	}
	else if(bits_per_sample_.size()==3){//rgb image
		int image_address = strip_offsets_[0];
		char r[1];
		char g[1];
		char b[1];
		file.seekg(image_address,std::ios::beg);
		glutReshapeWindow(image_length_, image_width_);
		for (i = 0; i < image_length_; i++) {
			for (j = 0; j < image_width_; j++) {
				if(should_reverse){
					file.read(r,1);
					file.read(g,1);
					file.read(b,1);
				}
				else{
					file.read(r,1);
					file.read(g,1);
					file.read(b,1);
				}
				checkImage[image_length_-i][j][0] = (GLubyte) r[0];
				checkImage[image_length_-i][j][1] = (GLubyte) g[0];
				checkImage[image_length_-i][j][2] = (GLubyte) b[0];
			}
		}
	}
	else if(bits_per_sample_.size()==4){
		int image_address = strip_offsets_[0];
		char r[1];
		char g[1];
		char b[1];
		char skippable[1];
		file.seekg(image_address,std::ios::beg);
		glutReshapeWindow(image_length_, image_width_);
		for (i = 0; i < image_length_; i++) {
			for (j = 0; j < image_width_; j++) {
				if(should_reverse){
					file.read(r,1);
					file.read(g,1);
					file.read(b,1);
					file.read(skippable,1);
				}
				else{
					file.read(r,1);
					file.read(g,1);
					file.read(b,1);
					file.read(skippable,1);
				}
				checkImage[image_length_-i][j][0] = (GLubyte) r[0];
				checkImage[image_length_-i][j][1] = (GLubyte) g[0];
				checkImage[image_length_-i][j][2] = (GLubyte) b[0];
			}
		}


	}
	
	 
	display();

}
