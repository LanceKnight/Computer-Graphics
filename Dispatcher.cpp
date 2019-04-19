#include <cstring>
#include <string>
#include <vector>
#include "Dispatcher.hh"
//assignment1
#include "Mover.hh"
#include "Drawer.hh"
#include "Colorer.hh"
//assignment2
#include "Reader.hh"
#include "TiffRead.hh"
#include "TiffStat.hh"
#include "TiffWrite.hh"
//assignment3
#include "Resize.hh"
#include "Zoom.hh"
#include "Border.hh"
#include "Select.hh"
//assignement4
#include "Push.hh"
#include "Pop.hh"
#include "Translate.hh"
#include "Scale.hh"
#include "Rotate.hh"
#include "Ortho.hh"
#include "Perspective.hh"
#include "Lookat.hh"
#include "Vertex.hh"
#include "Reset.hh"
#include "Pause.hh"
#include "Orient.hh"
//assignement5
#include "Screen.hh"
#include "Orthocamera.hh"
#include "Camera.hh"
#include "Background.hh"
#include "Sphere.hh"
#include "Triangle.hh"
#include "Box.hh"
#include "Clear.hh"
#include "Ilight.hh"
#include "Trace.hh"
#include "Plane.hh"
//other
#include "Util.hh"

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

//assignement2

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

//Assignment4

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
			std::cout<<"dispatching to Pop branch"<<std::endl;
		Util::debug_tail();
		strcpy(line, Pop::pop(paramList).c_str());
	}

//translate branch
	if(!strcmp(line, "translate"))
	{
		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to Translate branch"<<std::endl;
		Util::debug_tail();
		strcpy(line, Translate::translate(paramList).c_str());
	}

//scale branch
	if(!strcmp(line, "scale"))
	{
		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to Scale branch"<<std::endl;
		Util::debug_tail();
		strcpy(line, Scale::scale(paramList).c_str());
	}

//rotate branch
	if(!strcmp(line, "rotate"))
	{
		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to Rotate branch"<<std::endl;
		Util::debug_tail();
		strcpy(line, Rotate::rotate(paramList).c_str());
	}

//Ortho branch
		if(!strcmp(line, "ortho"))
		{
			Util::debug_head("Dispatcher.cpp");
				std::cout<<"dispatching to Ortho branch"<<std::endl;
			Util::debug_tail();
			strcpy(line, Ortho::ortho(paramList).c_str());
		}

//Perspective branch
		if(!strcmp(line, "perspective"))
		{
			Util::debug_head("Dispatcher.cpp");
				std::cout<<"dispatching to Perspective branch"<<std::endl;
			Util::debug_tail();
			strcpy(line, Perspective::perspective(paramList).c_str());
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

			std::cout<<"dispatching to Vertex branch"<<std::endl;
		Util::debug_tail();
		strcpy(line, Vertex::vertex(paramList).c_str());
	}

//reset branch
	if(!strcmp(line, "reset"))
	{
		Util::debug_head("Dispatcher.cpp");

			std::cout<<"dispatching to Reset branch"<<std::endl;
		Util::debug_tail();
		strcpy(line, Reset::reset(paramList).c_str());
	}

//pause branch
	if(!strcmp(line, "pause"))
	{
		Util::debug_head("Dispatcher.cpp");

			std::cout<<"dispatching to pause branch"<<std::endl;
		Util::debug_tail();
		strcpy(line, Pause::pause(paramList).c_str());
	}

//orient branch
	if(!strcmp(line, "orient"))
	{
		Util::debug_head("Dispatcher.cpp");

			std::cout<<"dispatching to orient branch"<<std::endl;
		Util::debug_tail();
		strcpy(line, Orient::orient(paramList).c_str());
	}

//Assignment5

//screen branch
	if(!strcmp(line, "screen"))
	{
		Util::debug_head("Dispatcher.cpp");

			std::cout<<"dispatching to screen branch"<<std::endl;
		Util::debug_tail();
		strcpy(line, Screen::screen(paramList).c_str());
	}

//Othocamera branch
	if(!strcmp(line, "orthocamera"))
	{

		Util::debug_head("Dispatcher.cpp");

			std::cout<<"dispatching to orthocamera branch"<<std::endl;

		Util::debug_tail();
		strcpy(line, Orthocamera::orthocamera(paramList).c_str());


	}

//camera branch
	if(!strcmp(line, "camera"))
	{
		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to camera branch"<<std::endl;
		Util::debug_tail();
		strcpy(line, Camera::camera(paramList).c_str());
	}

//background branch
	if(!strcmp(line, "background"))
	{
		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to background branch"<<std::endl;
		Util::debug_tail();
		strcpy(line, Background::background(paramList).c_str());
	}

//sphere branch
	if(!strcmp(line, "sphere"))
	{
		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to sphere branch"<<std::endl;
		Util::debug_tail();
		strcpy(line, Sphere::sphere(paramList).c_str());
	}

//Triangle branch
	if(!strcmp(line, "triangle"))
	{
		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to triangle branch"<<std::endl;
		Util::debug_tail();
		strcpy(line, Triangle::triangle(paramList).c_str());
	}

//Box branch
	if(!strcmp(line, "box"))
	{
		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to box branch"<<std::endl;
		Util::debug_tail();
		strcpy(line, Box::box(paramList).c_str());
	}

//Illight branch
	if(!strcmp(line, "ilight"))
	{
		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to Ilight branch"<<std::endl;
		Util::debug_tail();
		strcpy(line, Ilight::ilight(paramList).c_str());
	}

//clear branch
	if(!strcmp(line, "clear"))
	{
		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to clear branch"<<std::endl;
		Util::debug_tail();
		strcpy(line, Clear::clear(paramList).c_str());
	}

//trace branch
	if(!strcmp(line, "trace"))
	{
		Util::debug_head("Dispatcher.cpp");
			std::cout<<"dispatching to trace branch"<<std::endl;
		Util::debug_tail();
		strcpy(line, Trace::trace(paramList).c_str());
	}

	//plane branch
	if(!strcmp(line, "plane"))
	{
		Util::debug_head("Dispatcher.cpp");
		std::cout<<"dispatching to plane branch"<<std::endl;
		Util::debug_tail();
		strcpy(line, Plane::plane(paramList).c_str());
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
