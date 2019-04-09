#ifndef RAY_HH
#define RAY_HH

#include <string>
#include <vector>



class Ray{

public:
	Ray();

	Ray(float dir_x, float dir_y, float  dir_z,float  ori_x,float  ori_y,float  ori_z);

	~Ray();


	//data
	float dir_x_=0;
	float dir_y_=0;
	float dir_z_=0;
	float ori_x_=0;
	float ori_y_=0;
	float ori_z_=0;
	

};


#endif
