#include "interface/controls/button.hpp"


void Mineimator::Button::update()
{
    box.height = height;
}


void Mineimator::Button::draw()
{
    ScreenPos contentPos = pos;
    if (pressed) {
        contentPos += (ScreenPos){ 0, 1 };
    }
    
    // Box
    drawBoxEdges(box, pressed ? SETTING_INTERFACE_COLOR_BUTTONS_PRESSED : SETTING_INTERFACE_COLOR_BUTTONS, IMAGE_ROUNDED_4);
    
    // Icon
    if (icon != NO_ICON)
    {
        ScreenPos iconPos = contentPos + (ScreenPos){ box.width / 2 - 12, box.height / 2 - 12 };
        iconPos.x -= stringGetWidth(name) / 2;
        drawSubImage(IMAGE_ICONS, icon, iconPos, SETTING_INTERFACE_COLOR_BUTTONS_TEXT);
    }
    
    // Text
    ScreenPos textPos = contentPos + (ScreenPos){ box.width / 2, box.height / 2 };
    if (icon != NO_ICON) {
        textPos.x += 14;
    }
    drawTextAligned(name, textPos, CENTER, MIDDLE, SETTING_INTERFACE_COLOR_BUTTONS_TEXT);
}


void Mineimator::IconButton::update()
{
    box.width = width;
    if (showText) {
        box.width += 10 + stringGetWidth(name);
    }
    box.height = height;
}


void Mineimator::IconButton::draw()
{
    ScreenArea iconBox = { pos, width, height };
    ScreenPos contentPos = pos;
    if (pressed) {
        contentPos += (ScreenPos) { 0, 1 };
    }
    
    // Box
    if (mouseOn) {
        drawBoxEdges(iconBox, pressed ? SETTING_INTERFACE_COLOR_BUTTONS_PRESSED : SETTING_INTERFACE_COLOR_BUTTONS, IMAGE_ROUNDED_4);
    }
    
    // Icon
    if (iconFunc) {
        icon = iconFunc();
    }
    
    if (icon != NO_ICON) {
        ScreenPos iconPos = contentPos + (ScreenPos){ iconBox.width / 2 - 12, iconBox.height / 2 - 12 };
        drawSubImage(IMAGE_ICONS, icon, iconPos, mouseOn ? SETTING_INTERFACE_COLOR_BUTTONS_TEXT : SETTING_INTERFACE_COLOR_TEXT);
    }
    
    // Text
    if (showText) {
        ScreenPos textPos = pos + (ScreenPos){ iconBox.width + 5, iconBox.height / 2 };
        drawTextAligned(name, textPos, LEFT, MIDDLE, SETTING_INTERFACE_COLOR_TEXT);
    }
}