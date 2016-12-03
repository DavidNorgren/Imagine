#include "tabsection.hpp"


Mineimator::TabSection::TabSection(string name, bool open, std::vector<Control*> controls)
{
    this->name = name;
    this->open = open;
    this->openButton = new IconButton(
        name,
        true,
        [this](){ this->open = !this->open; this->parent->update();},
        NO_ICON,
        16,
        16,
        TRUE_FUNC,
        [this]() { return this->open ? ARROW_DOWN : ARROW_RIGHT; }
    );
    this->controls = controls;
}


void Mineimator::TabSection::update()
{
    ScreenPos currentPos = pos;
    openButton->box = { currentPos, box.width, 0 };
    openButton->update();
    currentPos.y += openButton->height + CONTROL_MARGIN_BOTTOM;
    box.height += openButton->height + CONTROL_MARGIN_BOTTOM;
    
    if (open)
    {
        for (Control* control : controls)
        {
            control->box = { currentPos, box.width, 0 };
            control->update();
            currentPos.y += control->height + CONTROL_MARGIN_BOTTOM;
            box.height += control->height + CONTROL_MARGIN_BOTTOM;
        }
    }
}


void Mineimator::TabSection::draw()
{
    openButton->draw();
    
    if (!open) {
        return;
    }
        
    for (Control* control : controls) {
        control->draw();
    }
}


void Mineimator::TabSection::mouseEvent()
{
    openButton->mouseEvent();
    
    if (!open) {
        return;
    }
    
    for (Control* control : controls) {
        control->mouseEvent();
    }
}


void Mineimator::TabSection::keyEvent()
{
    openButton->keyEvent();
    
    if (!open) {
        return;
    }
    
    for (Control* control : controls) {
        control->keyEvent();
    }
}


void Mineimator::TabSection::setParent(Element* parent)
{
    this->parent = parent;
    for (Control* control : controls) {
        control->setParent(this);
    }
}