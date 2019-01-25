#ifndef DRAWER_HH
#define DRAWER_HH

#include <string>
#include <vector>



class Drawer{

public:
	Drawer();

	~Drawer();

	std::string static draw(std::vector<std::string> paramList);

};


#endif
