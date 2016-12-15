#include "interface/controls/textbox.hpp"
#include "mineimatorapp.hpp"

#include <iostream>


void Mineimator::TextBox::update()
{
    box.height = stringGetHeight(" ") * lines + TEXTBOX_BOX_PADDING * 2;
    updateWrap();
    editCaret = caretAtIndex(editCaret.index);
    selectStartCaret = caretAtIndex(selectStartCaret.index);
    selectEndCaret = caretAtIndex(selectEndCaret.index);
}


void Mineimator::TextBox::draw()
{
    // Box
    drawBoxEdges(box, SETTING_INTERFACE_COLOR_BOXES, IMAGE_ROUNDED_2);
    
    // Text
    if (isFocused())
    {
        drawTextSelected(
            textWrap, box.pos + (ScreenPos){ TEXTBOX_BOX_PADDING, TEXTBOX_BOX_PADDING }, selectStartCaret.indexWrap, selectEndCaret.indexWrap,
            SETTING_INTERFACE_COLOR_BOXES_TEXT, SETTING_INTERFACE_COLOR_HIGHLIGHT, SETTING_INTERFACE_COLOR_HIGHLIGHT_TEXT
        );
        drawBox(box, Color(0, 0, 255, 200), true);
        drawLine(
            box.pos + (ScreenPos){ TEXTBOX_BOX_PADDING, TEXTBOX_BOX_PADDING } + editCaret.pos,
            box.pos + (ScreenPos){ TEXTBOX_BOX_PADDING, TEXTBOX_BOX_PADDING } + editCaret.pos + (ScreenPos){ 0, stringGetHeight(" ") },
            SETTING_INTERFACE_COLOR_BOXES_TEXT, 2
        );
    }
    else {
        drawText(textWrap, box.pos + (ScreenPos){ TEXTBOX_BOX_PADDING, TEXTBOX_BOX_PADDING }, SETTING_INTERFACE_COLOR_BOXES_TEXT);
    }
}


void Mineimator::TextBox::mouseEvent()
{
    mouseOn = (mouseInBox(box) && isInterfaceState(IDLE));
    pressed = false;
    
    if (mouseOn)
    {
        if (mouseLeftPressed())
        {
            focus();
            setInterfaceState(TEXTBOX_SELECT);
            
            if (!keyDown(GLFW_KEY_LEFT_SHIFT))
            {
                clickCaret = caretAtPos(mousePos() - box.pos);
                
                // Word select
                if (editCaret.indexWrap == clickCaret.indexWrap && selectStartCaret.indexWrap == selectEndCaret.indexWrap && mouseLastClickDuration() < 1.f)
                {
                    selectStartCaret = caretAtIndexWrap(findPreviousWord(clickCaret.indexWrap));
                    selectEndCaret = caretAtIndexWrap(findNextWord(clickCaret.indexWrap));
                    editCaret = selectEndCaret;
                    setInterfaceState(IDLE);
                }
            }
        }
        
        mouseSetCursor(BEAM);
    }
    
    if (isFocused() && isInterfaceState(TEXTBOX_SELECT))
    {
        editCaret = caretAtPos(mousePos() - box.pos);
        
        if (editCaret.indexWrap < clickCaret.indexWrap) {
            selectStartCaret = editCaret;
            selectEndCaret = clickCaret;
        }
        else {
            selectStartCaret = clickCaret;
            selectEndCaret = editCaret;
        }
        
        if (!mouseLeftDown()) {
            setInterfaceState(IDLE);
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
    bool moveRight = (keyPressed(GLFW_KEY_RIGHT) && editCaret.indexWrap < textWrap.size());
    bool moveLeft = (keyPressed(GLFW_KEY_LEFT) && editCaret.indexWrap > 0);
    bool moveUp = (keyPressed(GLFW_KEY_UP) && editCaret.pos.y > 0);
    bool moveDown = (keyPressed(GLFW_KEY_DOWN) && stringGetCount(textWrap, "\n", editCaret.indexWrap) + stringGetCount(textWrap, "\r", editCaret.indexWrap) > 0);
    
    if (moveRight || moveLeft || moveUp || moveDown)
    {
        if (moveRight)
        {
            if (keyDown(GLFW_KEY_LEFT_CONTROL)) {
                editCaret = caretAtIndexWrap(findNextWord(editCaret.indexWrap));
            }
            else {
                editCaret = caretAtIndexWrap(editCaret.indexWrap + 1);
            }
        }
        else if (moveLeft)
        {
            if (keyDown(GLFW_KEY_LEFT_CONTROL)) {
                editCaret = caretAtIndexWrap(findPreviousWord(editCaret.indexWrap - 1));
            }
            else {
                editCaret = caretAtIndexWrap(editCaret.indexWrap - 1);
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
            if (editCaret.indexWrap > clickCaret.indexWrap) {
                selectStartCaret = clickCaret;
                selectEndCaret = editCaret;
            }
            else {
                selectEndCaret = clickCaret;
                selectStartCaret = editCaret;
            }
        }
        else {
            clickCaret = selectStartCaret = selectEndCaret = editCaret;
        }
        return;
    }
    
    // Select all
    if (keyDown(GLFW_KEY_LEFT_CONTROL) && keyPressed(GLFW_KEY_A))
    {
        selectStartCaret = caretAtIndexWrap(0);
        selectEndCaret = caretAtIndexWrap(textWrap.size());
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
    else if (keyDown(GLFW_KEY_LEFT_CONTROL) && keyPressed(GLFW_KEY_V))
    {
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
        if (selectEndCaret.index != selectStartCaret.index)
        {
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
        updateWrap();
        editCaret = selectStartCaret;
    }
    
    // Delete previous character
    else if (keyPressed(GLFW_KEY_BACKSPACE) && editCaret.index > 0)
    {
        text = stringErase(text, editCaret.index - 1, 1);
        updateWrap();
        editCaret = caretAtIndex(editCaret.index - 1);
    }
    
    // Delete next character
    else if (keyPressed(GLFW_KEY_DELETE) && editCaret.index < text.size()) {
        text = stringErase(text, editCaret.index, 1);
        updateWrap();
        editCaret = caretAtIndex(editCaret.index);
    }
    
    // Insert text
    if (insert != "") {
        text = stringInsert(text, insert, editCaret.index);
        updateWrap();
        editCaret = caretAtIndex(editCaret.index + insert.length());
    }
    
    selectStartCaret = selectEndCaret = clickCaret = editCaret;
}


Mineimator::TextBox::Caret Mineimator::TextBox::caretAtPos(ScreenPos pos)
{
    Font* font = app->drawingFont;
    ScreenPos charPos = { 0, 0 };
    Caret caret;
    caret.index = 0;
    caret.indexWrap = 0;
    caret.pos = { 0, 0 };

    for (uint wc = 0, c = 0; wc < textWrap.size(); wc++)
    {
        uchar curChar = textWrap[wc];
        int width = 0;

        if (curChar == '\n' || curChar == '\r') {
            charPos.x = 0;
            charPos.y += font->height * LINE_SPACE;
        }
        else {
            if (curChar < font->start || curChar > font->end) {
                c++;
                continue;
            }
            width = font->chars[curChar].advanceX;
        }

        if (pos.x >= charPos.x + width / 2 && pos.y >= charPos.y) {
            caret.index = c + 1;
            caret.indexWrap = wc + 1;
            caret.pos = { charPos.x + width , charPos.y };
        }

        charPos.x += width;
        
        if (curChar != '\r') {
            c++;
        }
    }
    
    return caret;
}


Mineimator::TextBox::Caret Mineimator::TextBox::caretAtIndex(int index)
{
    Font* font = app->drawingFont;
    Caret caret;
    caret.index = index;
    caret.indexWrap = 0;
    caret.pos = { 0, 0 };

    uint wc, c;
    for (wc = 0, c = 0; wc < textWrap.size(); wc++)
    {
        uchar curChar = textWrap[wc];

        if (curChar != '\r') {
            c++;
            if (c > index) {
                break;
            }
        }
        
        if (curChar == '\n' || curChar == '\r') {
            caret.pos.x = 0;
            caret.pos.y += font->height * LINE_SPACE;
            continue;
        }
        
        if (curChar < font->start || curChar > font->end) {
            continue;
        }
        
        caret.pos.x += font->chars[curChar].advanceX;
    }

    caret.indexWrap = wc;
    
    return caret;
}


Mineimator::TextBox::Caret Mineimator::TextBox::caretAtIndexWrap(int indexWrap)
{
    Font* font = app->drawingFont;
    Caret caret;
    caret.index = 0;
    caret.indexWrap = indexWrap;
    caret.pos = { 0, 0 };

    for (uint wc = 0; wc < indexWrap; wc++)
    {
        uchar curChar = textWrap[wc];

        if (curChar != '\r') {
            caret.index++;
        }
        
        if (curChar == '\n' || curChar == '\r') {
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


bool isWordSeparator(char c)
{
    return (c == '\n' ||
            c == ' ' ||
            c == '(' ||
            c == ')' ||
            c == '[' ||
            c == ']' ||
            c == '+' ||
            c == '/' ||
            c == '\\');
}


int Mineimator::TextBox::findNextWord(int indexWrap)
{
    // Look to the right until a word separating character
    // is found, or the string ends.
    while (indexWrap < textWrap.length())
    {
        if (isWordSeparator(textWrap[indexWrap])) {
            if (textWrap[indexWrap] == ' ') {
                indexWrap++;
            }
            return indexWrap;
        }
        indexWrap++;
    }
    return indexWrap;
}


int Mineimator::TextBox::findPreviousWord(int indexWrap)
{
    // Look to the left until a word separating character
    // is found, or the beginning of the string is reached.
    while (indexWrap > 0)
    {
        if (isWordSeparator(textWrap[indexWrap - 1])) {
            return indexWrap;
        }
        indexWrap--;
    }
    return indexWrap;
}


void Mineimator::TextBox::updateWrap()
{
    // Insert word wrapping symbols between words or characters
    // to keep the text inside the box. They are treated as
    // new lines when rendering, however uses a different value.
    
    Font* font = app->drawingFont;
    int x = 0, sepIndex = -1;
    
    textWrap = text;
    
    for (uint wc = 0; wc < textWrap.length(); wc++)
    {
        uchar curChar = textWrap[wc];

        if (curChar == '\n') {
            x = 0;
            continue;
        }
        
        if (curChar < font->start || curChar > font->end) {
            continue;
        }
        
        x += font->chars[curChar].advanceX;
        
        if (x >= box.width - TEXTBOX_BOX_PADDING * 2)
        {
            if (sepIndex < 0) {
                sepIndex = wc;
            }
            
            textWrap = stringInsert(textWrap, "\r", sepIndex);
            x = stringGetWidth(stringSubstring(textWrap, sepIndex + 1, (wc - sepIndex) + 1));
            
            sepIndex = -1;
            wc++;
        }
        
        if (isWordSeparator(curChar)) {
            sepIndex = wc + 1;
        }
    }
}