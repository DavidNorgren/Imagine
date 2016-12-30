#include "interface/tabs/tabcollection.hpp"
#include "imagineapp.hpp"


void Imagine::TabCollection::update()
{
    // Get total width of every tab
    int selectTotalWidth = 0;
    for (Tab* tab : tabs)
    {
        // The selected tab is displayed with bold text
        if (selectedTab == tab) {
            tab->selectBox.width = stringGetWidth(tab->name, BOLD) + 20;
        } 
        else {
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
        { pos.x, pos.y + TAB_SELECT_HEIGHT },
        box.width, box.height - TAB_SELECT_HEIGHT
    };
    selectedTab->update();
}

void Imagine::TabCollection::draw()
{
    // Tab selector
    drawBox({ box.pos, box.width, TAB_SELECT_HEIGHT }, SETTING_INTERFACE_COLOR_BACKGROUND);
    for (Tab* tab : tabs)
    {
        if (selectedTab != tab)
        {
            drawTextAligned(tab->name,
                            tab->selectBox.pos + (ScreenPos){ tab->selectBox.width / 2, tab->selectBox.height / 2 },
                            CENTER, MIDDLE,
                            SETTING_INTERFACE_COLOR_TEXT);
        }
    }
    
    // Draw current tab contents
    selectedTab->draw();
}


void Imagine::TabCollection::mouseEvent()
{
    if (mouseOn)
    {
        /*if (mouseInBox(resizeBox))
        {
            // The mouse is near the edge for resizing
            mouseSetCursor(resizeCursor);
            if (mouseLeftPressed())
            {
                focus();
                setInterfaceState(PANEL_RESIZE);
                sizeResize = sizeVisible;
            }
        }
        else
        {*/
            // Tab selector
            for (Tab* tab : tabs)
            {
                if (mouseInBox(tab->selectBox))
                {
                    // Select the tab if clicked
                    if (selectedTab != tab)
                    {
                        mouseSetCursor(HANDPOINT);
                        if (mouseLeftPressed()) {
                            selectedTab = tab;
                            update();
                        }
                    }
                }
            }
        //}
    }
    
    selectedTab->mouseOn = (mouseOn && mouseInBox(selectedTab->box));
    selectedTab->mouseEvent();
}


void Imagine::TabCollection::keyEvent()
{
    selectedTab->keyEvent();
}


void Imagine::TabCollection::setParent(Element* parent)
{
    this->parent = parent;
    for (Tab* tab : tabs) {
        tab->setParent(this);
    }
}


void Imagine::TabCollection::addTab(Tab* tab, int index)
{
    // Append
    if (index < 0) {
        index = tabs.size();
    }

    vectorInsert(tabs, index, tab);
    if (tab->visible) {
        selectedTab = tab;
        visible = true;
    }
}


void Imagine::TabCollection::removeTab(Tab* tab)
{
    // Remove from list
    int i = vectorFind(tabs, tab);
    vectorErase(tabs, i);

    // If this was the selected tab, select the next or previous one
    if (selectedTab == tab)
    {
        if (tabs.size() > 0) {
            selectedTab = (i == tabs.size()) ? tabs[i - 1] : tabs[i];
        }
        else {
            selectedTab = nullptr;
            visible = false;
        }
    }
}