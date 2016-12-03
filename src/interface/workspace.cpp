#include "interface/workspace.hpp"


Mineimator::Workspace::Workspace()
{
    // Create panels
    panels.push_back(new Panel(Panel::LEFT_TOP,      PANEL_START_WIDTH));
    panels.push_back(new Panel(Panel::RIGHT_TOP,     PANEL_START_WIDTH));
    panels.push_back(new Panel(Panel::BOTTOM,        PANEL_START_HEIGHT));
    panels.push_back(new Panel(Panel::TOP,           PANEL_START_HEIGHT));
    panels.push_back(new Panel(Panel::LEFT_BOTTOM,   PANEL_START_WIDTH));
    panels.push_back(new Panel(Panel::RIGHT_BOTTOM,  PANEL_START_WIDTH));
    
    // Create tabs and assign to panels
    initTabProperties();
    panels[Panel::RIGHT_TOP]->addTab(tabProperties);
}


void Mineimator::Workspace::update()
{    
    // Set panel locations and sizes
    for (Panel* panel : panels) {
        panel->size_visible = (panel->tabs.size() > 0) ? panel->size : 0;
    }
    
    panels[Panel::LEFT_TOP]->box = {
        pos,
        panels[Panel::LEFT_TOP]->size_visible,
        box.height
    };
    
    panels[Panel::RIGHT_TOP]->box = {
        { pos.x + box.width - panels[Panel::RIGHT_TOP]->size_visible, pos.y },
        panels[Panel::RIGHT_TOP]->size_visible,
        box.height
    };
    
    panels[Panel::BOTTOM]->box = {
        { pos.x + panels[Panel::LEFT_TOP]->size_visible, pos.y + box.height - panels[Panel::BOTTOM]->size_visible },
        box.width - panels[Panel::LEFT_TOP]->size_visible - panels[Panel::RIGHT_TOP]->size_visible,
        panels[Panel::BOTTOM]->size_visible
    };
    
    panels[Panel::TOP]->box = {
        { pos.x + panels[Panel::LEFT_TOP]->size_visible, pos.y },
        box.width - panels[Panel::LEFT_TOP]->size_visible - panels[Panel::RIGHT_TOP]->size_visible,
        panels[Panel::TOP]->size_visible
    };
    
    panels[Panel::LEFT_BOTTOM]->box = {
        { pos.x + panels[Panel::LEFT_TOP]->size_visible, pos.y + panels[Panel::TOP]->size_visible },
        panels[Panel::LEFT_BOTTOM]->size_visible,
        box.height - panels[Panel::TOP]->size_visible - panels[Panel::BOTTOM]->size_visible
    };
    
    panels[Panel::RIGHT_BOTTOM]->box = {
        { pos.x + box.width - panels[Panel::RIGHT_TOP]->size_visible - panels[Panel::RIGHT_BOTTOM]->size_visible, pos.y + panels[Panel::TOP]->size_visible },
        panels[Panel::RIGHT_BOTTOM]->size_visible,
        box.height - panels[Panel::TOP]->size_visible - panels[Panel::BOTTOM]->size_visible
    };
    
    // Update panel elements
    for (Panel* panel : panels) {
        panel->update();
    }
}


void Mineimator::Workspace::draw()
{
    // Draw each panel
    for (Panel* panel : panels) {
        panel->draw();
    }
}


void Mineimator::Workspace::mouseEvent()
{
    for (Panel* panel : panels) {
        panel->mouseEvent();
    }
}


void Mineimator::Workspace::keyEvent()
{
    for (Panel* panel : panels) {
        panel->keyEvent();
    }
}


void Mineimator::Workspace::setParent(Element* parent)
{
    this->parent = parent;
    for (Panel* panel : panels) {
        panel->setParent(this);
    }
}