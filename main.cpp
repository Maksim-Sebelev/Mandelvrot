#include "mandelbrot/mandelbrot.hpp"

#include "lib/lib.hpp"
#ifdef _DEBUG
#include "log/log.hpp"
#endif // _DEBUG


int main()
{
    ON_DEBUG(
    LOG_OPEN();
    COLOR_PRINT(GREEN, "\nMANDELBROT START\n\n");
    )


    WindowBorder window_size =
    {
        .width_begin = 0   ,
        .width_end   = 1500, 
        .high_begin  = 0   ,
        .high_end    = 1000,

    };

    NativeMandelbrot(&window_size);

    ON_DEBUG(
    LOG_CLOSE();
    COLOR_PRINT(GREEN, "\nMANDELBROT END\n\n");
    )

    return 0;
}