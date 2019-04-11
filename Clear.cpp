#include <iostream>

#include "Background.hh"
#include "Clear.hh"
#include "Ilight.hh"
#include "Screen.hh"
#include "Sphere.hh"
#include "Trace.hh"
#include "Triangle.hh"

#include "main.hh"
#include "Util.hh"


std::string
Clear::clear(std::vector<std::string> paramList){


	if(paramList.size() ==0){

		for(int i = 0; i < Screen::nx_; i++){
			for(int j = 0; j < Screen::ny_; j++){
				checkImage[i][j][0]= 0;
				checkImage[i][j][1]= 0;
				checkImage[i][j][2]= 0;
			}
		}
		display();
		Background::r_ =0;
		Background::g_ =0;
		Background::b_ =0;
		for(std::vector<Shape*>::iterator it = Trace::object_list_.begin();it != Trace::object_list_.end();++it){
			delete *it;
		}
		for(std::vector<Ilight *>::iterator light_it = Trace::light_list_.begin();light_it != Trace::light_list_.end();++light_it){
			delete *light_it;
		}


		Trace::object_list_.clear();
		Trace::light_list_.clear();

		Triangle::counter_=0;
		Sphere::counter_=0;
		Ilight::counter_=0;
		return "Clear Done";
	}
	else{
		return  "too many parameters";
	}

}


