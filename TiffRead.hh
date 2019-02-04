#ifndef TIFFREAD_HH
#define TIFFREAD_HH

#include <fstream>
#include <string>
#include <vector>



class TiffRead{

public:
	TiffRead();

	~TiffRead();

	std::string static tiff_read(std::vector<std::string> paramList);


private:

	void static IFD_intepret(unsigned char* IFD, bool should_reverse, std::ifstream& file);
	std::string static tag_intepret(short code);
	std::string static type_intepret(short code);
	int static type_length_intepret(short code);
	void static type_output_intepret(short code, unsigned char *data_array, int n, bool should_reverse);

};



#endif
