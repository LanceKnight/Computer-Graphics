#ifndef TIFFSTAT_HH
#define TIFFSTAT_HH

#include <string>
#include <vector>



class TiffStat{

public:
	TiffStat();

	~TiffStat();

	std::string static tiff_stat(std::vector<std::string> paramList);

};


#endif
