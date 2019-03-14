#ifndef SELECT_HH
#define SELECT_HH

#include <string>
#include <vector>



class Select{

public:
	Select();

	~Select();

	std::string static select(std::vector<std::string> paramList);


	//data
	enum FilterType{
		gaussian,
		mitchell,
		lanczos,
		triangle,
		box,
		hemming,
		b_spline,
		catmull_rom
	};

	static FilterType filter_;

	static float alpha_;
	

};


#endif
