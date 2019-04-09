#include <iostream>
#include "Ray.hh"
#include "Util.hh"


Ray::Ray(){

}

Ray::Ray(float dir_x, float dir_y, float  dir_z,float  ori_x,float  ori_y,float  ori_z){
	this->dir_x_ = dir_x;
	this->dir_y_ = dir_y;
	this->dir_z_ = dir_z;
	this->ori_x_ = ori_x;
	this->ori_y_ = ori_y;
	this->ori_z_ = ori_z;

}

Ray::~Ray(){

}
