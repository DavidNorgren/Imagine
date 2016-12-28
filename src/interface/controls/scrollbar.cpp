#include "interface/controls/scrollbar.hpp"
#include "imagineapp.hpp"


void Imagine::ScrollBar::update()
{
    // Determine visibility
    if (visibleSize >= totalSize || totalSize <= 0 || visibleSize < SCROLLBAR_MIN_SIZE) {
        visible = false;
        *offset = 0;
        return;
    }
    else {
        visible = true;
    }

    // Set bar
    float visiblePerc = (float)visibleSize / (float)totalSize;
    float offsetPerc = (float)*offset / (float)totalSize;

    if (direction == VERTICAL) {
        box.width = SCROLLBAR_SIZE;
        bar = {
            pos + (ScreenPos){ 0, (int)(offsetPerc * box.height) },
            box.width,
            max(SCROLLBAR_MIN_SIZE, (int)(visiblePerc * box.height))
        };
        bar.pos.y = min(bar.pos.y, pos.y + box.height - bar.height);
    }
    else if (direction == HORIZONTAL) {
        box.height = SCROLLBAR_SIZE;
        bar = {
            pos + (ScreenPos){ (int)(offsetPerc * box.width), 0 },
            max(SCROLLBAR_MIN_SIZE, (int)(visiblePerc * box.width)),
            box.height
        };
        bar.pos.x = min(bar.pos.x, pos.x + box.width - bar.width);
    }

    // Keep the bar within the box
    *offset = clamp(*offset, 0, totalSize - visibleSize);
}


void Imagine::ScrollBar::draw()
{
    if (!visible) {
        return;
    }

    drawBox(box, SETTING_INTERFACE_COLOR_BACKGROUND);
    drawBox(bar, pressed ? SETTING_INTERFACE_COLOR_BUTTONS_PRESSED : SETTING_INTERFACE_COLOR_BUTTONS);
}


void Imagine::ScrollBar::mouseEvent()
{
    if (!visible) {
        return;
    }

    float visiblePerc = (float)visibleSize / (float)totalSize;

    mouseOn = (parent->mouseOn && mouseInBox(box));
    pressed = false;
    
    if (mouseOn && isInterfaceState(IDLE))
    {
        mouseSetCursor(HANDPOINT);

        // Click bar, start dragging
        if (mouseInBox(bar) && mouseLeftDown())
        {
            pressed = true;
            focus();
            setInterfaceState(SCROLLBAR_MOVE);
            clickOffset = *offset;
        }

        // Jump where clicked and start dragging
        else if (mouseLeftPressed())
        {
            pressed = true;
            focus();
            setInterfaceState(SCROLLBAR_MOVE);

            if (direction == VERTICAL) {
                *offset = (mousePos().y - box.pos.y - bar.height / 2) / visiblePerc;
            } else {
                *offset = (mousePos().x - box.pos.x - bar.width / 2) / visiblePerc;
            }
            *offset = clamp(*offset, 0, totalSize - visibleSize);
            clickOffset = *offset;
            parent->update();
        }
    }

    // Moving
    else if (isFocused() && isInterfaceState(SCROLLBAR_MOVE))
    {
        pressed = true;
        mouseSetCursor(HANDPOINT);

        if (direction == VERTICAL) {
            *offset = clickOffset + (mousePos().y - mousePosClick().y) / visiblePerc;
        } else {
            *offset = clickOffset + (mousePos().x - mousePosClick().x) / visiblePerc;
        }
        *offset = clamp(*offset, 0, totalSize - visibleSize);
        parent->update();

        if (!mouseLeftDown()) {
            setInterfaceState(IDLE);
        }
    }
}


void Imagine::ScrollBar::keyEvent()
{
}