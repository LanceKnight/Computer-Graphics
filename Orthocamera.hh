#ifndef ORTHOCAMERA_HH
#define ORTHOCAMERA_HH

#include <string>
#include <vector>



class Orthocamera{

public:
	Orthocamera();

	~Orthocamera();

	std::string static orthocamera(std::vector<std::string> paramList);


	//data
	static bool ortho_;

	

};


#endif
