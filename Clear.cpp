#include <iostream>

#include "Background.hh"
#include "Clear.hh"
#include "Screen.hh"
#include "Trace.hh"

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
		Trace::object_list_.clear();

		return "Clear Done";
	}
	else{
		return  "too many parameters";
	}

}


