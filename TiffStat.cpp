#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "Dispatcher.hh"
#include "TiffStat.hh"



TiffStat::TiffStat(){}

TiffStat::~TiffStat(){}



std::string
TiffStat::tiff_stat(std::vector<std::string> paramList){
	std::string result;
	char buffer[3];
	short magicno;
	int ifdaddress;
	short ifdcount;

	if(paramList.size()<=1){
		if(paramList.size()==1){
			std::ifstream file;
			std::string filename = *paramList.begin();
			std::string command; //the command tiffstat from lines of the file.
			file.open(filename, std::ios::binary);
		  	if (!file) {
				result = "Unable to open "+ filename;
				
			}
			file.seekg(0,std::ios::beg);
			
			file.read(buffer,2);

			file.read((char*)&magicno,2);

			file.read((char*)&ifdaddress,4);

			file.seekg(ifdaddress,std::ios::beg);
			
			file.read((char*)&ifdcount,2);


			#ifdef DEBUG
				std::cout<<"=====DEBUG  INFO====="<<std::endl;
				std::cout<< "buffer:"<<buffer <<std::endl;
				std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
			#endif



			file.close();
			result = "Read stat of file <"+filename+"> successfully";
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


