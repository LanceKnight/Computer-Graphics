#ifndef TIFFWRITE_HH
#define TIFFWRITE_HH

#include <fstream>
#include <string>
#include <vector>



class TiffWrite{

public:
	TiffWrite();

	~TiffWrite();

	std::string static tiff_write(std::vector<std::string> paramList);


private:

	static unsigned char* IFD_encode(short tag_no, short type, int count, int value_offset);
	int write_on_address(std::ofstream file, int address, unsigned char* content, int n);


	static int image_length_;
	static int image_width_;
	static int rows_per_strip_;
	static std::vector<int> strip_offsets_;
	static std::vector<int> strip_byte_counts_;
	static int strips_per_image_;
	static std::vector<int> bits_per_sample_;
	static int photo_metric_;
	static std::vector<int>r_color_map_;
	static std::vector<int>g_color_map_;
	static std::vector<int>b_color_map_;
};



#endif
