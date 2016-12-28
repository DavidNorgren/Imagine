#include "interface/controls/row.hpp"


void Imagine::Row::update()
{
    if (controls.size() == 0) {
        return;
    }
    
    int columnWidth = box.width / controls.size();
    if (controls.size() > 1) {
        columnWidth -= CONTROL_MARGIN_RIGHT;
    }
    
    ScreenPos currentPos = pos;
    for (Control* control : controls)
    {
        control->box = { currentPos, columnWidth, 0 };
        control->update();
        control->box.width = min(control->box.width, columnWidth);
        box.height = max(box.height, control->box.height);
        currentPos.x += columnWidth + CONTROL_MARGIN_RIGHT + CONTROL_MARGIN_LEFT;
    }
}


void Imagine::Row::draw()
{
    for (Control* control : controls) {
        control->draw();
    }
}


void Imagine::Row::mouseEvent()
{
    mouseOn = (parent->mouseOn && mouseInBox(box));

    for (Control* control : controls) {
        control->mouseEvent();
    }
}


void Imagine::Row::keyEvent()
{
    for (Control* control : controls) {
        control->keyEvent();
    }
}


void Imagine::Row::setParent(Element* parent)
{
    this->parent = parent;
    for (Control* control : controls) {
        control->setParent(this);
    }
}