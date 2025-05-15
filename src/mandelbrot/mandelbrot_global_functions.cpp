#include <stddef.h>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <assert.h>

#include "lib/lib.hpp"
#include "mandelbrot/mandelbrot.hpp"
#include "mandelbrot/mandelbrot_global_include.hpp"

#ifdef _DEBUG
#include "log/log.hpp"
#endif // _DEBUG

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void DrawMandeltbrote(const WindowSize* const window_size, RGBA GetPixelColor(const WindowParametrs* const, const PixelCoordinate* const))
{
    assert(window_size);
    assert(GetPixelColor);

    WindowParametrs    window_parametrs = WindowParametrsCtor(window_size);
    WindowMovementInfo window_movement  = WindowMovementCtor();
    size_t             pixels_quant     = GetPixelsQuant     (window_size);

    sf::VertexArray pixels(sf::PrimitiveType::Points, pixels_quant);
    MakePixelsArray(pixels, &window_parametrs, pixels_quant, GetPixelColor);

    sf::RenderWindow window(sf::VideoMode((unsigned int) window_size->width, (unsigned int) window_size->high), "Best policarbonate: SEBELEV GROUPP.");
    DrawPixelsArrayOnWindow(&window, pixels);

    sf::View  view  = window.getDefaultView();    
    sf::Event event = {};

    while (window.isOpen()) 
    {
        CloseWindowIfNeed  (&window,                         &event                                        );
        UpdateWindowOffset (&window_parametrs.window_offset, &window_movement                              );
        MakePixelsArray    (pixels,                          &window_parametrs, pixels_quant, GetPixelColor);
        UpadteSFMLWindow   (&window,                         &view,             pixels                     );
    }

    SFMLDtor(&window, pixels);

    return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

WindowParametrs WindowParametrsCtor(const WindowSize* const window_size)
{
    assert(window_size);

    WindowParametrs window_parametrs            = {};

    window_parametrs.window_size.width          = window_size->width;
    window_parametrs.window_size.high           = window_size->high ;

    window_parametrs.window_offset.high_offset  = 0;
    window_parametrs.window_offset.width_offset = 0;
    window_parametrs.window_offset.scale        = GetScale(window_size);

    return window_parametrs;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

real_number_t GetScale(const WindowSize* const window_size)
{
    assert(window_size);

    const size_t width = window_size->width;
    const size_t high  = window_size->high;

    return (real_number_t) sqrt(width * high) / 3.0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

WindowMovementInfo WindowMovementCtor()
{
    const real_number_t move_speed = 20.0;
    const real_number_t zoom_speed = 1.5;

    return {zoom_speed, move_speed};
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CloseWindowIfNeed(sf::RenderWindow* window, sf::Event* event)
{
    assert(window);
    assert(event);

    while (window->pollEvent(*event))
        if (event->type == sf::Event::Closed)
            window->close();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        window->close();

    return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void SFMLDtor(sf::RenderWindow* window, sf::VertexArray& pixels)
{
    assert(window);
    assert(&pixels);

    pixels.clear();
    window->clear();

    return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UpadteSFMLWindow(sf::RenderWindow* window, sf::View* view, sf::VertexArray& pixels)
{
    assert(&pixels);
    assert(&window);
    assert(&view);

    window->setView(*view);

    window->clear();
    window->draw(pixels);
    window->display();

    return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void DrawPixelsArrayOnWindow(sf::RenderWindow* window, sf::VertexArray& pixels)
{
    assert(window);
    assert(&pixels);

    window->draw(pixels);
    window->display();

    return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UpdateWindowOffset(WindofOffset* window_offset, const WindowMovementInfo* const window_movement)
{
    assert(window_offset);


    const real_number_t move_speed =  window_movement->move_speed;
    const real_number_t zoom_speed =  window_movement->zoom_speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        window_offset->high_offset  *= zoom_speed;
        window_offset->width_offset *= zoom_speed;
        window_offset->scale        *= zoom_speed;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        window_offset->high_offset  /= zoom_speed;
        window_offset->width_offset /= zoom_speed;
        window_offset->scale        /= zoom_speed;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        window_offset->high_offset -= move_speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        window_offset->high_offset += move_speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        window_offset->width_offset -= move_speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        window_offset->width_offset += move_speed;

    return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void MakePixelsArray(sf::VertexArray& pixels, const WindowParametrs* const window_parametrs, size_t pixels_quant, RGBA GetPixelColor(const WindowParametrs* const , const PixelCoordinate* const))
{
    assert(&pixels);
    assert(window_parametrs);

    for (size_t i = 0; i < pixels_quant; i++)
        PaintPixel(pixels, i, window_parametrs, GetPixelColor);

    return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void PaintPixel(sf::VertexArray& pixel, size_t pixel_index, const WindowParametrs* const window_parametrs, RGBA (*GetPixelColor) (const WindowParametrs* const , const PixelCoordinate* const))
{
    assert(&pixel);
    assert(window_parametrs);

    PixelCoordinate pixel_coordinate = GetPixelCoordinate(&window_parametrs->window_size, pixel_index);
    RGBA            pixel_color      = GetPixelColor(window_parametrs, &pixel_coordinate);

    pixel[pixel_index].color    = sf::Color(pixel_color.r, pixel_color.g, pixel_color.b, pixel_color.a);
    pixel[pixel_index].position = sf::Vector2f((float) (pixel_coordinate.width_coordinate), (float) pixel_coordinate.high_coordinate);

    return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

PixelCoordinate GetPixelCoordinate(const WindowSize* const window_size, size_t pixel_index)
{
    assert(window_size);

    PixelCoordinate coordinate = {};

    coordinate.width_coordinate = (pixel_index % window_size->width);
    coordinate.high_coordinate  = (pixel_index - coordinate.width_coordinate) / window_size->width;

    return coordinate;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

size_t GetPixelsQuant(const WindowSize* const window_size)
{
    assert(window_size);

    return window_size->high * window_size->width;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

RGBA GetRgbaForBadPixel(size_t bad_iteration)
{
    assert(bad_iteration <= MaxIteration);


    unsigned char color = (unsigned char) (((float) bad_iteration / (float) MaxIteration) * 255.0);

    return {0, color, color, 255};
    // return {color, color, color, color};
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

RGBA GetRgbaForGoodPixel()
{
    return {100, 0, 0, 255};
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ComplexNumber ComplexNumberCtor(const WindowParametrs* const window_parametrs, const PixelCoordinate* const pixel_coordinate)
{
    ComplexNumber number = 
    {
        .real_part      = (((real_number_t) pixel_coordinate->width_coordinate) - (((real_number_t) window_parametrs->window_size.width) / 2.0 - window_parametrs->window_offset.width_offset)) / window_parametrs->window_offset.scale,
        .imaginary_part = (((real_number_t) pixel_coordinate->high_coordinate ) - (((real_number_t) window_parametrs->window_size.high ) / 2.0 - window_parametrs->window_offset.high_offset )) / window_parametrs->window_offset.scale,
    };

    return number;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

real_number_t GetAbsoluteValueOfComplexNumber(const ComplexNumber* const number)
{
    assert(number);

    real_number_t real_part = number->real_part;
    real_number_t imaginary_part = number->imaginary_part;

    real_number_t absolute_value = (real_part * real_part) + (imaginary_part * imaginary_part);

    return absolute_value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ComplexNumber GetNextMandelbrotSequenceNumber(const ComplexNumber* const number, const ComplexNumber* const first_sequence_elem)
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
