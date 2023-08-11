#include "../../QSDL++/UIComponents/Canvas"

Canvas::Canvas(UIComponent *parent, Uint16 width, Uint16 height) : RUIComponent(parent, width, height)
{
    this->drawColor = {BLACK};
    this->fillColor = {0, 0, 0, 0};
    this->fill(this->getFillColor());
}

Color Canvas::getDrawColor()
{
    return this->drawColor;
}

void Canvas::setDrawColor(Color color)
{
    this->drawColor = color;
}

void Canvas::drawLine(int x1, int y1, int x2, int y2)
{
    drawLineToSurface(this->baseSurface, x1, y1, x2, y2, SDL_MapRGBA(this->baseSurface->format, this->drawColor.r, this->drawColor.g, this->drawColor.b, this->drawColor.a));
}

void Canvas::drawArc(int x, int y, int radius, int startAngle, int endAngle)
{
    // Calculate start and end angles in radians
    double startRadians = (360 - endAngle) * M_PI / 180.0;
    double endRadians = (360 - startAngle) * M_PI / 180.0;

    SDL_Surface *surface = this->baseSurface;

    // Iterate over each pixel in the surface
    for (int px = 0; px < surface->w; px++)
    {
        for (int py = 0; py < surface->h; py++)
        {
            // Calculate relative coordinates for current pixel
            int dx = px - x;
            int dy = py - y;

            // Calculate angle for current pixel
            double pixelAngle = atan2(dy, dx);

            // Normalize the pixel angle to be between 0 and 2π
            if (pixelAngle < 0)
            {
                pixelAngle += 2 * M_PI;
            }

            // Ensure angle is within the start and end range
            if (pixelAngle >= startRadians && pixelAngle <= endRadians)
            {
                // Calculate distance from center to current pixel
                double distance = sqrt(dx * dx + dy * dy);

                // Check if distance is within the radius
                if (distance >= radius - 1 && distance <= radius)
                {
                    // Set the pixel color
                    Uint8 *pixelAddress = static_cast<Uint8 *>(surface->pixels) + py * surface->pitch + px * 4;
                    Uint32 *pixel = reinterpret_cast<Uint32 *>(pixelAddress);
                    *pixel = SDL_MapRGBA(surface->format, this->drawColor.r, this->drawColor.g, this->drawColor.b, this->drawColor.a);
                }
            }
        }
    }
}

void Canvas::drawFilledArc(int x, int y, int radius, int startAngle, int endAngle)
{
    // Calculate start and end angles in radians
    double startRadians = (360 - endAngle) * M_PI / 180.0;
    double endRadians = (360 - startAngle) * M_PI / 180.0;

    SDL_Surface *surface = this->baseSurface;

    // Iterate over each pixel in the surface
    for (int px = 0; px < surface->w; px++)
    {
        for (int py = 0; py < surface->h; py++)
        {
            // Calculate relative coordinates for current pixel
            int dx = px - x;
            int dy = py - y;

            // Calculate angle for current pixel
            double pixelAngle = atan2(dy, dx);

            // Normalize the pixel angle to be between 0 and 2π
            if (pixelAngle < 0)
            {
                pixelAngle += 2 * M_PI;
            }

            // Ensure angle is within the start and end range
            if (pixelAngle >= startRadians && pixelAngle <= endRadians)
            {
                // Calculate distance from center to current pixel
                double distance = sqrt(dx * dx + dy * dy);

                // Check if distance is within the radius
                if (distance <= radius)
                {
                    // Set the pixel color
                    Uint8 *pixelAddress = static_cast<Uint8 *>(surface->pixels) + py * surface->pitch + px * 4;
                    Uint32 *pixel = reinterpret_cast<Uint32 *>(pixelAddress);
                    *pixel = SDL_MapRGBA(surface->format, this->drawColor.r, this->drawColor.g, this->drawColor.b, this->drawColor.a);
                }
            }
        }
    }
}

void Canvas::drawRect(int x, int y, int width, int height)
{
    SDL_Surface *rect = SDL_CreateRGBSurfaceWithFormat(0, width, height, this->baseSurface->format->BitsPerPixel, this->baseSurface->format->format);
    ENSURE_NOT(rect, NULL);
    ENSURE(SDL_FillRect(rect, NULL, SDL_MapRGBA(this->baseSurface->format, this->drawColor.r, this->drawColor.g, this->drawColor.b, this->drawColor.a)), 0);
    SDL_Rect r = {x, y, width, height};
    ENSURE(SDL_BlitSurface(rect, NULL, this->baseSurface, &r), 0);
    SDL_FreeSurface(rect);
}

void Canvas::drawPoint(int x, int y)
{
    SDL_Surface *surface = this->baseSurface;
    // Check if the coordinates are within the surface bounds
    if (x >= 0 && x < surface->w && y >= 0 && y < surface->h)
    {
        // Calculate the address of the pixel
        Uint8 *pixelAddress = static_cast<Uint8 *>(surface->pixels) + y * surface->pitch + x * 4;
        Uint32 *pixel = reinterpret_cast<Uint32 *>(pixelAddress);

        // Set the pixel color
        *pixel = SDL_MapRGBA(surface->format, this->drawColor.r, this->drawColor.g, this->drawColor.b, this->drawColor.a);
    }
}

void Canvas::rotate()
{
    this->baseSurface = rotatedSurface(this->baseSurface);
}

Canvas::~Canvas()
{
}
