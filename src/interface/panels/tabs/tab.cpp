#include "interface/interfacehandler.hpp"
#include "tab.hpp"


void Imagine::Tab::update()
{
    // Define section area
    sectionsBox = {
        { pos.x + TAB_CONTENT_PADDING, pos.y + TAB_CONTENT_PADDING },
        box.width - TAB_CONTENT_PADDING * 2, box.height - TAB_CONTENT_PADDING * 2
    };

    // Update scrollbar
    scrollBar->pos = sectionsBox.pos + (ScreenPos) { sectionsBox.width - SCROLLBAR_SIZE, 0 };
    scrollBar->box.height = sectionsBox.height;
    scrollBar->visibleSize = sectionsBox.height;
    scrollBar->totalSize = sectionsSize;
    scrollBar->update();
    if (scrollBar->visible) {
        sectionsBox.width -= SCROLLBAR_SIZE + TAB_CONTENT_PADDING;
    }
    
    // Update sections
    ScreenPos currentPos = sectionsBox.pos - sectionsOffset;
    sectionsSize = 0;

    // Split into columns
    int columns = clamp(sectionsBox.width / PANEL_START_WIDTH, 1, (int)sections.size());
    int columnWidth = sectionsBox.width / columns - TAB_CONTENT_PADDING * (columns - 1);
    int columnSection = 0;
    int column = 0;

    for (TabSection* section : sections)
    {
        section->box = { currentPos, columnWidth, 0 };
        section->update();
        currentPos.y += section->box.height;
        sectionsSize = max(sectionsSize, currentPos.y - (sectionsBox.pos.y - sectionsOffset.y));

        // Next column
        columnSection++;
        if (columnSection >= sections.size() / columns && column != columns - 1)
        {
            currentPos.x += columnWidth + TAB_CONTENT_PADDING * 2;
            currentPos.y = sectionsBox.pos.y - sectionsOffset.y;
            columnSection = 0;
            column++;
        }
    }
}


void Imagine::Tab::draw()
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

    // Scrollbar
    scrollBar->draw();

    // Reset alpha
    if (isFocused() && isInterfaceState(TAB_MOVE)) {
        setDrawingAlpha(1.f);
    }
}


void Imagine::Tab::mouseEvent()
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

        // Scrollbar
        scrollBar->mouseEvent();
    }
}


void Imagine::Tab::keyEvent()
{
    for (TabSection* section : sections) {
        section->keyEvent();
    }
}


void Imagine::Tab::setParent(Element* parent)
{
    this->parent = parent;
    for (TabSection* section : sections) {
        section->setParent(this);
    }
    scrollBar->setParent(this);
}