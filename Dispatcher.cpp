#include <cstring>
#include <string>
#include <vector>
#include "Dispatcher.hh"
#include "Mover.hh"
#include "Drawer.hh"
#include "Colorer.hh"
#include "Reader.hh"
#include "TiffRead.hh"
#include "TiffStat.hh"
#include "TiffWrite.hh"
#include "Resize.hh"
#include "Zoom.hh"

//static int nested_read_ = 0;//current nested read


int Dispatcher::nested_read_=0;

Dispatcher::Dispatcher(){}

Dispatcher::~Dispatcher(){}

void
Dispatcher::dispatch(char line[]){

	char * pch = strtok(line, " 	,");
	std::vector<std::string> paramList;
	pch = strtok(NULL, " 	,");
	while(pch!=NULL){
		std::string param(pch);
		paramList.push_back( param ); 
		pch = strtok(NULL, " 	,");
	}	
	
	for(int i(0), sz(strlen(line)); i < sz; i++){
		line[i] = tolower(line[i]);
	}
// move branch
	if(!strcmp(line,"move"))
	{
		#ifdef DEBUG
			std::cout<<"=====DEBUG  INFO====="<<std::endl;
			std::cout<<"dispatching to MOVE branch"<<std::endl;
			std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
		#endif
		
		strcpy(line, Mover::move(paramList).c_str());
	}

//draw branch
	if(!strcmp(line,"draw"))
	{
		#ifdef DEBUG
			std::cout<<"=====DEBUG  INFO====="<<std::endl;
			std::cout<<"dispatching to DRAW branch"<<std::endl;
			std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
		#endif
		
		strcpy(line, Drawer::draw(paramList).c_str());
	}

//color branch
	if(!strcmp(line,"color"))
	{
		#ifdef DEBUG
			std::cout<<"=====DEBUG  INFO====="<<std::endl;
			std::cout<<"dispatching to COLOR branch"<<std::endl;
			std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
		#endif
	
		strcpy(line, Colorer::color(paramList).c_str());
	}
//read branch
	if(!strcmp(line,"read"))
	{
		nested_read_++;
		#ifdef DEBUG
			std::cout<<"=====DEBUG  INFO====="<<std::endl;
			std::cout<<"dispatching to READ branch"<<std::endl;
			std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
		#endif
	
		#ifdef DEBUG
			std::cout<<"=====DEBUG  INFO====="<<std::endl;
			std::cout<< "nested_read:"<< nested_read_ <<std::endl;
			std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
		#endif
		if(nested_read_ < (get_max_read_recursion_depth()+1)){
			
			strcpy(line, Reader::read(paramList).c_str());
			
		}
		else{
			std::cout<< "WARNING:maxium number of nested read reached. Read command abandoned"<<std::endl;
		}
		nested_read_--;
	}

//tiffstat branch
	if(!strcmp(line, "tiffstat"))
	{

		#ifdef DEBUG
			std::cout<<"=====DEBUG  INFO====="<<std::endl;
			std::cout<<"dispatching to TiffStat branch"<<std::endl;
			std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
		#endif
		strcpy(line, TiffStat::tiff_stat(paramList).c_str());


	}


//tiffread branch
	if(!strcmp(line, "tiffread"))
	{

		#ifdef DEBUG
			std::cout<<"=====DEBUG  INFO====="<<std::endl;
			std::cout<<"dispatching to TiffRead branch"<<std::endl;
			std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
		#endif
		strcpy(line, TiffRead::tiff_read(paramList).c_str());


	}

//tiffwrite branch
	if(!strcmp(line, "tiffwrite"))
	{

		#ifdef DEBUG
			std::cout<<"=====DEBUG  INFO====="<<std::endl;
			std::cout<<"dispatching to TiffWrite branch"<<std::endl;
			std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
		#endif
		strcpy(line, TiffWrite::tiff_write(paramList).c_str());


	}

//resize branch
	if(!strcmp(line, "resize"))
	{

		#ifdef DEBUG
			std::cout<<"=====DEBUG  INFO====="<<std::endl;
			std::cout<<"dispatching to Resize branch"<<std::endl;
			std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
		#endif
		strcpy(line, Resize::resize(paramList).c_str());


	}

//zoom branch
	if(!strcmp(line, "zoom"))
	{

		#ifdef DEBUG
			std::cout<<"=====DEBUG  INFO====="<<std::endl;
			std::cout<<"dispatching to Zoom branch"<<std::endl;
			std::cout<<"==END OF DEBUG INFO==\n"<<std::endl;
		#endif
		strcpy(line, Zoom::zoom(paramList).c_str());


	}
//other processing
   if (line == NULL)
   {
      printf("Exiting...\n");
      exit(0);
   }
   else
		//std::cout<<"RESULT:" << line <<std::endl;
      printf("RESULT: %s\n", line);
     
   printf("CLI> ");
   fflush(stdout);

}
