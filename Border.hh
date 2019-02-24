#ifndef BORDER_HH
#define BORDER_HH

#include <string>
#include <vector>



class Border{

public:
	Border();

	~Border();

	std::string static border(std::vector<std::string> paramList);


	enum BorderType{
		zero,
		freeze,
		circular
	};

	static BorderType border_;

};


#endif
