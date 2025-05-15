
#include <stddef.h>
#include "mandelbrot/mandelbrot.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define __DOUBLE__

#ifndef __DOUBLE__
using real_number_t = float;
#else
using real_number_t = double;
#endif

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static const size_t        MaxIteration    = 64;
static const real_number_t MaxRadiusSquare = 4.0;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct WindofOffset
{
    real_number_t scale;

    real_number_t high_offset;
    real_number_t width_offset;
};


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct WindowParametrs
{
    WindowSize   window_size;
    WindofOffset window_offset;
};


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct ComplexNumber
{
    real_number_t real_part;
    real_number_t imaginary_part;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct PixelCoordinate
{
    size_t high_coordinate;
    size_t width_coordinate;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct WindowMovementInfo
{
    real_number_t zoom_speed;
    real_number_t move_speed;
};


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct RGBA
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void               UpdateWindowOffset               (WindofOffset* window_offset, const WindowMovementInfo* const window_movement);
void               PaintPixel                       (sf::VertexArray& pixel, size_t pixel_index, const WindowParametrs* const window_parametrs);
void               MakePixelsArray                  (sf::VertexArray& pixels, const WindowParametrs* const window_parametrs, size_t pixels_quant);
void               CloseWindowIfNeed                (sf::RenderWindow* window, sf::Event* event);
void               DrawPixelsArrayOnWindow          (sf::RenderWindow* window, sf::VertexArray& pixels);
void               UpadteSFMLWindow                 (sf::RenderWindow* window, sf::View* view, sf::VertexArray& pixels);
void               SFMLDtor                         (sf::RenderWindow* window, sf::VertexArray& pixels);
RGBA               GetPixelColor                    (const WindowParametrs* const window_parametrs, const PixelCoordinate* const pixel_coordinate);
ComplexNumber      ComplexNumberCtor                (const WindowParametrs* const window_parametrs, const PixelCoordinate* const pixel_coordinate);
PixelCoordinate    GetPixelCoordinate               (const WindowParametrs* const window_parametrs, size_t pixel_index);
ComplexNumber      GetNextMandelbrotSequenceNumber  (const ComplexNumber*   const number, const ComplexNumber* const first_sequence_number);
size_t             GetPixelsQuant                   (const WindowSize* const window_size);
WindowParametrs    WindowParametrsCtor              (const WindowSize* const window_size);
real_number_t      GetScale                         (const WindowSize* const window_size);
WindowMovementInfo WindowMovementCtor               ();
real_number_t      GetAbsoluteValueOfComplexNumber  (const ComplexNumber*   const number);
RGBA               GetRgbaForBadPixel               (size_t bad_iteration);
RGBA               GetRgbaForGoodPixel              ();

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
