#ifndef RESIZE_HH
#define RESIZE_HH

#include <string>
#include <vector>



class Resize{

public:
	Resize();

	~Resize();

	std::string static resize(std::vector<std::string> paramList);

	

private:
		
 	static float kernel(float x);	
	
	static float sinc(float x);

	static int get_lower_limit(int m, float scale_y, float M_y);

	static int get_upper_limit(int m, float scale_y, float M_y);
};


#endif
