#ifndef ILIGHT_HH
#define ILIGHT_HH

#include <string>
#include <vector>



class Ilight{

public:
	Ilight();

	Ilight(float lr, float lg, float lb,
		   float dx, float dy, float dz);

	~Ilight();

	std::string static ilight(std::vector<std::string> paramList);


	//data
	float lr_, lg_, lb_, dx_, dy_, dz_;
	std::string name_;
	static int counter_;
	

};


#endif
