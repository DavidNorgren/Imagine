#include "interface/controls/control.hpp"
#include "interface/interfacehandler.hpp"


void Mineimator::Control::mouseEvent()
{
    mouseOn = (parent->mouseOn && mouseInBox(box));
    pressed = false;
    
    if (mouseOn)
    {
        if (mouseLeftPressed()) {
            focus();
        }
        if (mouseLeftDown()) {
            pressed = true;
        }
        if (mouseLeftReleased() && clickFunc) {
            clickFunc();
            mouseClear();
        }
        mouseSetCursor(HANDPOINT);
    }
}


void Mineimator::Control::keyEvent()
{
    
}


void Mineimator::Control::setParent(Element* parent)
{
    this->parent = parent;
}