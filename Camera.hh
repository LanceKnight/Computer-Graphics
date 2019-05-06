#ifndef CAMERA_HH
#define CAMERA_HH

#include <string>
#include <vector>



class Camera{

public:
	Camera();

	~Camera();

	std::string static camera(std::vector<std::string> paramList);


	//data
	static bool perspect_;
	static float ex_;
	static float ey_;
	static float ez_;
	static float gx_;
	static float gy_;
	static float gz_;
	static float ux_;
	static float uy_;
	static float uz_;
	static float s_;
	static float au_;
	static float av_;
	static float bu_;
	static float bv_;
	
};


#endif
