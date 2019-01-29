#ifndef TIFFSTAT_HH
#define TIFFSTAT_HH

#include <fstream>
#include <string>
#include <vector>



class TiffStat{

public:
	TiffStat();

	~TiffStat();

	std::string static tiff_stat(std::vector<std::string> paramList);


private:

	void static IFD_intepret(unsigned char* IFD, bool should_reverse, std::ifstream& file);
	std::string static tag_intepret(short code);
	std::string static type_intepret(short code);
	int static type_length_intepret(short code);
	void static type_output_intepret(short code, unsigned char *data_array, int n);

};



#endif
