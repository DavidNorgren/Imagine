#include "interface/controls/checkbox.hpp"


void Mineimator::CheckBox::update()
{
    box.width = 16 + 10 + stringGetWidth(name);
    box.height = 16;
}


void Mineimator::CheckBox::draw()
{
    // Box
    drawImage(IMAGE_CHECKBOX, pos, pressed ? SETTING_INTERFACE_COLOR_BOXES_PRESSED : SETTING_INTERFACE_COLOR_BOXES);
    
    // Check
    if (checked) {
        drawSubImage(IMAGE_ICONS, CHECK, pos + (ScreenPos){8, 8} - (ScreenPos){12, 12}, {0.f});
    }
        
    // Text
    ScreenPos textPos = pos + (ScreenPos){ 20, box.height / 2 };
    drawTextAligned(name, textPos, LEFT, MIDDLE, SETTING_INTERFACE_COLOR_TEXT);
}