#include "interface/controls/textbox.hpp"
#include "mineimatorapp.hpp"

#include <iostream>


void Mineimator::TextBox::draw()
{
    // Box
    drawBox(box, SETTING_INTERFACE_COLOR_BOXES);
    
    // Text
    
    // Text selection
    if (isFocused())
    {
        drawTextSelected(
            text, box.pos, selectStartCaret.index, selectEndCaret.index,
            SETTING_INTERFACE_COLOR_BOXES_TEXT, SETTING_INTERFACE_COLOR_HIGHLIGHT, SETTING_INTERFACE_COLOR_HIGHLIGHT_TEXT
        );
        drawBox(box, Color(0, 0, 255, 200), true);
        drawLine(box.pos + editCaret.pos, box.pos + editCaret.pos + (ScreenPos){ 0, stringGetHeight(" ") }, SETTING_INTERFACE_COLOR_BOXES_TEXT, 2);
    }
    else {
        drawText(text, box.pos, SETTING_INTERFACE_COLOR_BOXES_TEXT);
    }
}


void Mineimator::TextBox::mouseEvent()
{
    mouseOn = (mouseInBox(box) && app->interfaceHandler->state == IDLE);
    pressed = false;
    
    if (mouseOn)
    {
        if (mouseLeftPressed())
        {
            focus();
            
            app->interfaceHandler->state = TEXTBOX_SELECT;
            
            if (!keyDown(GLFW_KEY_LEFT_SHIFT))
            {
                clickCaret = caretAtPos(mousePos() - box.pos);
                
                // Word select
                if (editCaret.index == clickCaret.index && selectStartCaret.index == selectEndCaret.index && mouseLastClickDuration() < 1.f)
                {
                    selectStartCaret = caretAtIndex(findNextWord(clickCaret.index));
                    selectEndCaret = caretAtIndex(findPreviousWord(clickCaret.index));
                    editCaret = selectEndCaret;
                    app->interfaceHandler->state = IDLE;
                }
            }
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
            mouseClear();
        }
        
        mouseSetCursor(BEAM);
    }
}


void Mineimator::TextBox::keyEvent()
{
    if (!isFocused()) {
        return;
    }
    
    // Move caret
    bool moveRight = (keyPressed(GLFW_KEY_RIGHT) && editCaret.index < text.size());
    bool moveLeft = (keyPressed(GLFW_KEY_LEFT) && editCaret.index > 0);
    bool moveUp = (keyPressed(GLFW_KEY_UP) && editCaret.pos.y > 0);
    bool moveDown = (keyPressed(GLFW_KEY_DOWN) && stringGetCount(text, "\n", editCaret.index) > 0);
    
    if (moveRight || moveLeft || moveUp || moveDown)
    {
        if (moveRight) {
            if (keyDown(GLFW_KEY_LEFT_CONTROL)) {
                editCaret = caretAtIndex(findNextWord(editCaret.index));
            } else {
                editCaret = caretAtIndex(editCaret.index + 1);
            }
        }
        else if (moveLeft) {
            if (keyDown(GLFW_KEY_LEFT_CONTROL)) {
                editCaret = caretAtIndex(findPreviousWord(editCaret.index - 1));
            } else {
                editCaret = caretAtIndex(editCaret.index - 1);
            }
        }
        else if (moveUp) {
            editCaret = caretAtPos({ editCaret.pos.x, editCaret.pos.y - (int)(app->drawingFont->height * LINE_SPACE) });
        }
        else if (moveDown) {
            editCaret = caretAtPos({ editCaret.pos.x, editCaret.pos.y + (int)(app->drawingFont->height * LINE_SPACE) });
        }
        
        if (keyDown(GLFW_KEY_LEFT_SHIFT))
        {
            if (editCaret.index > clickCaret.index) {
                selectStartCaret = clickCaret;
                selectEndCaret = editCaret;
            } else {
                selectEndCaret = clickCaret;
                selectStartCaret = editCaret;
            }
        } else {
            clickCaret = selectStartCaret = selectEndCaret = editCaret;
        }
        return;
    }
    
    // Select all
    if (keyDown(GLFW_KEY_LEFT_CONTROL) && keyPressed(GLFW_KEY_A))
    {
        selectStartCaret = caretAtIndex(0);
        selectEndCaret = caretAtIndex(text.size());
        editCaret = selectEndCaret;
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
        insert = 
            stringReplace(
                glfwGetClipboardString(app->mainWindow->handle),
                "\r\n",
                "\n"
            );
    }
    
    // Copy/Cut
    else if (keyDown(GLFW_KEY_LEFT_CONTROL) && (keyPressed(GLFW_KEY_C) || keyPressed(GLFW_KEY_X)))
    {
        if (selectEndCaret.index != selectStartCaret.index) {
            glfwSetClipboardString(
                app->mainWindow->handle,
                &stringReplace(
                    stringSubstring(text, selectStartCaret.index, selectEndCaret.index - selectStartCaret.index),
                    "\n",
                    "\r\n"
                )[0]
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
        editCaret = clickCaret = selectEndCaret = selectStartCaret;
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


int Mineimator::TextBox::findNextWord(int index)
{
    while (index < text.length())
    {
        char currentChar = text[index];
        if (currentChar == '\n' ||
            currentChar == ' ' ||
            currentChar == '(' ||
            currentChar == ')' ||
            currentChar == '[' ||
            currentChar == ']' ||
            currentChar == '+' ||
            currentChar == '/' ||
            currentChar == '\\')
        {
            if (currentChar == ' ') {
                index++;
            }
            return index;
        }
        index++;
    }
    return index;
}


int Mineimator::TextBox::findPreviousWord(int index)
{
    while (index > 0)
    {
        char currentChar = text[index - 1];
        if (currentChar == '\n' ||
            currentChar == ' ' ||
            currentChar == '(' ||
            currentChar == ')' ||
            currentChar == '[' ||
            currentChar == ']' ||
            currentChar == '+' ||
            currentChar == '/' ||
            currentChar == '\\')
        {
            return index;
        }
        index--;
    }
    return index;
}