#include "interface/controls/labeledgroup.hpp"


void Mineimator::LabeledGroup::update()
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


void Mineimator::LabeledGroup::draw()
{
    for (Control* control : controls) {
        drawTextAligned(control->name + ":", { pos.x, control->pos.y + control->box.height / 2 }, LEFT, MIDDLE, SETTING_INTERFACE_COLOR_TEXT);
        control->draw();
    }
}


void Mineimator::LabeledGroup::mouseEvent()
{
    mouseOn = (parent->mouseOn && mouseInBox(box));

    for (Control* control : controls) {
        control->mouseEvent();
    }
}


void Mineimator::LabeledGroup::keyEvent()
{
    for (Control* control : controls) {
        control->keyEvent();
    }
}


void Mineimator::LabeledGroup::setParent(Element* parent)
{
    this->parent = parent;
    for (Control* control : controls) {
        control->setParent(this);
    }
}