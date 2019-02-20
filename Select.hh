#ifndef SELECT_HH
#define SELECT_HH

#include <string>
#include <vector>



class Select{

public:
	Select();

	~Select();

	std::string static select(std::vector<std::string> paramList);

};


#endif
