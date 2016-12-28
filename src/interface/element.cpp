#include "imagineapp.hpp"


void Imagine::Element::setParent(Element* parent)
{
    this->parent = parent;
}


void Imagine::Element::focus()
{
    app->interfaceHandler->focus = this;
}


bool Imagine::Element::isFocused()
{
    return (app->interfaceHandler->focus == this);
}