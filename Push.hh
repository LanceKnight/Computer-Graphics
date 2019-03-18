#ifndef PUSH_HH
#define PUSH_HH

#include <string>
#include <vector>



class Push{

public:
	Push();

	~Push();

	std::string static push(std::vector<std::string> paramList);


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
