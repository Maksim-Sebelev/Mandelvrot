#ifndef __MANDELBROT_HPP__
#define __MANDELBROT_HPP__


#include <stddef.h>


struct WindowSize
{
    size_t width;
    size_t high;
};


void NativeMandelbrot(const WindowSize* const window_size);


#endif // __MANDELBROT_HPP__

