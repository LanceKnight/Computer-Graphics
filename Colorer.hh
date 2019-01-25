#ifndef COLORER_HH
#define COLORER_HH

#include <string>
#include <vector>



class Colorer{

public:
	Colorer();

	~Colorer();

	std::string static color(std::vector<std::string> paramList);

};


#endif
