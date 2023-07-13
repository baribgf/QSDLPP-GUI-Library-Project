#include "../../headers/UIComponents/textinput.hpp"

TextInput::TextInput(RUIComponent *parent, int width, int height) : Label(parent, "", width, height)
{
    this->cursorIndex = 0;
    this->uppercase = false;
    this->setBordersVisible(true);
    this->setTextAlign(Align::WEST);
}

TextInput::~TextInput()
{
}

void TextInput::drawCursor()
{
    Uint32 color = SDL_MapRGBA(this->baseSurface->format, 0, 0, 0, 255);
    Uint16 width = this->getSize().w;
    Uint8 hSpace = this->text.length() > 0 ? width * 0.005 : width * 0.02;
    Uint8 vSpace = this->getSize().h * 0.12;

    Uint32 cursorX;
    if (this->text.length() > 0)
    {
        cursorX = (this->textW / this->text.length()) * this->cursorIndex + hSpace + this->textX;

        if (cursorX > width) // in text overflow..
        {
            cursorX = width * (1 - 0.02);
        }
    }
    else
    {
        cursorX = hSpace;
    }

    drawLineToSurface(this->baseSurface,
                      cursorX, 0 + vSpace,
                      cursorX, this->textH - vSpace,
                      color);
}

void TextInput::update()
{
    Label::update();

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
}

void TextInput::onKeyPressed(Event e)
{
    RUIComponent::onKeyPressed(e);

    string text = this->getText();

    if (e.key.type == KeyCode::KEY_BACKSPACE)
    {
        if (text.length() > 0 && cursorIndex > 0)
        {
            this->cursorIndex = max(0, this->cursorIndex - 1);
            this->setText(text.substr(0, cursorIndex) + text.substr(cursorIndex + 1, text.length()));
        }
    }
    else if (e.key.type == KeyCode::KEY_LSHIFT)
    {
        this->uppercase = true;
    }
    else if (e.key.type == KeyCode::KEY_LEFT)
    {
        this->cursorIndex = max(0, this->cursorIndex - 1);
        this->update();
    }
    else if (e.key.type == KeyCode::KEY_RIGHT)
    {
        this->cursorIndex = min((int)text.length(), this->cursorIndex + 1);
        this->update();
    }
    else if (e.key.type == KeyCode::KEY_HOME)
    {
        this->cursorIndex = 0;
        this->update();
    }
    else if (e.key.type == KeyCode::KEY_END)
    {
        this->cursorIndex = text.length();
        this->update();
    }
    else if (32 <= e.key.type && e.key.type <= 126)
    {
        this->cursorIndex++;

        if (uppercase)
            this->setText(text + ((char)(e.key.type - 32)));
        else
        {
            this->setText(text.substr(0, cursorIndex - 1) + (char)(e.key.type) + text.substr(cursorIndex - 1, text.length()));
        }
    }
}
