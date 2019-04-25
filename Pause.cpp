#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include "3D.h"
#include "Pause.hh"
#include "Util.hh"


Pause::Pause(){}

Pause::~Pause(){}

std::string
Pause::pause(std::vector<std::string> paramList){
	if(paramList.size()<1){

		usleep( 1000000);

		return "Pause Done";
	}
	else{
		return "too many parameters";

	}

}



