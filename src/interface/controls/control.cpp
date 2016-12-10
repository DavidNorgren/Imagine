#include "interface/controls/control.hpp"
#include "mineimatorapp.hpp"


void Mineimator::Control::mouseEvent()
{
    mouseOn = (mouseInBox(box) && app->interfaceHandler->state == IDLE);
    pressed = false;
    
    if (mouseOn)
    {
        if (mouseLeftPressed()) {
            app->interfaceHandler->focus = this;
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