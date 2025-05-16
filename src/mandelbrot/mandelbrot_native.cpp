#include <stddef.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include "SFML/Graphics.hpp"
#include "mandelbrot/mandelbrot.hpp"
#include "mandelbrot/mandelbrot_global_include.hpp"
#include "lib/lib.hpp"


#ifdef _DEBUG
#include "log/log.hpp"
#endif // _DEBUG

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

RGBA GetPixelColorNative(const WindowParametrs* const window_parametrs, const PixelCoordinate* const pixel_coordinate)
{
    assert(window_parametrs);
    assert(pixel_coordinate);

    if (IsPixelCenter(pixel_coordinate, &window_parametrs->window_size))
        return GetRgbaForCenterPixel();

    const ComplexNumber first_sequence_number = ComplexNumberCtor(window_parametrs, pixel_coordinate);
          ComplexNumber number                = first_sequence_number;

    for (size_t i = 0; i < MaxIteration; i++)
    {
        if (GetAbsoluteValueOfComplexNumber(&number) > MaxRadiusSquare)
            return GetRgbaForBadPixel(i);

        number = GetNextMandelbrotSequenceNumber(&number, &first_sequence_number);
    }

    return GetRgbaForGoodPixel();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static const size_t N = 4;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

RGBA GetNPixelColor(const WindowParametrs* const window_parametrs, const PixelCoordinate* const pixel_coordinate)
{
    assert(window_parametrs);
    assert(pixel_coordinate);

    

    return GetRgbaForGoodPixel();    
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

