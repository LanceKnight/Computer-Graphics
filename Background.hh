#ifndef BACKGROUND_HH
#define BACKGROUND_HH

#include <string>
#include <vector>



class Background{

public:
	Background();

	~Background();

	std::string static background(std::vector<std::string> paramList);


	//data
	static float r_;
	static float g_;
	static float b_;

};


#endif
