#ifndef MOVER_HH
#define MOVER_HH

#include <string>
#include <vector>



class Mover{

public:
	Mover();

	~Mover();

	std::string static move(std::vector<std::string> paramList);

};


#endif
