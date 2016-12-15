#include "interface/interfacehandler.hpp"
#include "tab.hpp"


void Mineimator::Tab::update()
{
    ScreenPos currentPos = { pos.x + TAB_CONTENT_PADDING, pos.y + TAB_CONTENT_PADDING };
    
    for (TabSection* section : sections)
    {
        section->box = { currentPos, box.width - TAB_CONTENT_PADDING * 2, 0 };
        section->update();
        currentPos.y += section->box.height;
    }
}


void Mineimator::Tab::draw()
{
    // Background
    drawBox(box, SETTING_INTERFACE_COLOR_MAIN);
    drawBox(selectBox, SETTING_INTERFACE_COLOR_MAIN);

    // Title
    drawTextAligned(name,
                    selectBox.pos + (ScreenPos){ selectBox.width / 2, selectBox.height / 2 },
                    CENTER, MIDDLE,
                    SETTING_INTERFACE_COLOR_TEXT,
                    BOLD);

    // Sections
    for (TabSection* section : sections) {
        section->draw();
    }
}


void Mineimator::Tab::mouseEvent()
{
    // This tab is currently being moved
    if (isFocused() && isInterfaceState(TAB_MOVE))
    {
        // Offset the boxes using the mouse current and start position
        pos = moveStartPos + (mousePos() - mousePosClick());
        selectBox.pos = moveSelectStartPos + (mousePos() - mousePosClick());
        update();
    }

    // The tab title has been clicked and the mouse is held down
    if (isFocused() && isInterfaceState(TAB_CLICK))
    {
        // Cursor was moved while held down, start moving
        if (ScreenPos::distance(mousePosClick(), mousePos()) > 10) {
            setInterfaceState(TAB_MOVE);
            moveStartPos = pos;
            moveSelectStartPos = selectBox.pos;
            ((Panel*)parent)->removeTab(this);
            parent->update();
        }

        // Cursor was released, return to idle state
        if (!mouseLeftDown()) {
            setInterfaceState(IDLE);
        }
    }
    else 
    {
        // Click title
        if (mouseInBox(selectBox) && mouseLeftPressed()) {
            setInterfaceState(TAB_CLICK);
            focus();
        }

        // Proceed to sections
        for (TabSection* section : sections) {
            section->mouseEvent();
        }
    }
}


void Mineimator::Tab::keyEvent()
{
    for (TabSection* section : sections) {
        section->keyEvent();
    }
}


void Mineimator::Tab::setParent(Element* parent)
{
    this->parent = parent;
    for (TabSection* section : sections) {
        section->setParent(this);
    }
}