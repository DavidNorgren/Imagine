#include "interface/controls/row.hpp"


void Mineimator::Row::update()
{
    if (controls.size() == 0) {
        return;
    }
    
    int columnWidth = box.width / controls.size();
    if (controls.size() > 1) {
        columnWidth -= CONTROL_MARGIN_RIGHT;
    }
    
    ScreenPos currentPos = pos;
    for (Control* control : controls) {
        control->box = { currentPos, columnWidth, 0 };
        control->update();
        height = max(control->height, height);
        currentPos.x += columnWidth + CONTROL_MARGIN_RIGHT + CONTROL_MARGIN_LEFT;
    }
}


void Mineimator::Row::draw()
{
    for (Control* control : controls) {
        control->draw();
    }
}


void Mineimator::Row::setParent(Element* parent)
{
    this->parent = parent;
    for (Control* control : controls) {
        control->setParent(this);
    }
}