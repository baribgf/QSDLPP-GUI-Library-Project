#include "../QSDL++/Common"

int randint(int min, int max)
{
    srand(time(NULL));
    return rand() % max + min;
}

int pointsToPixels(int points)
{
    return points * (1.0 / 3.0 + 1.0);
}

int pixelsToPoints(int pixels)
{
    return pixels / (1.0 / 3.0 + 1.0);
}

void drawLineToSurface(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 color)
{
    /*
    SDL_LockSurface(surface);

    Uint8 *pixels = (Uint8 *)surface->pixels;

    int dx = abs(x1 - x2);
    int dy = abs(y1 - y2);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int error = dx - dy;

    while (x1 != x2 || y1 != y2)
    {
        Uint8 *pixel = pixels + (y1 * surface->pitch) + (x1 * surface->format->BytesPerPixel);
        *(Uint32 *)pixel = color;

        int error2 = 2 * error;
        if (error2 > -dy)
        {
            error -= dy;
            x1 += sx;
        }
        if (error2 < dx)
        {
            error += dx;
            y1 += sy;
        }
    }

    SDL_UnlockSurface(surface);
    */

    // Bresenham's line algorithm
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
        // Set the pixel color at the current position
        if (x1 >= 0 && x1 < surface->w && y1 >= 0 && y1 < surface->h)
        {
            Uint8 *pixelAddress = static_cast<Uint8 *>(surface->pixels) + y1 * surface->pitch + x1 * 4;
            Uint32 *pixel = reinterpret_cast<Uint32 *>(pixelAddress);
            *pixel = color;
        }

        // Break if the end point is reached
        if (x1 == x2 && y1 == y2)
            break;

        int err2 = 2 * err;

        // Adjust the coordinates based on the error value
        if (err2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (err2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void delay(Uint32 ms)
{
    SDL_Delay(ms);
}

Uint64 getTicks()
{
    return SDL_GetTicks64();
}

string exec(const char *cmd)
{
    array<char, 128> buffer;
    string output;

    FILE *pipe = popen(cmd, "r");
    if (pipe)
    {
        while (!feof(pipe))
        {
            if (fgets(buffer.data(), 128, pipe) != nullptr)
                output += buffer.data();
        }

        pclose(pipe);
    }

    return output;
}

SDL_SystemCursor toSDLCursor(Cursor c)
{
    SDL_SystemCursor sc;
    switch (c)
    {
    case CURSOR_ARROW:
        sc = SDL_SYSTEM_CURSOR_ARROW;
        break;
    case CURSOR_IBEAM:
        sc = SDL_SYSTEM_CURSOR_IBEAM;
        break;
    case CURSOR_WAIT:
        sc = SDL_SYSTEM_CURSOR_WAIT;
        break;
    case CURSOR_CROSSHAIR:
        sc = SDL_SYSTEM_CURSOR_CROSSHAIR;
        break;
    case CURSOR_WAITARROW:
        sc = SDL_SYSTEM_CURSOR_WAITARROW;
        break;
    case CURSOR_SIZENWSE:
        sc = SDL_SYSTEM_CURSOR_SIZENWSE;
        break;
    case CURSOR_SIZENESW:
        sc = SDL_SYSTEM_CURSOR_SIZENESW;
        break;
    case CURSOR_SIZEWE:
        sc = SDL_SYSTEM_CURSOR_SIZEWE;
        break;
    case CURSOR_SIZENS:
        sc = SDL_SYSTEM_CURSOR_SIZENS;
        break;
    case CURSOR_SIZEALL:
        sc = SDL_SYSTEM_CURSOR_SIZEALL;
        break;
    case CURSOR_NO:
        sc = SDL_SYSTEM_CURSOR_NO;
        break;
    case CURSOR_HAND:
        sc = SDL_SYSTEM_CURSOR_HAND;
        break;
    }

    return sc;
}

void setCursor(Cursor cursor)
{
    SDL_Cursor *c = SDL_CreateSystemCursor(toSDLCursor(cursor));
    SDL_SetCursor(c);
    SDL_ShowCursor(SDL_ENABLE);
    // SDL_FreeCursor(c);
}
