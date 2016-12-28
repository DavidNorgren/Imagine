#include "interface/controls/labeledgroup.hpp"


void Imagine::LabeledGroup::update()
{
    int labelWidth = 0;
    
    for (Control* control : controls) {
        labelWidth = max(labelWidth, stringGetWidth(control->name) + 20);
    }
    
    ScreenPos currentPos = pos;
    box.height = 0;
    
    for (Control* control : controls)
    {
        control->box = { { currentPos.x + labelWidth, currentPos.y }, box.width - labelWidth, 0 };
        control->update();
        currentPos.y += control->box.height + CONTROL_MARGIN_BOTTOM;
        box.height += control->box.height + CONTROL_MARGIN_BOTTOM;
    }
}


void Imagine::LabeledGroup::draw()
{
    for (Control* control : controls) {
        drawTextAligned(control->name + ":", { pos.x, control->pos.y + control->box.height / 2 }, LEFT, MIDDLE, SETTING_INTERFACE_COLOR_TEXT);
        control->draw();
    }
}


void Imagine::LabeledGroup::mouseEvent()
{
    mouseOn = (parent->mouseOn && mouseInBox(box));

    for (Control* control : controls) {
        control->mouseEvent();
    }
}


void Imagine::LabeledGroup::keyEvent()
{
    for (Control* control : controls) {
        control->keyEvent();
    }
}


void Imagine::LabeledGroup::setParent(Element* parent)
{
    this->parent = parent;
    for (Control* control : controls) {
        control->setParent(this);
    }
}