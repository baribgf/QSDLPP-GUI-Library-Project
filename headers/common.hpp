#ifndef COMMON_HEADER
#define COMMON_HEADER

#ifndef _GLIBCXX_IOSTREAM
#include <iostream>
using namespace std;
#endif

#ifndef SDL_h_
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

#ifndef _GLIBCXX_VECTOR
#include <vector>
#endif

#define debug(x) std::cout << x << std::endl;
#define TEST cout << "test: " << SDL_GetTicks() << endl;

#define WHITE 255, 255, 255, 255
#define BLACK 0  , 0  , 0  , 255
#define RED   255, 0  , 0  , 255
#define GREEN 0  , 255, 0  , 255
#define BLUE  0  , 0  , 255, 255

int randint(int min, int max);
SDL_Color toSDLColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
int pointsToPixels(int points);
int pixelsToPoints(int pixels);

void drawLineToSurface(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 color);

typedef struct
{
    Uint16 w;
    Uint16 h;
} SDL_Size;

enum Align
{
    NORTH      = 0,
    EAST       = 1,
    SOUTH      = 2,
    WEST       = 3,
    NORTH_EAST = 4,
    NORTH_WEST = 5,
    SOUTH_EAST = 6,
    SOUTH_WEST = 7,
    CENTER     = 8
};

enum EventButtonType
{
    LEFT_BUTTON   = SDL_BUTTON_LEFT,
    RIGHT_BUTTON  = SDL_BUTTON_RIGHT,
    MIDDLE_BUTTON = SDL_BUTTON_MIDDLE
};

enum EventType
{
    MOUSE_DOWN    = SDL_MOUSEBUTTONDOWN,
    MOUSE_UP      = SDL_MOUSEBUTTONDOWN,
    MOUSE_MOTION  = SDL_MOUSEMOTION,
    KEY_DOWN      = SDL_KEYDOWN,
    KEY_UP        = SDL_KEYUP,
    WINDOW_RESIZE = SDL_WINDOWEVENT_RESIZED,
    QUIT          = SDL_QUIT
};

typedef struct
{
    Uint8 type;
    int x;
    int y;
} EventButton;

#endif
