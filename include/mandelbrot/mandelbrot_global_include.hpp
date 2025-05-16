
#include <stddef.h>
#include "mandelbrot/mandelbrot.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static const size_t        MaxIteration    = 64;
static const real_number_t MaxRadiusSquare = 4.0;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct ComplexNumber
{
    real_number_t real_part;
    real_number_t imaginary_part;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct WindowMovementInfo
{
    real_number_t zoom_speed;
    real_number_t move_speed;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct FPS
{
    float     fps;
    int       frame_count;
    float     fps_update_time;
    sf::Text  fps_text;
    sf::Clock fps_clock;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void               UpdateWindowOffset               (WindofOffset* window_offset, const WindowMovementInfo* const window_movement);
void               PaintPixel                       (sf::VertexArray& pixel, size_t pixel_index, const WindowParametrs* const window_parametrs, RGBA GetPixelColorconst(const WindowParametrs* const , const PixelCoordinate* const));
void               MakePixelsArray                  (sf::VertexArray& pixels, const WindowParametrs* const window_parametrs, size_t pixels_quant, RGBA GetPixelColorconst(const WindowParametrs* const , const PixelCoordinate* const));
void               CloseWindowIfNeed                (sf::RenderWindow* window, sf::Event* event);
void               DrawPixelsArrayOnWindow          (sf::RenderWindow* window, sf::VertexArray& pixels);
void               UpadteSFMLWindow                 (sf::RenderWindow* window, sf::VertexArray& pixels, const FPS* const fps);
void               SFMLDtor                         (sf::RenderWindow* window, sf::VertexArray& pixels);
ComplexNumber      ComplexNumberCtor                (const WindowParametrs* const window_parametrs, const PixelCoordinate* const pixel_coordinate);
PixelCoordinate    GetPixelCoordinate               (const WindowSize*      const window_size, size_t pixel_index);
ComplexNumber      GetNextMandelbrotSequenceNumber  (const ComplexNumber*   const number, const ComplexNumber* const first_sequence_number);
size_t             GetPixelsQuant                   (const WindowSize*      const window_size);
WindowParametrs    WindowParametrsCtor              (const WindowSize*      const window_size);
real_number_t      GetScale                         (const WindowSize*      const window_size);
WindowMovementInfo WindowMovementCtor               ();
real_number_t      GetAbsoluteValueOfComplexNumber  (const ComplexNumber*   const number);
RGBA               GetRgbaForBadPixel               (size_t bad_iteration);
RGBA               GetRgbaForGoodPixel              ();
RGBA               GetRgbaForCenterPixel            ();
bool               IsRealNumbersEqual               (real_number_t first_num, real_number_t second_num);
bool               IsPixelCenter                    (const PixelCoordinate* const pixel_coordinate, const WindowSize* const window_size);
void               CalcFps                          (FPS* fps);
FPS                FpsCtor                          ();
sf::Text           FpsTextCtor                      ();

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
