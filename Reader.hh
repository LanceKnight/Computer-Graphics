#ifndef READER_HH
#define READER_HH

#include <string>
#include <vector>



class Reader{

public:
	Reader();

	~Reader();

	std::string static read(std::vector<std::string> paramList);

};


#endif
