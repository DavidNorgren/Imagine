#include "interface/interfacehandler.hpp"
#include "mineimatorapp.hpp"


void Mineimator::Panel::update()
{
    if (!selectedTab) {
        return;
    }

    // Tab selector
    int selectTotalWidth = 0;
    for (Tab* tab : tabs) {
        tab->selectBox.width = stringGetWidth(tab->name) + 20;
        tab->selectBox.height = TAB_SELECT_HEIGHT;
        selectTotalWidth += tab->selectBox.width;
    }
    
    ScreenPos selectPos = pos;
    for (Tab* tab : tabs)
    {
        if (selectedTab != tab) {
            tab->selectBox.width -= max(0, selectTotalWidth - box.width) / (tabs.size() - 1);
        }
        tab->selectBox.pos = selectPos;
        selectPos.x += tab->selectBox.width;
    }

    selectedTab->box = {
        { pos.x + TAB_CONTENT_PADDING, pos.y + TAB_SELECT_HEIGHT + TAB_CONTENT_PADDING }, 
        box.width - TAB_CONTENT_PADDING * 2,
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
        drawTextAligned(tab->name, tab->selectBox.pos + (ScreenPos){ tab->selectBox.width / 2, tab->selectBox.height / 2 }, CENTER, MIDDLE, SETTING_INTERFACE_COLOR_TEXT);
    }
    
    // TODO: Tab selector
    selectedTab->draw();
}


void Mineimator::Panel::mouseEvent()
{
    if (!selectedTab) {
        return;
    }
    
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