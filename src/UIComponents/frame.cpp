#include "../../QSDL++/UIComponents/Frame"

Frame::Frame(RUIComponent* parent, int width, int height) : RUIComponent(parent, width, height)
{
	this->overflowX = false;
	this->overflowY = false;
	this->movingScrollBarV = false;
	this->movingScrollBarH = false;
	this->overrideCompInsideBounds = false;
	this->topBorderVisible = false;
	this->bottomBorderVisible = false;
	this->leftBorderVisible = false;
	this->rightBorderVisible = false;
	this->scrollBarMovingDistY = 0;
	this->scrollBarMovingStartY = 0;
	this->scrollBarMovingDistX = 0;
	this->scrollBarMovingStartX = 0;
	this->pf = this->baseSurface->format;
	this->viewport = { 0, 0, width, height };
	this->DEFAULT_SCROLL_BAR_WIDTH = 15;
	this->setBordersColor({BLACK});
}

Color Frame::getBg()
{
	return this->bg;
}

bool Frame::isOverflowX()
{
	return this->overflowX;
}

bool Frame::isOverflowY()
{
	return this->overflowY;
}

bool Frame::insideBounds(int x, int y)
{
	if (!this->overrideCompInsideBounds)
	{
		// Traverse the members tree and get all components
		vector<RUIComponent*> nodes;
		vector<RUIComponent*> stack = { this };

		while (stack.size() > 0)
		{
			RUIComponent* node = stack.at(stack.size() - 1);
			stack.pop_back();
			nodes.push_back(node);

			Frame* nodeFrame = dynamic_cast<Frame*>(node);
			if (nodeFrame)
			{
				for (RUIComponent* c : nodeFrame->getMembers())
					stack.push_back(c);
			}
		}

		for (int i = 1; i < nodes.size(); i++)
		{
			if (nodes[i]->insideBounds(x, y))
				return false;
		}
	}

	if (!RUIComponent::insideBounds(x, y))
		return false;

	if (this->parent)
	{
		if (!(this->getAbsPosition().x - parviewport.x <= x && x <= this->getAbsPosition().x + this->viewport.w - parviewport.x && this->getAbsPosition().y - parviewport.y <= y && y <= this->getAbsPosition().y + this->viewport.h - parviewport.y))
			return false;
	}
	else
	{
		if (!(this->getAbsPosition().x <= x && x <= this->getAbsPosition().x + this->viewport.w && this->getAbsPosition().y <= y && y <= this->getAbsPosition().y + this->viewport.h))
			return false;
	}

	return true;
}

void Frame::setBg(Color bg)
{
	this->bg = bg;
	this->fill(bg);
}

void Frame::setPosition(int x, int y)
{
	RUIComponent::setPosition(x, y);

	for (RUIComponent* comp : this->members)
	{
		Point compPos = comp->getPosition();
		comp->setAbsPosition(x + compPos.x, y + compPos.y); // comp->setPosition(compPos.x, compPos.y);
	}
}

void Frame::setAbsPosition(int x, int y)
{
	RUIComponent::setAbsPosition(x, y);

	for (RUIComponent* comp : this->members)
	{
		Point compPos = comp->getPosition();
		comp->setAbsPosition(x + compPos.x, y + compPos.y);
	}
}

void Frame::setSize(Uint16 width, Uint16 height)
{
	RUIComponent::setSize(width, height);

	this->setViewport(0, 0, width, height);
}

void Frame::setOverflowX(bool ovf)
{
	this->overflowX = ovf;
}

void Frame::setOverflowY(bool ovf)
{
	this->overflowY = ovf;
}

void Frame::setViewport(int x, int y, int w, int h)
{
	if (!(0 <= x && x <= this->getSize().w && 0 <= y && y <= this->getSize().h && 0 <= w + x && w + x <= this->getSize().w && 0 <= h + y && h + y <= this->getSize().h))
	{
		printf("WARNING: invalid viewport bounds, got: x: %i, y: %i, w: %i, h: %i\n", x, y, w, h);
	}

	this->viewport = { x, y, w, h };

	for (RUIComponent* comp : this->members)
	{
		comp->parviewport = { x, y, w, h };
	}
}

void Frame::add(RUIComponent* component)
{
	this->members.push_back(component);
	component->parviewport = this->viewport;
}

void Frame::pop(RUIComponent* component)
{
	for (int i = 0; i < this->members.size(); i++)
	{
		if (this->members.at(i) == component)
		{
			this->members.erase(this->members.begin() + i);
			return;
		}
	}

	printf("WARNING: Cannot delete non existing component.\n");
}

void Frame::clear()
{
	this->members.clear();
}

vector<RUIComponent*> Frame::getMembers()
{
	return this->members;
}

void Frame::update()
{
	if (this->movingScrollBarV)
	{
		if (0 <= this->viewport.y + this->scrollBarMovingDistY && this->viewport.y + this->scrollBarMovingDistY <= this->getSize().h - this->viewport.h)
		{
			this->scrollV(this->scrollBarMovingDistY);
		}

		this->scrollBarMovingDistY = 0;
	}

	if (this->movingScrollBarH)
	{
		if (0 <= this->viewport.x + this->scrollBarMovingDistX && this->viewport.x + this->scrollBarMovingDistX <= this->getSize().w - this->viewport.w)
		{
			this->scrollH(this->scrollBarMovingDistX);
		}

		this->scrollBarMovingDistX = 0;
	}

	if (this->isOverflowY())
		this->drawVScrollBar();

	if (this->isOverflowX())
		this->drawHScrollBar();

	this->drawBorders();
}

void Frame::drawBorders()
{
	// drawing borders
	Uint32 color = SDL_MapRGBA(this->pf, this->bordersColor.r, this->bordersColor.g, this->bordersColor.b, this->bordersColor.a);
	
    if (this->topBorderVisible)
        drawLineToSurface( // top line
            this->baseSurface,
            0, 0,
            this->getSize().w, 0,
            color);

    if (this->bottomBorderVisible)
        drawLineToSurface( // bottom line
            this->baseSurface,
            0, this->getSize().h - 1,
            this->getSize().w, this->getSize().h - 1,
            color);

    if (this->leftBorderVisible)
        drawLineToSurface( // left line
            this->baseSurface,
            0, 0,
            0, this->getSize().h,
            color);

    if (this->rightBorderVisible)
        drawLineToSurface( // right line
            this->baseSurface,
            this->getSize().w - 1, 0,
            this->getSize().w - 1, this->getSize().h - 1,
            color);
}

void Frame::drawVScrollBar()
{
	int height = this->getSize().h;

	// draw scroll bar container
	drawLineToSurface(this->baseSurface,
		this->viewport.w + this->viewport.x - this->DEFAULT_SCROLL_BAR_WIDTH, 0,
		this->viewport.w + this->viewport.x - this->DEFAULT_SCROLL_BAR_WIDTH,
		this->viewport.h + this->viewport.y,
		SDL_MapRGBA(this->baseSurface->format, BLACK));

	float ratio = (float)this->viewport.y / height;
	int thumbHeight = ((float)(this->viewport.h - (this->overflowX ? this->DEFAULT_SCROLL_BAR_WIDTH : 0)) / height) * viewport.h;
	SDL_Surface* c = SDL_CreateRGBSurfaceWithFormat(0,
		this->DEFAULT_SCROLL_BAR_WIDTH, thumbHeight,
		this->baseSurface->format->BitsPerPixel,
		this->baseSurface->format->format);
	ENSURE_NOT(c, NULL);
	ENSURE(SDL_FillRect(c, NULL, SDL_MapRGBA(this->baseSurface->format, BLACK)), 0);

	SDL_Rect r = { this->viewport.w + this->viewport.x - c->w, (int)ceil((height / ((float)(this->viewport.h - (this->overflowX ? this->DEFAULT_SCROLL_BAR_WIDTH : 0))) + 1) * ratio * (this->viewport.h - (this->overflowX ? this->DEFAULT_SCROLL_BAR_WIDTH : 0))), c->w, c->h };

	ENSURE(SDL_BlitSurface(c, NULL, this->baseSurface, &r), 0);
	SDL_FreeSurface(c);
}

void Frame::drawHScrollBar()
{
	int width = this->getSize().w;

	// draw scroll bar container
	drawLineToSurface(this->baseSurface, 0,
		this->viewport.h + this->viewport.y - this->DEFAULT_SCROLL_BAR_WIDTH, width,
		this->viewport.h + this->viewport.y - this->DEFAULT_SCROLL_BAR_WIDTH,
		SDL_MapRGBA(this->baseSurface->format, BLACK));

	// draw scroll thumb
	float ratio = (float)this->viewport.x / width;
	int thumbWidth = ((float)(this->viewport.w - (this->overflowY ? this->DEFAULT_SCROLL_BAR_WIDTH : 0)) / width) * viewport.w;
	SDL_Surface* c = SDL_CreateRGBSurfaceWithFormat(0, thumbWidth,
		this->DEFAULT_SCROLL_BAR_WIDTH,
		this->baseSurface->format->BitsPerPixel,
		this->baseSurface->format->format);
	ENSURE_NOT(c, NULL);
	ENSURE(SDL_FillRect(c, NULL, SDL_MapRGBA(this->baseSurface->format, BLACK)), 0);

	SDL_Rect r =
	{ (int)ceil(
		 (width / ((float)(this->viewport.w - (this->overflowY ? this->DEFAULT_SCROLL_BAR_WIDTH : 0))) + 1) * ratio * (this->viewport.w - (this->overflowY ? this->DEFAULT_SCROLL_BAR_WIDTH : 0))),
	 this->viewport.h + this->viewport.y - c->h, c->w, c->h };

	ENSURE(SDL_BlitSurface(c, NULL, this->baseSurface, &r), 0);
	SDL_FreeSurface(c);
}

void Frame::scrollV(int step)
{
	this->setViewport(this->viewport.x, this->viewport.y + step,
		this->viewport.w, this->viewport.h);
}

void Frame::scrollH(int step)
{
	this->setViewport(this->viewport.x + step, this->viewport.y,
		this->viewport.w, this->viewport.h);
}

void Frame::allowEventPropagation(bool allow)
{
	this->overrideCompInsideBounds = allow;
}

void Frame::invokeEvents(Event e)
{
	if (this->isOverflowX())
	{
		this->overrideCompInsideBounds = this->insideHScrollBarArea(e.button.x, e.button.y)
			|| this->insideHScrollBarArea(e.wheel.mouseX, e.wheel.mouseY);
	}
	else if (this->isOverflowY())
	{
		this->overrideCompInsideBounds = this->insideVScrollBarArea(e.button.x, e.button.y)
			|| this->insideVScrollBarArea(e.wheel.mouseX, e.wheel.mouseY);
	}
	else
	{
		this->overrideCompInsideBounds = false;
	}

	RUIComponent::invokeEvents(e);

	int step = 15;
	if (e.type == EventType::MOUSE_WHEEL)
	{
		if (!this->insideBounds(e.wheel.mouseX, e.wheel.mouseY))
			return;

		if (this->isOverflowY() && (!this->insideHScrollBarArea(e.wheel.mouseX, e.wheel.mouseY) || this->insideVScrollBarArea(e.wheel.mouseX,
			e.wheel.mouseY)))
		{
			if (e.wheel.y > 0) // Scrolling down
			{
				if (0 <= this->viewport.y - step && this->viewport.y - step <= this->getSize().h - this->viewport.h)
				{
					this->scrollV(-step);
				}
				else
				{
					this->setViewport(this->viewport.x, 0, this->viewport.w,
						this->viewport.h);
				}
			}
			else if (e.wheel.y < 0) // Scrolling up
			{
				if (0 <= this->viewport.y + step && this->viewport.y + step <= this->getSize().h - this->viewport.h)
				{
					this->scrollV(step);
				}
				else
				{
					this->setViewport(this->viewport.x,
						this->getSize().h - this->viewport.h,
						this->viewport.w, this->viewport.h);
				}
			}
		}

		if (this->isOverflowX() && this->insideHScrollBarArea(e.wheel.mouseX, e.wheel.mouseY))
		{
			if (e.wheel.y > 0) // Scrolling left
			{
				if (0 <= this->viewport.x - step && this->viewport.x - step <= this->getSize().w - this->viewport.w)
				{
					this->scrollH(-step);
				}
				else
				{
					this->setViewport(0, this->viewport.y, this->viewport.w,
						this->viewport.h);
				}
			}
			else if (e.wheel.y < 0) // Scrolling right
			{
				if (0 <= this->viewport.x + step && this->viewport.x + step <= this->getSize().w - this->viewport.w)
				{
					this->scrollH(step);
				}
				else
				{
					this->setViewport(this->getSize().w - this->viewport.w,
						this->viewport.y, this->viewport.w,
						this->viewport.h);
				}
			}
		}
	}
}

void Frame::setBordersVisible(bool top, bool right, bool bottom, bool left)
{
	this->topBorderVisible = top;
    this->bottomBorderVisible = bottom;
    this->rightBorderVisible = right;
    this->leftBorderVisible = left;
    this->update();
}

void Frame::setBordersVisible(bool visible)
{
	this->setBordersVisible(visible, visible, visible, visible);
}

void Frame::setBordersColor(Color color)
{
	this->bordersColor = color;
    this->update();
}

Frame::~Frame()
{
}

bool Frame::insideVScrollBarArea(int x, int y)
{
	if (this->parent)
	{
		return this->getAbsPosition().x + this->viewport.w - this->DEFAULT_SCROLL_BAR_WIDTH - this->parviewport.x <= x && x <= this->getAbsPosition().x + this->viewport.w - this->parviewport.x && this->getAbsPosition().y - this->parviewport.y <= y && y <= this->getAbsPosition().y + this->viewport.h - this->parviewport.y;
	}
	else
	{
		return this->getAbsPosition().x + this->viewport.w - this->DEFAULT_SCROLL_BAR_WIDTH <= x && x <= this->getAbsPosition().x + this->viewport.w && this->getAbsPosition().y <= y && y <= this->getAbsPosition().y + this->viewport.h;
	}
}

bool Frame::insideHScrollBarArea(int x, int y)
{
	if (this->parent)
	{
		return this->getAbsPosition().x - parviewport.x <= x && x <= this->getAbsPosition().x + this->viewport.w - parviewport.x && this->getAbsPosition().y + this->viewport.h - this->DEFAULT_SCROLL_BAR_WIDTH - parviewport.y <= y && y <= this->getAbsPosition().y + this->viewport.h - parviewport.y;
	}
	else
	{
		return this->getAbsPosition().x <= x && x <= this->getAbsPosition().x + this->viewport.w && this->getAbsPosition().y + this->viewport.h - this->DEFAULT_SCROLL_BAR_WIDTH <= y && y <= this->getAbsPosition().y + this->viewport.h;
	}
}

void Frame::onMousePressed(Event e)
{
	RUIComponent::onMousePressed(e);

	if (this->insideVScrollBarArea(e.button.x, e.button.y))
	{
		this->movingScrollBarV = true;
		this->scrollBarMovingStartY = e.button.y;
		this->scrollBarMovingDistY = 0;
	}
	else if (this->insideHScrollBarArea(e.button.x, e.button.y))
	{
		this->movingScrollBarH = true;
		this->scrollBarMovingStartX = e.button.x;
		this->scrollBarMovingDistX = 0;
	}
}

void Frame::onMouseReleased(Event e)
{
	RUIComponent::onMouseReleased(e);

	this->movingScrollBarV = false;
	this->movingScrollBarH = false;
}

void Frame::onMouseMotion(Event e)
{
	RUIComponent::onMouseMotion(e);

	if (this->movingScrollBarV && this->isOverflowY())
	{
		this->scrollBarMovingDistY = e.button.y - this->scrollBarMovingStartY;
		this->scrollBarMovingStartY = e.button.y;
	}

	if (this->movingScrollBarH && this->isOverflowX())
	{
		this->scrollBarMovingDistX = e.button.x - this->scrollBarMovingStartX;
		this->scrollBarMovingStartX = e.button.x;
	}
}

void Frame::onMouseLeaved(Event e)
{
	RUIComponent::onMouseLeaved(e);

	this->movingScrollBarV = false;
	this->movingScrollBarH = false;
}
