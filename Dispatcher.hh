#ifndef DISPATCHER_HH
#define DISPATCHER_HH


#include <iostream>

class Dispatcher{

public:
	Dispatcher();

	~Dispatcher();

	static void dispatch(char line[]);

   static int &get_max_read_recursion_depth()
   {
     static int s_max( 5);
     return s_max;
   }
private:
	static int nested_read_;
};

#endif
