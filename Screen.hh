#ifndef SCREEN_HH
#define SCREEN_HH

#include <string>
#include <vector>



class Screen{

public:
	Screen();

	~Screen();

	std::string static screen(std::vector<std::string> paramList);


	//data
	static float nx_;
	static float ny_;
	

};


#endif
