#ifndef RESIZE_HH
#define RESIZE_HH

#include <string>
#include <vector>



class Resize{

public:
	Resize();

	~Resize();

	std::string static resize(std::vector<std::string> paramList);

	float static sinc(float x);
	
	float static h(float x);	

};


#endif
