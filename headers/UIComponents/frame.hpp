#pragma once

#include "../../headers/reluicomp.hpp"

class Frame : public RelativeUIComponent
{
protected:
    Color bg;
    vector<RelativeUIComponent*> members;

public:
    Frame(RelativeUIComponent *parent, int width, int height);
    ~Frame();

    Color getBg();
    RelativeUIComponent *getMemberAt(int index);
    int getSizeOfMembers();

    void setBg(Color bg);
    void setPosition(int x, int y) override;
    
    void addComponent(RelativeUIComponent *component);
    void delComponent(RelativeUIComponent *component);
};
