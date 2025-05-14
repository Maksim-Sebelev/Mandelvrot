#ifndef __MANDELBROT_HPP__
#define __MANDELBROT_HPP__


#include <stddef.h>


struct WindowBorder
{
    long long width_begin;
    long long width_end;

    long long high_begin;
    long long high_end;
};


void NativeMandelbrot(WindowBorder* window_size);


#endif // __MANDELBROT_HPP__

