#include "tabsection.hpp"


Imagine::TabSection::TabSection(string name, bool open, std::vector<Control*> controls)
{
    this->name = name;
    this->open = open;
    this->openButton = new IconButton(
        name,
        true,
        [this]() { this->open = !this->open; this->parent->update(); },
        NO_ICON,
        16,
        16,
        TRUE_FUNC,
        [this]() { return this->open ? ARROW_DOWN : ARROW_RIGHT; }
    );
    this->controls = controls;
}


void Imagine::TabSection::update()
{
    ScreenPos currentPos = pos;
    openButton->box = { currentPos, box.width, 0 };
    openButton->update();
    currentPos.y += openButton->box.height + CONTROL_MARGIN_BOTTOM;
    box.height += openButton->box.height + CONTROL_MARGIN_BOTTOM;
    
    if (open)
    {
        for (Control* control : controls)
        {
            control->box = { currentPos, box.width, 0 };
            control->update();
            currentPos.y += control->box.height + CONTROL_MARGIN_BOTTOM;
            box.height += control->box.height + CONTROL_MARGIN_BOTTOM;
        }
    }
}


void Imagine::TabSection::draw()
{
    openButton->draw();
    
    if (!open) {
        return;
    }
        
    for (Control* control : controls) {
        control->draw();
    }
}


void Imagine::TabSection::mouseEvent()
{
    openButton->mouseEvent();
    
    if (!open) {
        return;
    }
    
    for (Control* control : controls) {
        control->mouseEvent();
    }
}


void Imagine::TabSection::keyEvent()
{
    openButton->keyEvent();
    
    if (!open) {
        return;
    }
    
    for (Control* control : controls) {
        control->keyEvent();
    }
}


void Imagine::TabSection::setParent(Element* parent)
{
    this->parent = parent;
    openButton->setParent(this);
    for (Control* control : controls) {
        control->setParent(this);
    }
}