#include "interface/interfacehandler.hpp"
#include "tab.hpp"


void Mineimator::Tab::update()
{
    // Define section area
    sectionsBox = {
        { pos.x + TAB_CONTENT_PADDING, pos.y + TAB_CONTENT_PADDING },
        box.width - TAB_CONTENT_PADDING * 2, box.height - TAB_CONTENT_PADDING * 2
    };
    ScreenPos currentPos = sectionsBox.pos;
    
    // Update sections
    for (TabSection* section : sections)
    {
        section->box = { currentPos, box.width - TAB_CONTENT_PADDING * 2, 0 };
        section->update();
        currentPos.y += section->box.height;
    }
}


void Mineimator::Tab::draw()
{
    // Transparent if moved
    if (isFocused() && isInterfaceState(TAB_MOVE)) {
        setDrawingAlpha(0.75f);
    }

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
    setDrawingArea(sectionsBox);
    for (TabSection* section : sections) {
        section->draw();
    }
    resetDrawingArea();

    // Reset alpha
    if (isFocused() && isInterfaceState(TAB_MOVE)) {
        setDrawingAlpha(1.f);
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
        if (ScreenPos::distance(mousePosClick(), mousePos()) > 10)
        {
            setInterfaceState(TAB_MOVE);
            moveStartPos = pos;
            moveSelectStartPos = selectBox.pos;
            ((Panel*)parent)->removeTab(this);
            parent->parent->update();
        }

        // Cursor was released, return to idle state
        if (!mouseLeftDown()) {
            setInterfaceState(IDLE);
        }
    }
    else
    {
        // Click title
        if (isInterfaceState(IDLE) && mouseInBox(selectBox) && mouseLeftPressed()) {
            setInterfaceState(TAB_CLICK);
            focus();
        }

        // Proceed to sections
        for (TabSection* section : sections) {
            section->mouseOn = (mouseOn && mouseInBox(sectionsBox));
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