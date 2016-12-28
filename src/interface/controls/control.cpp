#include "interface/controls/control.hpp"


void Imagine::Control::mouseEvent()
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


void Imagine::Control::keyEvent()
{
    
}