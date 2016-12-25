#include "interface/controls/textfield.hpp"
#include "mineimatorapp.hpp"

#include <iostream>


Mineimator::TextField::TextField(string name, int lines, std::function<bool()> visibleFunc)
{
    this->name = name;
    this->lines = lines;
    this->visibleFunc = visibleFunc;
    editCaret.index = selectStartCaret.index = selectEndCaret.index = 0;
    if (lines > 1) {
        scrollBar = new ScrollBar(ScrollBar::VERTICAL, &textOffset.y);
    }
}


void Mineimator::TextField::update()
{
    /* Define text area */
    box.height = stringGetHeight(" ") * lines + TEXTBOX_BOX_PADDING * 2;
    textBox = {
        pos + (ScreenPos){ TEXTBOX_BOX_PADDING, TEXTBOX_BOX_PADDING },
        box.width - TEXTBOX_BOX_PADDING * 2,
        box.height - TEXTBOX_BOX_PADDING * 2
    };

    /* Update scrollbar */
    if (scrollBar) {
        scrollBar->pos = pos + (ScreenPos){ box.width - SCROLLBAR_SIZE, 0 };
        scrollBar->box.height = box.height;
        scrollBar->visibleSize = textBox.height;
        scrollBar->totalSize = stringGetHeight(textWrap);
        scrollBar->update();
        if (scrollBar->visible) {
            textBox.width -= SCROLLBAR_SIZE;
        }
    }

    /* Update wrapping and carets */
    updateWrap();
    editCaret = caretAtIndex(editCaret.index);
    selectStartCaret = caretAtIndex(selectStartCaret.index);
    selectEndCaret = caretAtIndex(selectEndCaret.index);
    if (lines == 1) {
        textOffset.x = clamp(textOffset.x, 0, stringGetWidth(textWrap) - textBox.width);
    }
}


void Mineimator::TextField::draw()
{
    ScreenArea drawingArea = getDrawingArea();

    // Box
    drawBoxEdges(box, SETTING_INTERFACE_COLOR_BOXES, IMAGE_ROUNDED_2);
    
    // Text
    setDrawingArea(ScreenArea::intersection(drawingArea, textBox));
    if (isFocused())
    {
        drawTextSelected(
            textWrap, textBox.pos - textOffset, selectStartCaret.indexWrap, selectEndCaret.indexWrap,
            SETTING_INTERFACE_COLOR_BOXES_TEXT, SETTING_INTERFACE_COLOR_HIGHLIGHT, SETTING_INTERFACE_COLOR_HIGHLIGHT_TEXT
        );
        drawBox(box, Color(0, 0, 255, 200), true);

        // Blinking caret
        if (fmodf(glfwGetTime() - clickTime, 1.f) < 0.5f)
        {
            setDrawingArea(ScreenArea::intersection(drawingArea, box));
            drawLine(
                textBox.pos - textOffset + editCaret.pos,
                textBox.pos - textOffset + editCaret.pos + (ScreenPos){ 0, stringGetHeight(" ") },
                SETTING_INTERFACE_COLOR_BOXES_TEXT, 2
            );
        }
    }
    else {
        drawText(textWrap, textBox.pos - textOffset, SETTING_INTERFACE_COLOR_BOXES_TEXT);
    }

    setDrawingArea(drawingArea);
    
    if (scrollBar) {
        scrollBar->draw();
    }
}


void Mineimator::TextField::mouseEvent()
{
    mouseOn = (parent->mouseOn && mouseInBox(box));

    if (scrollBar) {
        scrollBar->mouseEvent();
        if (scrollBar->mouseOn) {
            mouseOn = false;
        }
    }
    
    if (mouseOn)
    {
        if (mouseLeftPressed())
        {
            clickTime = glfwGetTime();
            focus();
            setInterfaceState(TEXTBOX_SELECT);
            
            if (!keyDown(GLFW_KEY_LEFT_SHIFT))
            {
                clickCaret = caretAtPos(mousePos() - box.pos + textOffset);
                
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
        clickTime = glfwGetTime();
        editCaret = caretAtPos(mousePos() - box.pos + textOffset);
        updateOffset();
        
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


void Mineimator::TextField::keyEvent()
{
    if (!isFocused()) {
        return;
    }

    clickTime = glfwGetTime();
    
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

        updateOffset();
        return;
    }
    
    // Select all
    if (keyDown(GLFW_KEY_LEFT_CONTROL) && keyPressed(GLFW_KEY_A))
    {
        selectStartCaret = caretAtIndexWrap(0);
        selectEndCaret = caretAtIndexWrap(textWrap.size());
        editCaret = selectEndCaret;
        updateOffset();
        return;
    }
    
    string insert = "";
    
    // Type character
    if (charPressed()) {
        insert = string(1, charPressed());
    }
    
    // Linebreak
    else if (keyPressed(GLFW_KEY_ENTER) && lines > 1) {
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
        if (lines == 1) {
            insert = stringReplace(insert, "\n", " ");
        }
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
    if (scrollBar) {
        scrollBar->totalSize = stringGetHeight(textWrap);
    }

    updateOffset();
}


void Mineimator::TextField::setParent(Element* parent)
{
    this->parent = parent;
    if (scrollBar) {
        scrollBar->setParent(this);
    }
}


Mineimator::TextField::Caret Mineimator::TextField::caretAtPos(ScreenPos pos)
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


Mineimator::TextField::Caret Mineimator::TextField::caretAtIndex(int index)
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


Mineimator::TextField::Caret Mineimator::TextField::caretAtIndexWrap(int indexWrap)
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


int Mineimator::TextField::findNextWord(int indexWrap)
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


int Mineimator::TextField::findPreviousWord(int indexWrap)
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


void Mineimator::TextField::updateWrap()
{
    Font* font = app->drawingFont;
    int x = 0, sepIndex = -1;
    
    textWrap = text;

    if (lines == 1) {
        return;
    }

    // Insert word wrapping symbols between words or characters
    // to keep the text inside the box. They are treated as
    // new lines when rendering, however uses a different value.
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
        
        if (x >= textBox.width)
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


void Mineimator::TextField::updateOffset()
{
    // Moves the view to the editing marker

    if (lines > 1)
    {
        if (editCaret.pos.y < textOffset.y) {
            textOffset.y = editCaret.pos.y;
        }
        if (editCaret.pos.y + stringGetHeight(" ") >= textOffset.y + textBox.height) {
            textOffset.y = editCaret.pos.y - textBox.height + stringGetHeight(" ");
        }
        textOffset.y = clamp(textOffset.y, 0, stringGetHeight(textWrap) - textBox.height);
    }
    else
    {
        if (editCaret.pos.x < textOffset.x) {
            textOffset.x = editCaret.pos.x;
        }
        if (editCaret.pos.x >= textOffset.x + textBox.width) {
            textOffset.x = editCaret.pos.x - textBox.width;
        }
        textOffset.x = clamp(textOffset.x, 0, stringGetWidth(textWrap) - textBox.width);
    }

    if (scrollBar) {
        scrollBar->update();
    }
}