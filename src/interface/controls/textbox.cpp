#include "interface/controls/textbox.hpp"
#include "mineimatorapp.hpp"

#include <iostream>


void Mineimator::TextBox::draw()
{
    drawBox(box, SETTING_INTERFACE_COLOR_BOXES);
    if (isFocused()) {
        drawBox(box, Color(0, 0, 255, 200), true);
        drawLine(box.pos + caretStartPos, box.pos + caretStartPos + (ScreenPos){ 0, stringGetHeight(" ") }, SETTING_INTERFACE_COLOR_BOXES_TEXT, 2);
    }
    
    drawText(text, box.pos, SETTING_INTERFACE_COLOR_BOXES_TEXT);
}


void Mineimator::TextBox::mouseEvent()
{
    Font* font = app->drawingFont;
    mouseOn = mouseInBox(box);
    pressed = false;
    
    if (mouseOn)
    {
        if (mouseLeftPressed())
        {
            focus();
            pressed = true;
            
            ScreenPos charPos = { 0, 0 };
            caretStartIndex = 0;
            caretStartPos = charPos;
            
            for (uint c = 0; c < text.size(); c++)
            {
                uchar curChar = text[c];

                if (curChar < font->start || curChar > font->end) {
                    continue;
                }
                
                CharInfo curCharInfo = font->chars[curChar];

                if (mousePos().x - box.pos.x > charPos.x + curCharInfo.advanceX / 2 &&
                    mousePos().y - box.pos.y > charPos.y) {
                    caretStartIndex = c + 1;
                    caretStartPos = { charPos.x + (int)curCharInfo.advanceX, charPos.y };
                }

                if (curChar == '\n') {
                    charPos.x = 0;
                    charPos.y += height * LINE_SPACE;
                    continue;
                }

                charPos += { (int)curCharInfo.advanceX, (int)curCharInfo.advanceY };
            }
        }
        mouseSetCursor(BEAM);
    }
    
    if (isFocused() && app->interfaceHandler->state == TEXTBOX_SELECT)
    {
        
    }
}


void Mineimator::TextBox::keyEvent()
{
    if (!isFocused()) {
        return;
    }
    
    if (charPressed())
    {
        text = stringInsert(text, string(1, charPressed()), caretStartIndex);
        caretStartIndex++;
        caretStartPos.x += stringGetWidth(string(1, charPressed()));
    }
    
    if (keyPressed(GLFW_KEY_ENTER))
    {
        text += "\n";
    }
    
    if (keyPressed(GLFW_KEY_BACKSPACE) && caretStartIndex > 0)
    {
        caretStartIndex--;
        caretStartPos.x -= stringGetWidth(stringSubstring(text, caretStartIndex, 1));
        text = stringErase(text, caretStartIndex, 1);
    }
}