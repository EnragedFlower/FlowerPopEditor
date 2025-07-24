//https://solarianprogrammer.com/2018/11/19/cpp-reading-writing-bmp-images/

#pragma once
#include<vector>
#include<string>
#include"rgbcolor.h"

#pragma pack(push, 1)

struct BMPFileHeader {
	uint16_t file_type{ 0x4D42 };          // File type always BM which is 0x4D42
	uint32_t file_size{ 0 };               // Size of the file (in bytes)
	uint16_t reserved1{ 0 };               // Reserved, always 0
	uint16_t reserved2{ 0 };               // Reserved, always 0
	uint32_t offset_data{ 0 };             // Start position of pixel data (bytes from the beginning of the file)
};

struct BMPInfoHeader {
	unsigned int  size{ 0 };                      // Size of this header (in bytes)
	int width{ 0 };                      // width of bitmap in pixels
	int height{ 0 };                     // width of bitmap in pixels
	                                         //       (if positive, bottom-up, with origin in lower left corner)
	                                         //       (if negative, top-down, with origin in upper left corner)
	unsigned short int planes{ 1 };                    // No. of planes for the target device, this is always 1
	unsigned short int bit_count{ 0 };                 // No. of bits per pixel
	unsigned int compression{ 0 };               // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
	unsigned int size_image{ 0 };                // 0 - for uncompressed images
	int x_pixels_per_meter{ 0 };
	int y_pixels_per_meter{ 0 };
	unsigned int colors_used{ 0 };               // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
	unsigned int colors_important{ 0 };          // No. of colors used for displaying the bitmap. If 0 all colors are required
	
};

struct BMPColorHeader {
	unsigned int red_mask{   0x00ff0000 };         // Bit mask for the red channel
	unsigned int green_mask{ 0x0000ff00 };       // Bit mask for the green channel
	unsigned int blue_mask{  0x000000ff };        // Bit mask for the blue channel
	unsigned int alpha_mask{ 0xff000000 };       // Bit mask for the alpha channel
	unsigned int color_space_type{ 0x73524742 }; // Default "sRGB" (0x73524742)
	unsigned int unused[16]{ 0 };                // Unused data for sRGB color space
};

#pragma pack(pop)

struct BMP {
	BMPFileHeader file_header;
	BMPInfoHeader bmp_info_header;
	BMPColorHeader bmp_color_header;
	std::vector<unsigned char> data;
	
	inline BMP(std::string filepath) {
		read(filepath);
	}

	void read(std::string fname);

	inline rgbcolor pixel24(const int& x, const int& y) {
		int i = (bmp_info_header.width * (bmp_info_header.height - y - 1) + x) * 3;
		rgbcolor pixelcolor{ data[i + 2], data[i + 1], data[i] };
		return pixelcolor;
	}

	inline uint8_t pixel8(const int& x, const int& y) {
		int i = (bmp_info_header.width * (bmp_info_header.height - y - 1) + x);
		uint8_t pixelcolor{ data[i] };
		return pixelcolor;
	}

	private:
	// Check if the pixel data is stored as BGRA and if the color space type is sRGB
	void check_color_header(BMPColorHeader& bmp_color_header);
	
	unsigned int row_stride{ 0 };

	// Add 1 to the row_stride until it is divisible with align_stride
	inline unsigned int make_stride_aligned(unsigned int align_stride) {
		unsigned int new_stride = row_stride;
		while (new_stride % align_stride != 0) {
			new_stride++;
		}
		return new_stride;
	}
};