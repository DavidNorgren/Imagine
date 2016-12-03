#include "interface/controls/control.hpp"
#include "mineimatorapp.hpp"


void Mineimator::Control::update()
{
    box.height = height;
}


void Mineimator::Control::mouseEvent()
{
    mouseOn = mouseInBox(box);
    pressed = false;
    
    if (mouseOn)
    {
        if (mouseLeftDown()) {
            app->interfaceHandler->focus = this;
            pressed = true;
        }
        if (mouseLeftReleased() && clickFunc) {
            clickFunc();
            //mouseClear();
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