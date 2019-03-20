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
#include "Border.hh"
#include "Select.hh"
#include "Push.hh"
#include "Translate.hh"
#include "Ortho.hh"
#include "Lookat.hh"
#include "Vertex.hh"
#include "Reset.hh"
#include "Util.hh"

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
		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to MOVE branch"<<std::endl;
		Util::debug_tail();
		
		strcpy(line, Mover::move(paramList).c_str());
	}

//draw branch
	if(!strcmp(line,"draw"))
	{
		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to DRAW branch"<<std::endl;
		Util::debug_tail();
		
		strcpy(line, Drawer::draw(paramList).c_str());
	}

//color branch
	if(!strcmp(line,"color"))
	{
		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to COLOR branch"<<std::endl;

		Util::debug_tail();
	
		strcpy(line, Colorer::color(paramList).c_str());
	}
//read branch
	if(!strcmp(line,"read"))
	{
		nested_read_++;
		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to READ branch"<<std::endl;
		Util::debug_tail();
	
		Util::debug_head("Dispatcher.cpp");
			std::cout<< "nested_read:"<< nested_read_ <<std::endl;

		Util::debug_tail();
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

		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to TiffStat branch"<<std::endl;

		Util::debug_tail();
		strcpy(line, TiffStat::tiff_stat(paramList).c_str());


	}


//tiffread branch
	if(!strcmp(line, "tiffread"))
	{

		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to TiffRead branch"<<std::endl;

		Util::debug_tail();
		strcpy(line, TiffRead::tiff_read(paramList).c_str());


	}

//tiffwrite branch
	if(!strcmp(line, "tiffwrite"))
	{

		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to TiffWrite branch"<<std::endl;

		Util::debug_tail();
		strcpy(line, TiffWrite::tiff_write(paramList).c_str());


	}

//resize branch
	if(!strcmp(line, "resize"))
	{

		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to Resize branch"<<std::endl;

		Util::debug_tail();
		strcpy(line, Resize::resize(paramList).c_str());


	}

//zoom branch
	if(!strcmp(line, "zoom"))
	{

		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to Zoom branch"<<std::endl;

		Util::debug_tail();
		strcpy(line, Zoom::zoom(paramList).c_str());


	}

//border branch
	if(!strcmp(line, "border"))
	{

		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to Border branch"<<std::endl;

		Util::debug_tail();
		strcpy(line, Border::border(paramList).c_str());


	}

//select branch
	if(!strcmp(line, "select"))
	{

		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to Select branch"<<std::endl;

		Util::debug_tail();
		strcpy(line, Select::select(paramList).c_str());


	}

//push branch
	if(!strcmp(line, "push"))
	{

		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to Push branch"<<std::endl;

		Util::debug_tail();
		strcpy(line, Push::push(paramList).c_str());


	}
//pop branch
	if(!strcmp(line, "pop"))
	{

		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to Push branch"<<std::endl;

		Util::debug_tail();
		strcpy(line, Push::push(paramList).c_str());


	}

//translate branch
	if(!strcmp(line, "translate"))
	{

		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to Translate branch"<<std::endl;

		Util::debug_tail();
		strcpy(line, Translate::translate(paramList).c_str());


	}

//Ortho branch
		if(!strcmp(line, "ortho"))
		{

			Util::debug_head("Dispatcher.cpp");
				std::cout<<"dispatching to Ortho branch"<<std::endl;

			Util::debug_tail();
			strcpy(line, Ortho::ortho(paramList).c_str());


		}

//lookat branch
	if(!strcmp(line, "lookat"))
	{

		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to LookAt branch"<<std::endl;

		Util::debug_tail();
		strcpy(line, Lookat::lookat(paramList).c_str());


	}
//vertex branch
	if(!strcmp(line, "vertex"))
	{

		Util::debug_head("Dispatcher.cpp");
			std::cout<<"=====DEBUG  INFO====="<<std::endl;
			std::cout<<"dispatching to Vertex branch"<<std::endl;

		Util::debug_tail();
		strcpy(line, Vertex::vertex(paramList).c_str());


	}

//reset branch
	if(!strcmp(line, "reset"))
	{

		Util::debug_head("Dispatcher.cpp");
			std::cout<<"=====DEBUG  INFO====="<<std::endl;
			std::cout<<"dispatching to Reset branch"<<std::endl;

		Util::debug_tail();
		strcpy(line, Reset::reset(paramList).c_str());


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
