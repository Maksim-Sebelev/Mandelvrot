#include <stddef.h>
#include <SFML/Graphics.hpp>
#include <assert.h>

#include "lib/lib.hpp"
#include "mandelbrot/mandelbrot.hpp"
#include "mandelbrot/mandelbrot_global_include.hpp"

#ifdef _DEBUG
#include "log/log.hpp"
#endif // _DEBUG


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


static size_t GetPixelsQuant(const WindowSize* const window_size)
{
    assert(window_size);

    return window_size->high * window_size->width;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static WindowParametrs WindowParametrsCtor(const WindowSize* const window_size)
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

static real_number_t GetScale(const WindowSize* const window_size)
{
    assert(window_size);

    const size_t width = window_size->width;
    const size_t high  = window_size->high;

    return (real_number_t) sqrt(width * high) / 3.0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static WindowMovementInfo WindowMovementCtor()
{
    const real_number_t move_speed = 20.0;
    const real_number_t zoom_speed = 1.5;
    return {zoom_speed, move_speed};
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void CloseWindowIfNeed(sf::RenderWindow* window, sf::Event* event)
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

static void SFMLDtor(sf::RenderWindow* window, sf::VertexArray& pixels)
{
    assert(window);
    assert(&pixels);

    pixels.clear();
    window->clear();

    return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void UpadteSFMLWindow(sf::RenderWindow* window, sf::View* view, sf::VertexArray& pixels)
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

static void DrawPixelsArrayOnWindow(sf::RenderWindow* window, sf::VertexArray& pixels)
{
    assert(window);
    assert(&pixels);

    window->draw(pixels);
    window->display();

    return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void UpdateWindowOffset(WindofOffset* window_offset, const WindowMovementInfo* const window_movement)
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

static void MakePixelsArray(sf::VertexArray& pixels, const WindowParametrs* const window_parametrs, size_t pixels_quant)
{
    assert(&pixels);
    assert(window_parametrs);

    for (size_t i = 0; i < pixels_quant; i++)
        PaintPixel(pixels, i, window_parametrs);

    return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void PaintPixel(sf::VertexArray& pixel, size_t pixel_index, const WindowParametrs* const window_size)
{
    assert(&pixel);
    assert(window_size);

    PixelCoordinate pixel_coordinate = GetPixelCoordinate(window_size, pixel_index);
    RGBA            pixel_color      = GetPixelColor(window_size, &pixel_coordinate);

    pixel[pixel_index].color    = sf::Color(pixel_color.r, pixel_color.g, pixel_color.b, pixel_color.a);
    pixel[pixel_index].position = sf::Vector2f((float) (pixel_coordinate.width_coordinate), (float) pixel_coordinate.high_coordinate);

    return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
