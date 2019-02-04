#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "Dispatcher.hh"
#include "TiffStat.hh"
#include "Util.hh"


TiffStat::TiffStat(){}

TiffStat::~TiffStat(){}



std::string
TiffStat::tiff_stat(std::vector<std::string> paramList){
	std::string result;
	char buffer[3];
	short magicno;
	int ifdaddress;
	short ifdcount;
	unsigned char IFD[13];

	if(paramList.size()<=1){
		if(paramList.size()==1){
			std::ifstream file;
			std::string filename = *paramList.begin();
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
					std::cout<<"=====DEBUG  INFO====="<<std::endl;
					std::cout<<"Is machine little-endian? " <<std::boolalpha<< Util::isLittleEndian() <<std::endl;
					std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
				#endif

				std::cout<< "buffer: 0x"<<std::hex<<(int)buffer[0] <<(int)buffer[1];
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
							std::cout<<"=====DEBUG  INFO====="<<std::endl;
							std::cout<<"reversing"<<std::endl;
							std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
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

						std::cout<< "magicno: 0x"<<magicno <<std::endl;
						std::cout<< "IFD address: 0x" <<std::setfill('0')<<std::setw(8)<<ifdaddress <<std::endl;	
						std::cout<< "IFD count: 0x" <<ifdcount <<std::endl;
						
						int file_position;
						for(int i =0; i<ifdcount; i++){
							file.read((char *)IFD, 12);
							file_position = file.tellg();
							IFD[12] = '\0';
							IFD_intepret(IFD, should_reverse, file);
							file.seekg(file_position);
						}

						file.close();
						result = "Read stat of file <"+filename+"> successfully";

					}
				}
				else{//if buffer value is either little nor big Endian
					result = "This is not a valid TIFF file";
				}
			}
		}
		else{
			result = "No file specified!";
		}
		
	}
	else{

		result =  "too many parameters";
	}
	return result;

}

void
TiffStat::IFD_intepret(unsigned char* IFD, bool should_reverse,  std::ifstream & file){

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
	#endif
		

	std::cout<<tag_intepret(*((short*)ifd_tag))<<" ";
	std::cout<<type_intepret(*((short*)ifd_type))<<"("<<std::dec<<* ((short*)ifd_type)<<")"<<" ";
	std::cout<<std::dec<<*((short*)ifd_count)<<"<";

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
		type_output_intepret(*((short*)ifd_type), data_array, (*(short*)ifd_count), should_reverse);
		file.seekg(ifd_position, std::ios::beg);
		#ifdef DEBUG
/*
			std::cout<<"=====DEBUG  INFO====="<<std::endl;
			std::cout<<"#: "<<type_length_multip_count<<std::endl;	
			std::cout<<"length > 4"<<std::endl;
			std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
*/
		#endif

	}
	else{
		if(*((short*)ifd_type) == 3){//type short
			
			if(*((short *)ifd_count)==1){//if count ==1
				if(should_reverse){
					unsigned char value[2] ={ifd_value_offset[1], ifd_value_offset[0]};
					std::cout<<std::dec<<*((short*)value)<<">"<<std::endl;
				}
				else{
					unsigned char value[2] ={ifd_value_offset[0], ifd_value_offset[1]};
					std::cout<<std::dec<<*((short*)value)<<">"<<std::endl;
				}
			}
			else{//count ==2
				if(should_reverse){
					unsigned char value1[2]={ifd_value_offset[1], ifd_value_offset[0]};
					unsigned char value2[2]={ifd_value_offset[3], ifd_value_offset[2]};
					std::cout<<std::dec<<*((short*)value1)<<" "<<*((short*)value2)<<">"<<std::endl;
				}
				else{

					unsigned char value1[2]={ifd_value_offset[0], ifd_value_offset[1]};
					unsigned char value2[2]={ifd_value_offset[2], ifd_value_offset[3]};
					std::cout<<std::dec<<*((short*)value1)<<" "<<*((short*)value2)<<">"<<std::endl;
				}
			}
		}
		else if((*((short*)ifd_type) == 2)||(*((short*)ifd_type) == 1)){//type byte or char
		
			if(*((short*)ifd_count) ==1){
				unsigned char value1=ifd_value_offset[0];
				std::cout<<std::hex<<value1<<">"<<std::endl;
			}
			else if((*((short*)ifd_count) ==2)){

				unsigned char value1=ifd_value_offset[0];
				unsigned char value2=ifd_value_offset[1];
				std::cout<<std::hex<<value1<<" "<<value2<<">"<<std::endl;
			}
			else if((*((short*)ifd_count) ==3)){
				unsigned char value1=ifd_value_offset[0];
				unsigned char value2=ifd_value_offset[1];
				unsigned char value3=ifd_value_offset[2];
				std::cout<<std::hex<<value1<<" "<<value2<<" "<<value3<<">"<<std::endl;
			}
			else{
				unsigned char value1=ifd_value_offset[0];
				unsigned char value2=ifd_value_offset[1];
				unsigned char value3=ifd_value_offset[2];
				unsigned char value4=ifd_value_offset[3];

				std::cout<<std::hex<<value1<<" "<<value2<<" "<<value3<<" "<<value4 <<">"<<std::endl;
			}

		}
		else if((*((short*)ifd_type) == 4)){//type long
			if(should_reverse){
				unsigned char value[4] ={ifd_value_offset[3], ifd_value_offset[2], ifd_value_offset[1], ifd_value_offset[0]};
				std::cout<<std::dec<<*((int*) value) <<">"<<std::endl;
			}
			else{
				unsigned char value[4] ={ifd_value_offset[0], ifd_value_offset[1], ifd_value_offset[2], ifd_value_offset[3]};
				std::cout<<std::dec<<*((int*) value) <<">"<<std::endl;
			
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
TiffStat::tag_intepret(short code){
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
TiffStat::type_intepret(short code){
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
TiffStat::type_length_intepret(short code){
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
TiffStat::type_output_intepret(short code, unsigned char *data_array, int n, bool should_reverse){

	switch(code){
		case 1:
			for (int i = 0;  i < n; i++){
				std::cout<<"0x"<<std::hex<<(int)data_array[i]<<" ";
			}
			std::cout<<">"<<std::endl;
			return;
		case 2:
			for (int i = 0;  i < n; i++){
				std::cout<<data_array[i];
			}
			std::cout<<">"<<std::endl;
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
				
				std::cout<<std::dec<< short_num<<" ";
			}

			std::cout<<">"<<std::endl;
			return;


		case 4:

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
				
				std::cout<<std::dec<< long_num<<" ";
			}

			std::cout<<">"<<std::endl;
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
			unsigned int numerator;
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


				std::cout<<std::dec<< numerator/denominator<<" ";
			}

			std::cout<<">"<<std::endl;
			return;		
			
		default:
			std::cout<<"error in type"<<std::endl;

	}	




}
