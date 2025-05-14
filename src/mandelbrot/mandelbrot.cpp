#include <stddef.h>
#include <assert.h>
#include <limits.h>
#include "SFML/Graphics.hpp"
#include "mandelbrot/mandelbrot.hpp"
#include "lib/lib.hpp"


#ifdef _DEBUG
#include "log/log.hpp"
#endif // _DEBUG

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static const size_t MaxIteration = 64;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define __DOUBLE__

#ifndef __DOUBLE__
using real_number_t = float;
#else
using real_number_t = double;
#endif

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static const real_number_t MaxRadiusSquare = 4.0;
static real_number_t Scale           = 400.0;
static real_number_t HighOffset      = 0.0;
static real_number_t WidthOffset     = 0.0;


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct ComplexNumber
{
    real_number_t real_part;
    real_number_t imaginary_part;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct PixelCoordinate
{
    long long high_coordinate;
    long long width_coordinate;
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
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void            PaintPixel                       (sf::VertexArray& pixel, size_t pixel_index, const WindowBorder* const window_border);
static RGBA            GetPixelColor                    (const WindowBorder*    const window_border, const PixelCoordinate* const pixel_coordinate);
static ComplexNumber   ComplexNumberCtor                (const WindowBorder*    const window_border, const PixelCoordinate* const pixel_coordinate);
static ComplexNumber   GetNextMandelbrotSequenceNumber  (const ComplexNumber* const number,const ComplexNumber* const first_sequence_number);
static real_number_t   GetAbsoluteValueOfComplexNumber  (const ComplexNumber* const number);
static PixelCoordinate GetPixelCoordinate               (const WindowBorder*    const window_border, size_t pixel_index);
static RGBA            GetRgbaForBadPixel               (size_t bad_iteration);
static RGBA            GetRgbaForGoodPixel              ();

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void NativeMandelbrot(WindowBorder* window_border)
{
    assert(window_border);

    const size_t window_width = window_border->width_end;
    const size_t window_high  = window_border->high_end;

    const size_t pixels_quant = window_high * window_width;

    sf::VertexArray pixels(sf::PrimitiveType::Points, pixels_quant);    

    ON_DEBUG(
    LOG_COLOR(Yellow);
    )

    for (size_t i = 0; i < pixels_quant; i++)
        PaintPixel(pixels, i, window_border);

    sf::RenderWindow window(sf::VideoMode((unsigned int) window_width, (unsigned int) window_high), "Best policarbonate: SEBELEV GROUPP.");

    window.draw(pixels);
    window.display(); 

    sf::View view = window.getDefaultView();

    while (window.isOpen()) 
    {
        sf::Event event = {};

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            Scale *= 1.2;
    
            // size_t offset_w = ((window_border->width_end - window_border->width_begin) / 2);
            // size_t offset_h = ((window_border->high_end - window_border->high_begin)   / 2);
            
            // window_border->high_begin += offset_h;
            // window_border->high_end   -= offset_h;

            // window_border->width_begin += offset_w;
            // window_border->width_end   -= offset_w;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            Scale /= 1.2;
            // size_t offset_w = ((window_border->width_end - window_border->width_begin) / 2);
            // size_t offset_h = ((window_border->high_end - window_border->high_begin)   / 2);

            // window_border->high_begin -= offset_h;
            // window_border->high_end   += offset_h;

            // window_border->width_begin -= offset_w;
            // window_border->width_end   += offset_w;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            HighOffset -= 10;
            window_border->high_begin -= 10;
            window_border->high_end   -= 10;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            HighOffset += 10;
            window_border->high_begin += 10;
            window_border->high_end   += 10;
        }
    
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            WidthOffset -= 10;
            window_border->width_begin -= 10;
            window_border->width_end   -= 10;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            WidthOffset += 10;
            window_border->width_begin += 10;
            window_border->width_end   += 10;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            window.close();
        }

        
        for (size_t w = window_border->width_begin; w < window_border->width_end; w++)
            for (size_t h = window_border->high_begin; h < window_border->high_end; h++)
                PaintPixel(pixels, w * (window_border->width_end - window_border->width_begin) + h, window_border);

        window.setView(view);

        window.clear();
        window.draw(pixels);
        window.display();
    }
    
    pixels.clear();
    window.clear();    

    return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void PaintPixel(sf::VertexArray& pixel, size_t pixel_index, const WindowBorder* const window_border)
{
    assert(&pixel);
    assert(window_border);

    PixelCoordinate pixel_coordinate = GetPixelCoordinate(window_border, pixel_index);
    RGBA            pixel_color      = GetPixelColor(window_border, &pixel_coordinate);

    pixel[pixel_index].color    = sf::Color(pixel_color.r, pixel_color.g, pixel_color.b, pixel_color.a);
    pixel[pixel_index].position = sf::Vector2f((float) (pixel_coordinate.width_coordinate), (float) pixel_coordinate.high_coordinate);

    return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static PixelCoordinate GetPixelCoordinate(const WindowBorder* const window_border, size_t pixel_index)
{
    assert(window_border);

    PixelCoordinate coordinate = {};

    coordinate.width_coordinate = (pixel_index % window_border->width_end);
    coordinate.high_coordinate  = (pixel_index - coordinate.width_coordinate) / window_border->width_end;

    return coordinate;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static RGBA GetPixelColor(const WindowBorder* const window_border, const PixelCoordinate* const pixel_coordinate)
{
    assert(window_border);
    assert(pixel_coordinate);

    const ComplexNumber first_sequence_number = ComplexNumberCtor(window_border, pixel_coordinate);
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

static RGBA GetRgbaForBadPixel(size_t bad_iteration)
{
    assert(bad_iteration <= MaxIteration);


    unsigned char color = ((float) bad_iteration / (float) MaxIteration) * 255;

    return {color, color, color, 255};
    return {color, color, color, color};
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static RGBA GetRgbaForGoodPixel()
{
    return {0, 0, 0, 0};
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static ComplexNumber ComplexNumberCtor(const WindowBorder* const window_border, const PixelCoordinate* const pixel_coordinate)
{
    ComplexNumber number = 
    {
        .real_part      = (((real_number_t) pixel_coordinate->width_coordinate) - ((real_number_t) (window_border->width_end - window_border->width_begin) / 2  - WidthOffset + 200.0)) / Scale,
        .imaginary_part = (((real_number_t) pixel_coordinate->high_coordinate ) - ((real_number_t) (window_border->high_end  - window_border->high_begin ) / 2  - HighOffset         )) / Scale
    };

    ON_DEBUG(
    size_t index = pixel_coordinate->high_coordinate * window_border->width + pixel_coordinate->width_coordinate;
    if (index < 1000)
        LOG_ADC_PRINT("number = \n{\n\tindex = %lu\n\tre = %lf\n\tim = %lf\n}\n\n", index, number.real_part, number.imaginary_part);
    )
    return number;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static real_number_t GetAbsoluteValueOfComplexNumber(const ComplexNumber* const number)
{
    assert(number);

    real_number_t real_part = number->real_part;
    real_number_t imaginary_part = number->imaginary_part;

    real_number_t absolute_value = (real_part * real_part) + (imaginary_part * imaginary_part);

    return absolute_value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static ComplexNumber GetNextMandelbrotSequenceNumber(const ComplexNumber* const number, const ComplexNumber* const first_sequence_elem)
{
    assert(number);

    real_number_t real_part      = number->real_part;
    real_number_t imaginary_part = number->imaginary_part;

    ComplexNumber next_number    = {};

    next_number.real_part        = (real_part * real_part) - (imaginary_part * imaginary_part) + first_sequence_elem->real_part;
    next_number.imaginary_part   = (2 * real_part * imaginary_part)                            + first_sequence_elem->imaginary_part;

    return next_number;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
