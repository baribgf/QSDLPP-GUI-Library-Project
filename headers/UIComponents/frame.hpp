#pragma once

#include "../../headers/reluicomp.hpp"

class Frame : public RUIComponent
{
protected:
    Color bg;
    vector<RUIComponent*> members;

    bool insideBounds(int x, int y) override;

public:
    Frame(RUIComponent *parent, int width, int height);
    ~Frame();

    Color getBg();
    RUIComponent *getMemberAt(int index);
    int getSizeOfMembers();

    void setBg(Color bg);
    void setPosition(int x, int y) override;
    
    void add(RUIComponent *component);
    void del(RUIComponent *component);
};
