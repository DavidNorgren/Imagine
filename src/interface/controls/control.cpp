#include "interface/controls/control.hpp"


void Mineimator::Control::update()
{
    box.height = height;
}


void Mineimator::Control::setParent(Element* parent)
{
    this->parent = parent;
}