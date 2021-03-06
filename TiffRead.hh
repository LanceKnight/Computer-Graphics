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


	static int image_length_;
	static int image_width_;
	static int rows_per_strip_;
	static std::vector<int> strip_offsets_;
	static std::vector<int> strip_byte_counts_;
	static int strips_per_image_;
	static std::vector<int> bits_per_sample_;
	static int compression_;
	static int photo_metric_;
	static int x_resolution_numerator_;
	static int x_resolution_denominator_; 
	static int y_resolution_numerator_;
	static int y_resolution_denominator_;
	static short resolution_unit_;	

	static std::vector<int>r_color_map_;
	static std::vector<int>g_color_map_;
	static std::vector<int>b_color_map_;

	static bool is_gray_image_;
	static bool has_image_;
private:

	static void IFD_intepret(unsigned char* IFD, bool should_reverse, std::ifstream& file);
	static std::string tag_intepret(short code);
	static std::string type_intepret(short code);
	static int type_length_intepret(short code);
	static void type_output_intepret(short tag, short code, unsigned char *data_array, int n, bool should_reverse);
	static void display_image(std::ifstream& file, bool should_reverse);

};



#endif
