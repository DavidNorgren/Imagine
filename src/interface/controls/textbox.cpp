#include "interface/controls/textbox.hpp"
#include "mineimatorapp.hpp"

#include <iostream>

void Mineimator::TextBox::draw()
{
    drawBox(box, SETTING_INTERFACE_COLOR_BOXES);
    
    drawText(text, box.pos + (ScreenPos){ TEXTBOX_BOX_PADDING, TEXTBOX_BOX_PADDING }, SETTING_INTERFACE_COLOR_BOXES_TEXT);
}


void Mineimator::TextBox::mouseEvent()
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
        }
        mouseSetCursor(BEAM);
    }
}


void Mineimator::TextBox::keyEvent()
{
    if (app->interfaceHandler->focus != this) {
        return;
    }
    
    std::cout << charPressed();
    if (charPressed())
    {
        text += charPressed();
    }
    
    if (keyPressed(GLFW_KEY_ENTER))
    {
        text += "\n";
    }
    
    if (keyPressed(GLFW_KEY_BACKSPACE))
    {
        
    }
}