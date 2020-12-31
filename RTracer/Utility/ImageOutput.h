#ifndef IMAGEOUTPUT_H
#define IMAGEOUTPUT_H

#include <iostream>
#include <sstream>
#include "Vector3D.h"


class ImageOut
{
public:
	void write_color( Color pixel_color, int samples_per_pixel);
	std::string get_buffer_string()const { return m_ppm_buffer.str(); }
private:
	std::stringstream m_ppm_buffer = {};
};

#endif