#include "ImageOutput.h"
#include "VMath.h"

void ImageOut::write_color( Color pixel_color, int samples_per_pixel)
{
    double red = pixel_color.x();
    double green = pixel_color.y();
    double blu = pixel_color.z();

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    double scale = 1.0 / samples_per_pixel;
    red = sqrt(scale * red);
    green = sqrt(scale * green);
    blu = sqrt(scale * blu);

    // Write the translated [0,255] value of each color component.
    int f_red = static_cast<int>(256 * clamp(red, 0.0, 0.999));
    int f_green = static_cast<int>(256 * clamp(green, 0.0, 0.999));
    int f_blue = static_cast<int>(256 * clamp(blu, 0.0, 0.999));
    m_ppm_buffer << f_red << ' ' << f_green << ' ' << f_blue << '\n';
}