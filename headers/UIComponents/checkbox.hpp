#pragma once

#include "label.hpp"

class CheckBox : public Label
{
protected:
    int cbX, cbY, cbW, cbH;
    bool checked;

    void drawCheckBox();
    void update() override;
    void setTextAlign(Align a) override;
    void onClick(Event e) override;

public:
    CheckBox(RUIComponent *parent, string text, int width, int height);
    ~CheckBox();

    bool isChecked();
    void setChecked(bool checked);
};
