#include "interface/controls/textbox.hpp"
#include "mineimatorapp.hpp"

#include <iostream>


void Mineimator::TextBox::draw()
{
    drawBox(box, SETTING_INTERFACE_COLOR_BOXES);
    if (isFocused()) {
        drawBox(box, Color(0, 0, 255, 200), true);
        drawLine(box.pos + editCaret.pos, box.pos + editCaret.pos + (ScreenPos){ 0, stringGetHeight(" ") }, SETTING_INTERFACE_COLOR_BOXES_TEXT, 2);
        drawLine(box.pos + selectStartCaret.pos, box.pos + selectStartCaret.pos + (ScreenPos){ 0, stringGetHeight(" ") }, COLOR_RED, 2);
        drawLine(box.pos + selectEndCaret.pos, box.pos + selectEndCaret.pos + (ScreenPos){ 0, stringGetHeight(" ") }, COLOR_BLUE, 2);
    }
    
    drawText(text, box.pos, SETTING_INTERFACE_COLOR_BOXES_TEXT);
}


void Mineimator::TextBox::mouseEvent()
{
    mouseOn = mouseInBox(box);
    pressed = false;
    
    if (mouseOn)
    {
        if (mouseLeftPressed())
        {
            focus();
            pressed = true;
            clickCaret = editCaret = caretAtPos(mousePos() - box.pos);
            app->interfaceHandler->state = TEXTBOX_SELECT;
        }
        mouseSetCursor(BEAM);
    }
    
    if (isFocused() && app->interfaceHandler->state == TEXTBOX_SELECT)
    {
        editCaret = caretAtPos(mousePos() - box.pos);
        
        if (editCaret.index < clickCaret.index) {
            selectStartCaret = editCaret;
            selectEndCaret = clickCaret;
        }
        else {
            selectStartCaret = clickCaret;
            selectEndCaret = editCaret;
        }
        
        if (!mouseLeftDown()) {
            app->interfaceHandler->state = IDLE;
        }
    }
}


void Mineimator::TextBox::keyEvent()
{
    if (!isFocused()) {
        return;
    }
    
    // Move caret right
    if (keyPressed(GLFW_KEY_RIGHT) && editCaret.index < text.size()) {
        editCaret = caretAtIndex(editCaret.index + 1);
        selectStartCaret = selectEndCaret = editCaret;
        return;
    }
    
    // Move caret left
    if (keyPressed(GLFW_KEY_LEFT) && editCaret.index > 0) {
        editCaret = caretAtIndex(editCaret.index - 1);
        selectStartCaret = selectEndCaret = editCaret;
        return;
    }
    
    string insert = "";
    
    // Type character
    if (charPressed()) {
        insert = string(1, charPressed());
    }
    
    // Linebreak
    else if (keyPressed(GLFW_KEY_ENTER)) {
        insert = "\n";
    }
    
    // Paste
    else if (keyDown(GLFW_KEY_LEFT_CONTROL) && keyPressed(GLFW_KEY_V)) {
        insert = glfwGetClipboardString(app->mainWindow->handle);
    }
    
    // Copy/Cut
    else if (keyDown(GLFW_KEY_LEFT_CONTROL) && (keyPressed(GLFW_KEY_C) || keyPressed(GLFW_KEY_X)))
    {
        if (selectEndCaret.index != selectStartCaret.index) {
            glfwSetClipboardString(
                app->mainWindow->handle,
                &stringSubstring(text, selectStartCaret.index, selectEndCaret.index - selectStartCaret.index)[0]
            );
        }
        if (keyPressed(GLFW_KEY_C)) {
            return;
        }
    }
    
    // No action
    else if (!keyPressed(GLFW_KEY_BACKSPACE) && !keyPressed(GLFW_KEY_DELETE)) {
        return;
    }
    
    // Delete selection
    if (selectEndCaret.index != selectStartCaret.index)
    {
        text = stringErase(text, selectStartCaret.index, selectEndCaret.index - selectStartCaret.index);
        editCaret = selectStartCaret;
    }
    
    // Delete previous character
    else if (keyPressed(GLFW_KEY_BACKSPACE) && editCaret.index > 0)
    {
        text = stringErase(text, editCaret.index - 1, 1);
        editCaret = caretAtIndex(editCaret.index - 1);
    }
    
    // Delete next character
    else if (keyPressed(GLFW_KEY_DELETE) && editCaret.index < text.size()) {
        text = stringErase(text, editCaret.index, 1);
    }
    
    // Insert text
    if (insert != "") {
        text = stringInsert(text, insert, editCaret.index);
        editCaret = caretAtIndex(editCaret.index + insert.length());
    }
    
    selectStartCaret = selectEndCaret = editCaret;
}


Mineimator::TextBox::Caret Mineimator::TextBox::caretAtPos(ScreenPos pos)
{
    Font* font = app->drawingFont;
    ScreenPos charPos = { 0, 0 };
    Caret caret;
    caret.index = 0;
    caret.pos = { 0, 0 };

    for (uint c = 0; c < text.size(); c++)
    {
        uchar curChar = text[c];
        int width = 0;

        if (curChar == '\n') {
            charPos.x = 0;
            charPos.y += font->height * LINE_SPACE;
        }
        else {
            if (curChar < font->start || curChar > font->end) {
                continue;
            }
            width = font->chars[curChar].advanceX;
        }

        if (pos.x >= charPos.x + width / 2 && pos.y >= charPos.y) {
            caret.index = c + 1;
            caret.pos = { charPos.x + width , charPos.y };
        }

        charPos.x += width;
    }
    
    return caret;
}


Mineimator::TextBox::Caret Mineimator::TextBox::caretAtIndex(int index)
{
    Font* font = app->drawingFont;
    Caret caret;
    caret.index = index;
    caret.pos = { 0, 0 };

    for (uint c = 0; c < index; c++)
    {
        uchar curChar = text[c];

        if (curChar == '\n') {
            caret.pos.x = 0;
            caret.pos.y += font->height * LINE_SPACE;
            continue;
        }
        
        if (curChar < font->start || curChar > font->end) {
            continue;
        }
        
        caret.pos.x += font->chars[curChar].advanceX;
    }
    
    return caret;
}