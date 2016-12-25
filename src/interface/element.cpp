#include "interface/element.hpp"
#include "mineimatorapp.hpp"


void Mineimator::Element::setParent(Element* parent)
{
    this->parent = parent;
}


void Mineimator::Element::focus()
{
    app->interfaceHandler->focus = this;
}


bool Mineimator::Element::isFocused()
{
    return (app->interfaceHandler->focus == this);
}