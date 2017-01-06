#include "interface/tabs/tabcollection.hpp"
#include "imagineapp.hpp"


Imagine::TabCollection::TabCollection(std::vector<Tab*> tabs) :
    Container(OTHER, {}, FIXED)
{
    this->tabs = tabs;

    // Select the first visible tab
    visible = false;
    for (Tab* tab : tabs)
    {
        if (tab->visible)
        {
            visible = true;
            selectedTab = tab;
            break;
        }
    }
}


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
    int i = 0;
    for (Tab* tab : tabs)
    {
        if (selectedTab != tab)
        {
            drawTextAligned(tab->name,
                            tab->selectBox.pos + (ScreenPos){ tab->selectBox.width / 2, tab->selectBox.height / 2 },
                            CENTER, MIDDLE,
                            SETTING_INTERFACE_COLOR_TEXT);

            if (interface->moveContainer == this && interface->moveIndex == i) {
                drawBox(tab->selectBox, Color(COLOR_YELLOW, 0.25f));
            }
        }
        i++;
    }       

    if (interface->moveContainer == this && interface->moveIndex == i) {
        drawBox(moveLastBox, Color(COLOR_YELLOW, 0.25f));
    }
    
    // Draw current tab contents
    selectedTab->draw();

    if (interface->moveContainer == this && interface->moveIndex == vectorFind(tabs, selectedTab)) {
        drawBox(selectedTab->box, Color(COLOR_YELLOW, 0.25f));
        drawBox(selectedTab->selectBox, Color(COLOR_YELLOW, 0.25f));
    }
}


void Imagine::TabCollection::mouseEvent()
{
    if (isInterfaceState(TAB_MOVE))
    {
        if (mouseInBox(box))
        {
            Tab* movedTab = (Tab*)getFocused();

            // Check mouse with each tab header
            int i = 0;
            for (Tab* tab : tabs)
            {
                if (mouseInBox(tab->selectBox)) {
                    interface->moveContainer = this;
                    interface->moveIndex = i;
                    break;
                }
                i++;
            }

            // Check with last tab
            moveLastBox = {
                { tabs.back()->selectBox.pos.x + tabs.back()->selectBox.width, tabs.back()->selectBox.pos.y },
                movedTab->selectBox.width,
                TAB_SELECT_HEIGHT
            };
            if (mouseInBox(moveLastBox)) {
                interface->moveContainer = this;
                interface->moveIndex = tabs.size();
            }

            // Check with currently selected tab
            ScreenArea currentSelectBox = {
                { insertBox[LEFT].pos.x + CONTAINER_INSERT_SIZE, insertBox[TOP].pos.y + CONTAINER_INSERT_SIZE },
                max(0, insertBox[RIGHT].pos.x - (insertBox[LEFT].pos.x + CONTAINER_INSERT_SIZE)),
                max(0, insertBox[BOTTOM].pos.y - (insertBox[TOP].pos.y + CONTAINER_INSERT_SIZE))
            };
            if (mouseInBox(currentSelectBox)) {
                interface->moveContainer = this;
                interface->moveIndex = vectorFind(tabs, selectedTab);
            }
        }
    }
    else
    {
        mouseOn = (parent->mouseOn && mouseInBox(box));

        if (mouseOn)
        {
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
        }
        
        selectedTab->mouseEvent();
    }
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


void Imagine::TabCollection::addTab(Tab* tab, InsertPosition position, int index)
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

    tab->setParent(this);
    update();
}


void Imagine::TabCollection::removeTab(Tab* tab)
{
    // Remove from list
    int i = vectorFind(tabs, tab);
    vectorErase(tabs, i);

    // If this was the selected tab, select the next or previous one
    if (tabs.size() > 0)
    {
        if (selectedTab == tab) {
            selectedTab = (i == tabs.size()) ? tabs[i - 1] : tabs[i];
        }
    }
    else {
        // No more tabs, remove
        ((Container*)parent)->removeSubContainer(this);
        return;
    }

    update();
}