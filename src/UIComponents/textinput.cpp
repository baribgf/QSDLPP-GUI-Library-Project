#include "../../headers/UIComponents/textinput.hpp"

#define DISPLAY_NUMBER this->getSize().w / (this->textW / this->text.length())

TextInput::TextInput(RUIComponent *parent, int width, int height) : Label(parent, "", width, height)
{
    this->shmnt = 0;
    this->textToRender = "";
    this->cursorIndex = 0;
    this->inspecting = false;
    this->uppercase = false;
    this->ctrl = false;
    this->setBordersVisible(true);
    this->setTextAlign(Align::WEST);
}

TextInput::~TextInput()
{
}

void TextInput::drawText()
{
    if (text.length() > 0)
    {
        SDL_Rect r = {this->textX + 2, this->textY, this->getSize().w, this->getSize().h};
        TTF_Font *font = TTF_OpenFont(this->FONT_PATH.c_str(), this->textSize);
        ENSURE_NOT(font, NULL);

        SDL_Surface *textSurface = TTF_RenderText_Blended(
            font,
            text.c_str(),
            {this->fg.r, this->fg.g, this->fg.b, this->fg.a});

        this->textW = textSurface->w;
        this->textH = textSurface->h;

        textSurface = TTF_RenderText_Blended(
            font,
            this->textToRender.c_str(),
            {this->fg.r, this->fg.g, this->fg.b, this->fg.a});

        ENSURE_NOT(textSurface, NULL);

        ENSURE(SDL_BlitSurface(
                   textSurface,
                   NULL,
                   this->baseSurface,
                   &r),
               0);

        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
    }
    else
    {
        this->textW = 0;
        this->textH = this->getSize().h;
    }
}

void TextInput::drawCursor()
{
    Uint32 color = SDL_MapRGBA(this->baseSurface->format, 0, 0, 0, 255);
    Uint16 width = this->getSize().w;
    Uint16 height = this->getSize().h;
    Uint8 hSpace = this->text.length() > 0 ? width * 0.005 : width * 0.02;
    Uint8 vSpace = this->getSize().h * 0.15;

    int cursorX;
    if (this->text.length() > 0)
    {
        cursorX = this->textW / this->text.length() * (this->cursorIndex - this->shmnt) + hSpace;

        if (this->cursorIndex <= this->shmnt)
        {
            cursorX = hSpace + 2;
        }

        if (cursorX > width - hSpace)
        {
            cursorX = width - hSpace;
        }
    }
    else
    {
        cursorX = hSpace;
    }

    drawLineToSurface(this->baseSurface,
                      cursorX, vSpace,
                      cursorX, height - vSpace,
                      color);
}

void TextInput::update()
{
    this->fill(this->fillColor);

    this->drawBorders();

    if (this->imgPath.length() > 0)
        this->drawImage();

    this->drawText();

    if (this->hasFocus())
        this->drawCursor();
}

void TextInput::setFocus(bool focus)
{
    Label::setFocus(focus);

    this->update();
}

void TextInput::onKeyReleased(Event e)
{
    RUIComponent::onKeyReleased(e);

    if (e.key.type == KeyCode::KEY_LSHIFT)
        this->uppercase = false;

    else if (e.key.type == KeyCode::KEY_LCTRL)
    {
        this->ctrl = false;
    }
}

void TextInput::onKeyPressed(Event e)
{
    RUIComponent::onKeyPressed(e);

    string text = this->getText();

    if (e.key.type == KeyCode::KEY_BACKSPACE)
    {
        if (this->inspecting)
        {
            if (text.length() > 0 && cursorIndex > 0)
            {
                string t = text.substr(0, cursorIndex - 1) + text.substr(cursorIndex, text.length());
                this->cursorIndex--;
                if (this->textW > 0)
                    this->textToRender = t.substr(this->shmnt, DISPLAY_NUMBER);
                this->setText(t);
            }
        }
        else
        {
            if (text.length() > 0 && cursorIndex > 0)
            {
                string t = text.substr(0, cursorIndex - 1) + text.substr(cursorIndex, text.length());
                this->cursorIndex--;
                this->textToRender = t;
                this->setText(t);
            }

            if (this->textW > 0 && this->text.length() >= DISPLAY_NUMBER)
            {
                this->shmnt--;
                this->textToRender = this->text.substr(this->text.size() - (this->getSize().w / (this->textW / this->text.length())), this->text.size());
                this->update();
            }
        }
    }
    else if (e.key.type == KeyCode::KEY_LSHIFT)
    {
        this->uppercase = true;
    }
    else if (e.key.type == KeyCode::KEY_LCTRL)
    {
        this->ctrl = true;
    }
    else if (e.key.type == KeyCode::KEY_LEFT)
    {
        this->cursorIndex = max(0, this->cursorIndex - 1);

        if (this->cursorIndex < this->shmnt)
        { // shift display right
            this->textToRender = this->text.substr(
                cursorIndex,
                DISPLAY_NUMBER);

            this->shmnt--;
        }

        this->update();
    }
    else if (e.key.type == KeyCode::KEY_RIGHT)
    {
        this->cursorIndex = min((int)text.length(), this->cursorIndex + 1);

        if (this->textW > 0 && this->cursorIndex > this->shmnt + DISPLAY_NUMBER)
        { // shift display left
            this->textToRender = this->text.substr(
                cursorIndex - DISPLAY_NUMBER,
                DISPLAY_NUMBER);

            this->shmnt++;
        }

        if (this->textW > 0 && this->text.length() > DISPLAY_NUMBER)
            this->inspecting = true;

        if (this->cursorIndex == this->text.length())
            this->inspecting = false;

        this->update();
    }
    else if (e.key.type == KeyCode::KEY_HOME)
    {
        this->cursorIndex = 0;
        this->shmnt = 0;
        if (textW > 0)
            this->textToRender = this->text.substr(0, DISPLAY_NUMBER);
        this->update();
    }
    else if (e.key.type == KeyCode::KEY_END)
    {
        this->inspecting = false;
        this->cursorIndex = text.length();

        if (textW > 0)
        {
            this->shmnt = max(0, (int)((int)this->text.length() - (int)DISPLAY_NUMBER));
            if (shmnt > 0)
                this->textToRender = this->text.substr(text.length() - DISPLAY_NUMBER);
        }
        this->update();
    }
    else if (32 <= e.key.type && e.key.type <= 126)
    {
        if (this->inspecting)
        { // among inspecting mode
            if (uppercase)
            {
                string t = text.substr(0, cursorIndex) + ((char)(e.key.type - 32)) + text.substr(cursorIndex);
                this->cursorIndex++;
                if (this->textW > 0)
                    this->textToRender = t.substr(this->shmnt, DISPLAY_NUMBER);
                this->setText(t);
            }
            else
            {
                if ((char)(e.key.type) == 'v' && this->ctrl)
                {
                    char *cbText = SDL_GetClipboardText();
                    if (string(cbText).length() > 0)
                    {
                        string t = text.substr(0, cursorIndex) + string(cbText) + text.substr(cursorIndex);
                        this->cursorIndex += string(cbText).length();
                        this->textToRender = t;
                        this->setText(t);
                    }

                    SDL_free(cbText);
                }
                else
                {
                    string t = text.substr(0, cursorIndex) + (char)(e.key.type) + text.substr(cursorIndex);
                    this->cursorIndex++;
                    if (this->textW > 0)
                        this->textToRender = t.substr(this->shmnt, DISPLAY_NUMBER);
                    this->setText(t);
                }
            }
        }
        else
        { // among noramal mode
            if (uppercase)
            {
                string t = text.substr(0, cursorIndex) + ((char)(e.key.type - 32)) + text.substr(cursorIndex);
                this->cursorIndex++;
                this->textToRender = t;
                this->setText(t);
            }
            else
            {
                if ((char)(e.key.type) == 'v' && this->ctrl)
                {
                    char *cbText = SDL_GetClipboardText();
                    if (string(cbText).length() > 0)
                    {
                        string t = text.substr(0, cursorIndex) + string(cbText) + text.substr(cursorIndex);
                        this->cursorIndex += string(cbText).length();
                        this->textToRender = t;
                        this->setText(t);
                    }

                    SDL_free(cbText);
                }
                else
                {
                    string t = text.substr(0, cursorIndex) + (char)(e.key.type) + text.substr(cursorIndex);
                    this->cursorIndex++;
                    this->textToRender = t;
                    this->setText(t);
                }
            }

            if (this->textW > 0 && this->text.length() > DISPLAY_NUMBER)
            {
                this->shmnt++;
                this->textToRender = this->text.substr(this->text.size() - (DISPLAY_NUMBER), this->text.size());
                this->update();
            }
        }
    }
}
