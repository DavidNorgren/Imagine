#include "interface/interfacehandler.hpp"
#include "mineimatorapp.hpp"


void Mineimator::Panel::update()
{
    if (!selectedTab) {
        return;
    }
    
    selectedTab->box = { { pos.x + 5, pos.y + 5 }, box.width - 10, box.height - 10 };
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
    
    // TODO: Tab selector
    selectedTab->draw();
}


void Mineimator::Panel::mouseEvent()
{
    if (!selectedTab) {
        return;
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