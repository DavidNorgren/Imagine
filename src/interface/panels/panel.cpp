#include "interface/interfacehandler.hpp"
#include "mineimatorapp.hpp"


void Mineimator::Panel::update()
{
    if (!selectedTab) {
        return;
    }

    // Get total width of every tab
    int selectTotalWidth = 0;
    for (Tab* tab : tabs)
    {
        // The selected tab is displayed with bold text
        if (selectedTab == tab) {
            tab->selectBox.width = app->drawingFontBold->stringGetWidth(tab->name) + 20;
        } else {
            tab->selectBox.width = stringGetWidth(tab->name) + 20;
        }
        tab->selectBox.height = TAB_SELECT_HEIGHT;
        selectTotalWidth += tab->selectBox.width;
    }
    
    // Set the positions of each tab selector, and
    // shrink them if necessary to fit everything
    ScreenPos selectPos = pos;
    for (Tab* tab : tabs)
    {
        if (selectedTab != tab) {
            tab->selectBox.width -= max(0, selectTotalWidth - box.width) / (tabs.size() - 1);
        }
        tab->selectBox.pos = selectPos;
        selectPos.x += tab->selectBox.width;
    }

    // Define the content box and update the selected tab
    selectedTab->box = {
        { pos.x + TAB_CONTENT_PADDING, pos.y + TAB_SELECT_HEIGHT + TAB_CONTENT_PADDING }, 
        box.width  - TAB_CONTENT_PADDING * 2,
        box.height - TAB_SELECT_HEIGHT - TAB_CONTENT_PADDING * 2 
    };
    selectedTab->update();
}


void Mineimator::Panel::draw()
{
    // Continue only if there is an active tab in the panel
    if (!selectedTab) {
        return;
    }
    
    // Draw the background of the panel
    drawBox(box, SETTING_INTERFACE_COLOR_MAIN);
    
    // Tab selector
    drawBox({ box.pos, box.width, TAB_SELECT_HEIGHT }, SETTING_INTERFACE_COLOR_BACKGROUND);
    for (Tab* tab : tabs)
    {
        if (selectedTab == tab) {
            drawBox(tab->selectBox, SETTING_INTERFACE_COLOR_MAIN);
        }
        drawTextAligned(tab->name,
                        tab->selectBox.pos + (ScreenPos){ tab->selectBox.width / 2, tab->selectBox.height / 2 },
                        CENTER, MIDDLE,
                        SETTING_INTERFACE_COLOR_TEXT,
                        (selectedTab == tab ? app->drawingFontBold : app->drawingFont ));
    }
    
    // Draw current tab contents
    selectedTab->draw();
}


void Mineimator::Panel::mouseEvent()
{
    if (!selectedTab) {
        return;
    }

    if (getInterfaceState() == IDLE)
    {
        if (mouseInBox(resizeBox))
        {
            // The mouse is near the edge for resizing
            mouseSetCursor((location == BOTTOM || location == TOP) ? NSRESIZE : WERESIZE);
            if (mouseLeftPressed())
            {
                focus();
                setInterfaceState(PANEL_RESIZE);
                sizeResize = sizeVisible;
            }
        }
        else
        {
            // Tab selector
            for (Tab* tab : tabs)
            {
                if (selectedTab != tab && mouseInBox(tab->selectBox))
                {
                    mouseSetCursor(HANDPOINT);
                    if (mouseLeftPressed()) {
                        selectedTab = tab;
                        update();
                    }
                }
            }
        }
    }

    // Panel is being resized 
    else if (isFocused() && getInterfaceState() == PANEL_RESIZE)
    {
        if (location == RIGHT_TOP || location == RIGHT_BOTTOM) {
            mouseSetCursor(WERESIZE);
            size = max(PANEL_MIN_WIDTH, sizeResize + (mousePosClick().x - mousePos().x));
        }
        else if (location == LEFT_TOP || location == LEFT_BOTTOM) {
            mouseSetCursor(WERESIZE);
            size = max(PANEL_MIN_WIDTH, sizeResize + (mousePos().x - mousePosClick().x));
        }
        else if (location == TOP) {
            mouseSetCursor(NSRESIZE);
            size = max(PANEL_MIN_HEIGHT, sizeResize + (mousePos().y - mousePosClick().y));
        }
        else if (location == BOTTOM) {
            mouseSetCursor(NSRESIZE);
            size = max(PANEL_MIN_HEIGHT, sizeResize + (mousePosClick().y - mousePos().y));
        }
        parent->update();

        // Stop resizing
        if (!mouseLeftDown()) {
            setInterfaceState(IDLE);
        }
    }
    
    selectedTab->mouseEvent();
}


void Mineimator::Panel::keyEvent()
{
    if (!selectedTab) {
        return;
    }
    
    selectedTab->keyEvent();
}


void Mineimator::Panel::setParent(Element* parent)
{
    this->parent = parent;
    for (Tab* tab : tabs) {
        tab->setParent(this);
    }
}


void Mineimator::Panel::addTab(Tab* tab)
{
    tabs.push_back(tab);
    if (tab->visible) {
        selectedTab = tab;
    }
}