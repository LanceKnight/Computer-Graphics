#ifndef SHAPE_HH
#define SHAPE_HH

#include <string>
#include <vector>



class Shape{

public:
	Shape();

	~Shape();

	virtual std::vector<float> get_geo_params();

	virtual std::vector<float> get_color_params();

	virtual std::string get_type();

	virtual void set_geo_params(std::vector<float> param_list);

	virtual void set_color_params(std::vector<float> param_list);
	//data

	std::string type_;
	std::string name_;
	std::vector<float> geo_params_;
	std::vector<float> color_params_;

};


#endif
