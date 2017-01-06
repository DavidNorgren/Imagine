#include "imagineapp.hpp"


void Imagine::Element::setParent(Element* parent)
{
    this->parent = parent;
}


void Imagine::Element::focus()
{
    interface->focus = this;
}


bool Imagine::Element::isFocused()
{
    return (interface->focus == this);
}