#include "../headers/common.hpp"

int randint(int min, int max)
{
    srand(time(NULL));
    return rand() % max + min;
}

SDL_Color toSDLColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_Color c = {r, g, b, a};
    return c;
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
    SDL_LockSurface(surface);

    Uint8 *pixels = (Uint8 *)surface->pixels;

    int dx = abs(x1 - x2);
    int dy = abs(y1 - y2);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int error = dx - dy;

    while (x1 != x2 || y1 != y2)
    {
        Uint8* pixel = pixels + (y1 * surface->pitch) + (x1 * surface->format->BytesPerPixel);
        *(Uint32*) pixel = color;

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
}
