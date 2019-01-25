#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "Dispatcher.hh"
#include "Reader.hh"



Reader::Reader(){}

Reader::~Reader(){}



std::string
Reader::read(std::vector<std::string> paramList){
	std::string result;
   static char line[1024];
	if(paramList.size()<=1){
		if(paramList.size()==1){
			std::ifstream file;
			std::string filename = *paramList.begin();
			std::string command; //the command read from lines of the file.
			file.open(filename);
		  	if (!file) {
				result = "Unable to open "+ filename;
				
			}
			else{
				while (std::getline(file, command)) {
					if(command.empty()!= true){
						if(command.at(0)!='#'){
						
						std::cout<<"executing a command from the file..."<<std::endl;
						std::cout<<"command:"<<command<<std::endl;
						strcpy(line, command.c_str());
						strtok(line, "#");
						#ifdef DEBUG
							std::cout<<"=====DEBUG  INFO====="<< std::endl;
								std::cout<<"line:"<<line<<std::endl;

							std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
						#endif
						
						Dispatcher::dispatch(line);

						result = "File <"+ filename+"> was processed succefully";
						}	
					}
				}

				file.close();

				#ifdef DEBUG
					std::cout<<"=====DEBUG  INFO====="<< std::endl;
						std::cout<<"File closed, nestRead:"<<Dispatcher::nested_read_<<std::endl;

					std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
				#endif
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


