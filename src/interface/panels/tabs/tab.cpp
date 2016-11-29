#include "tab.hpp"


void Mineimator::Tab::update()
{
    ScreenPos currentPos = pos;
    
    for (TabSection* section : sections)
    {
        section->box = { currentPos, box.width, 0 };
        section->update();
        currentPos.y += section->box.height + CONTROL_MARGIN_BOTTOM;
    }
}


void Mineimator::Tab::draw()
{
    for (TabSection* section : sections) {
        section->draw();
    }
}


void Mineimator::Tab::setParent(Element* parent)
{
    this->parent = parent;
    for (TabSection* section : sections) {
        section->setParent(this);
    }
}