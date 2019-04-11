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
	static int nx_;
	static int ny_;
	

};


#endif
