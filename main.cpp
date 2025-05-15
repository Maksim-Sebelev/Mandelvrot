#include "mandelbrot/mandelbrot.hpp"

#include "lib/lib.hpp"
#ifdef _DEBUG
#include "log/log.hpp"
#endif // _DEBUG

#include <assert.h>

int main()
{
    ON_DEBUG(
    LOG_OPEN();
    COLOR_PRINT(GREEN, "\nMANDELBROT START\n\n");
    )

    WindowSize window_size =
    {
        .width = 1500, 
        .high  = 1000,
    };

    NativeMandelbrot(&window_size);

    ON_DEBUG(
    LOG_CLOSE();
    COLOR_PRINT(GREEN, "\nMANDELBROT END\n\n");
    )

    return 0;
}