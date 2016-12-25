#include "interface/controls/scrollbar.hpp"
#include "mineimatorapp.hpp"


void Mineimator::ScrollBar::update()
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

    /* Set bar */
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

    *offset = clamp(*offset, 0, totalSize - visibleSize);
}


void Mineimator::ScrollBar::draw()
{
    if (!visible) {
        return;
    }

    drawBox(box, SETTING_INTERFACE_COLOR_BACKGROUND);
    drawBox(bar, pressed ? SETTING_INTERFACE_COLOR_BUTTONS_PRESSED : SETTING_INTERFACE_COLOR_BUTTONS);
}


void Mineimator::ScrollBar::mouseEvent()
{
    if (!visible) {
        return;
    }

    mouseOn = (parent->mouseOn && mouseInBox(box));
    pressed = false;

    
    if (mouseOn && isInterfaceState(IDLE))
    {
        mouseSetCursor(HANDPOINT);

        if (mouseInBox(bar) && mouseLeftDown())
        {
            pressed = true;
            focus();
            setInterfaceState(SCROLLBAR_MOVE);
            clickOffset = *offset;
        }
        else if (mouseLeftPressed())
        {
            focus();
            if (direction == VERTICAL)
            {
                if (mousePos().y < bar.pos.y) {
                    *offset -= visibleSize;
                } else {
                    *offset += visibleSize;
                }
            }
            else
            {
                if (mousePos().x < bar.pos.x) {
                    *offset -= visibleSize;
                } else {
                    *offset += visibleSize;
                }
            }
            *offset = clamp(*offset, 0, totalSize - visibleSize);
            parent->update();
        }
    }
    else if (isFocused() && isInterfaceState(SCROLLBAR_MOVE))
    {
        pressed = true;
        mouseSetCursor(HANDPOINT);

        if (direction == VERTICAL) {
            *offset = clickOffset + (mousePos().y - mousePosClick().y) * ((float)totalSize / (float)visibleSize);
        } else {
            *offset = clickOffset + (mousePos().x - mousePosClick().x) * ((float)totalSize / (float)visibleSize);
        }
        *offset = clamp(*offset, 0, totalSize - visibleSize);
        parent->update();

        if (!mouseLeftDown()) {
            setInterfaceState(IDLE);
        }
    }
}


void Mineimator::ScrollBar::keyEvent()
{
}