#include "interface/interfacehandler.hpp"
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
    initTabTimeline();
    initTabSettings();
    panels[Panel::RIGHT_TOP]->addTab(tabProperties);
    panels[Panel::BOTTOM]->addTab(tabTimeline);
    panels[Panel::RIGHT_TOP]->addTab(tabSettings);
}


void Mineimator::Workspace::update()
{    
    // Set panel locations and sizes
    for (Panel* panel : panels) {
        panel->sizeVisible = (panel->tabs.size() > 0) ? panel->size : 0;
    }

    // Stop overlapping
    panels[Panel::LEFT_BOTTOM]->sizeVisible -= max(0, 
        panels[Panel::LEFT_TOP]->sizeVisible +
        panels[Panel::LEFT_BOTTOM]->sizeVisible +
        panels[Panel::RIGHT_BOTTOM]->sizeVisible +
        panels[Panel::RIGHT_TOP]->sizeVisible - box.width
    );
    panels[Panel::LEFT_TOP]->sizeVisible -= max(0,
        panels[Panel::LEFT_TOP]->sizeVisible +
        panels[Panel::RIGHT_TOP]->sizeVisible +
        panels[Panel::RIGHT_BOTTOM]->sizeVisible - box.width
    );
    panels[Panel::BOTTOM]->sizeVisible -= max(0,
        panels[Panel::TOP]->sizeVisible +
        panels[Panel::BOTTOM]->sizeVisible - box.height
    );

    // Set minimum size
    panels[Panel::LEFT_TOP]->sizeVisible = min(panels[Panel::LEFT_TOP]->sizeVisible, box.width);
    panels[Panel::RIGHT_TOP]->sizeVisible = min(panels[Panel::RIGHT_TOP]->sizeVisible, box.width);
    panels[Panel::TOP]->sizeVisible = min(panels[Panel::TOP]->sizeVisible, box.height);
    panels[Panel::BOTTOM]->sizeVisible = min(panels[Panel::BOTTOM]->sizeVisible, box.height);
    
    // Left top
    panels[Panel::LEFT_TOP]->box = {
        pos,
        panels[Panel::LEFT_TOP]->sizeVisible,
        box.height
    };
    panels[Panel::LEFT_TOP]->resizeBox = {
        panels[Panel::LEFT_TOP]->pos + (ScreenPos){ panels[Panel::LEFT_TOP]->box.width - PANEL_RESIZE_BOX_SIZE, 0 },
        PANEL_RESIZE_BOX_SIZE,
        panels[Panel::LEFT_TOP]->box.height
    };
    
    // Right top
    panels[Panel::RIGHT_TOP]->box = {
        { pos.x + box.width - panels[Panel::RIGHT_TOP]->sizeVisible, pos.y },
        panels[Panel::RIGHT_TOP]->sizeVisible,
        box.height
    };
    panels[Panel::RIGHT_TOP]->resizeBox = {
        panels[Panel::RIGHT_TOP]->pos,
        PANEL_RESIZE_BOX_SIZE,
        panels[Panel::RIGHT_TOP]->box.height
    };
    
    // Bottom
    panels[Panel::BOTTOM]->box = {
        { pos.x + panels[Panel::LEFT_TOP]->sizeVisible,
          pos.y + box.height - panels[Panel::BOTTOM]->sizeVisible },
        box.width - panels[Panel::LEFT_TOP]->sizeVisible - panels[Panel::RIGHT_TOP]->sizeVisible,
        panels[Panel::BOTTOM]->sizeVisible
    };
    panels[Panel::BOTTOM]->resizeBox = {
        panels[Panel::BOTTOM]->pos,
        panels[Panel::BOTTOM]->box.width,
        PANEL_RESIZE_BOX_SIZE
    };
    
    // Top
    panels[Panel::TOP]->box = {
        { pos.x + panels[Panel::LEFT_TOP]->sizeVisible, pos.y },
        box.width - panels[Panel::LEFT_TOP]->sizeVisible - panels[Panel::RIGHT_TOP]->sizeVisible,
        panels[Panel::TOP]->sizeVisible
    };
    panels[Panel::TOP]->resizeBox = {
        panels[Panel::TOP]->pos + (ScreenPos){ 0, panels[Panel::TOP]->box.height - PANEL_RESIZE_BOX_SIZE },
        panels[Panel::TOP]->box.width,
        PANEL_RESIZE_BOX_SIZE
    };
    
    // Left bottom
    panels[Panel::LEFT_BOTTOM]->box = {
        { pos.x + panels[Panel::LEFT_TOP]->sizeVisible,
          pos.y + panels[Panel::TOP]->sizeVisible },
        panels[Panel::LEFT_BOTTOM]->sizeVisible,
        box.height - panels[Panel::TOP]->sizeVisible - panels[Panel::BOTTOM]->sizeVisible
    };
    panels[Panel::LEFT_BOTTOM]->resizeBox = {
        panels[Panel::LEFT_BOTTOM]->pos + (ScreenPos){ panels[Panel::LEFT_BOTTOM]->box.width - PANEL_RESIZE_BOX_SIZE, 0 },
        PANEL_RESIZE_BOX_SIZE,
        panels[Panel::LEFT_BOTTOM]->box.height
    };
    
    // Right bottom
    panels[Panel::RIGHT_BOTTOM]->box = {
        { pos.x + box.width - panels[Panel::RIGHT_TOP]->sizeVisible - panels[Panel::RIGHT_BOTTOM]->sizeVisible,
          pos.y + panels[Panel::TOP]->sizeVisible },
        panels[Panel::RIGHT_BOTTOM]->sizeVisible,
        box.height - panels[Panel::TOP]->sizeVisible - panels[Panel::BOTTOM]->sizeVisible
    };
    panels[Panel::RIGHT_BOTTOM]->resizeBox = {
        panels[Panel::RIGHT_BOTTOM]->pos,
        PANEL_RESIZE_BOX_SIZE,
        panels[Panel::RIGHT_BOTTOM]->box.height
    };
    
    // Update panel elements
    for (Panel* panel : panels) {
        panel->update();
    }
    if (isInterfaceState(TAB_MOVE)) {
        getFocused()->update();
    }

    // Update view area
    viewArea = {
        { pos.x + panels[Panel::LEFT_TOP]->sizeVisible + panels[Panel::LEFT_BOTTOM]->sizeVisible,
          pos.y + panels[Panel::TOP]->sizeVisible },
        box.width - (panels[Panel::LEFT_TOP]->sizeVisible + panels[Panel::LEFT_BOTTOM]->sizeVisible +
                     panels[Panel::RIGHT_TOP]->sizeVisible + panels[Panel::RIGHT_BOTTOM]->sizeVisible),
        box.height - (panels[Panel::TOP]->sizeVisible + panels[Panel::BOTTOM]->sizeVisible)
    };
}


void Mineimator::Workspace::draw()
{
    // Draw each view
    //viewMain->draw();
    //viewSecond->draw();

    // Draw each panel
    for (Panel* panel : panels) {
        panel->draw();
    }

    drawBox(viewArea, Color(0.1f));

    // Draw the moved tab on top
    if (isInterfaceState(TAB_MOVE)) {
        getFocused()->draw();
    }
}


void Mineimator::Workspace::mouseEvent()
{
    for (Panel* panel : panels) {
        panel->mouseEvent();
    }

    // Currently moved tab
    if (isInterfaceState(TAB_MOVE))
    {
        Tab* tab = (Tab*)getFocused();
        tab->mouseEvent();

        // Find the new location
        Panel* newPanel = nullptr;
        int newTabIndex = -1;

        // Mouse is on existing panel
        for (Panel* panel : panels) {
            if (mouseInBox(panel->box)) {
                newPanel = panel;
            }
        }

        // Find an empty panel to drop the tab into
        if (!newPanel)
        {
            float moveTopHeight = viewArea.height,
                  moveBottomHeight = viewArea.height,
                  moveLeftTopWidth = viewArea.width,
                  moveRightTopWidth = viewArea.width,
                  moveLeftBottomWidth = viewArea.width;

            // Adjust top panel check size
            if (!panels[Panel::LEFT_TOP]->sizeVisible || !panels[Panel::LEFT_BOTTOM]->sizeVisible
             || !panels[Panel::RIGHT_TOP]->sizeVisible || !panels[Panel::RIGHT_BOTTOM]->sizeVisible) {
                 moveTopHeight *= 0.33;
            }
            else if (!panels[Panel::BOTTOM]->sizeVisible) {
                moveTopHeight *= 0.5;
            }

            // Adjust bottom panel check size
            if (!panels[Panel::LEFT_TOP]->sizeVisible || !panels[Panel::LEFT_BOTTOM]->sizeVisible
             || !panels[Panel::RIGHT_TOP]->sizeVisible || !panels[Panel::RIGHT_BOTTOM]->sizeVisible) {
                 moveBottomHeight *= 0.33;
            }
            else if (!panels[Panel::BOTTOM]->sizeVisible) {
                moveBottomHeight *= 0.5;
            }

            // Adjust top left panel check size
            if (!panels[Panel::RIGHT_TOP]->sizeVisible || !panels[Panel::RIGHT_BOTTOM]->sizeVisible) {
                moveLeftTopWidth *= 0.5f;
            }
            if (!panels[Panel::LEFT_BOTTOM]->sizeVisible) {
                moveLeftTopWidth *= 0.5f;
            }

            // Adjust top right panel check size
            if (!panels[Panel::LEFT_TOP]->sizeVisible || !panels[Panel::LEFT_BOTTOM]->sizeVisible) {
                moveRightTopWidth *= 0.5f;
            }
            if (!panels[Panel::RIGHT_BOTTOM]->sizeVisible) {
                moveRightTopWidth *= 0.5f;
            }

            // Adjust left bottom panel check size
            if (!panels[Panel::RIGHT_TOP]->sizeVisible || !panels[Panel::RIGHT_BOTTOM]->sizeVisible) {
                moveLeftBottomWidth *= 0.5f;
            }

            // Find panel
            if (mousePos().y <= viewArea.pos.y + moveTopHeight && !panels[Panel::TOP]->sizeVisible) {
                newPanel = panels[Panel::TOP];
            }
            else if (mousePos().y >= viewArea.pos.y + viewArea.height - moveBottomHeight && !panels[Panel::BOTTOM]->sizeVisible) {
                newPanel = panels[Panel::BOTTOM];
            }
            else if (mousePos().x <= viewArea.pos.x + moveLeftTopWidth && !panels[Panel::LEFT_TOP]->sizeVisible) {
                newPanel = panels[Panel::LEFT_TOP];
            }
            else if (mousePos().x <= viewArea.pos.x + moveLeftBottomWidth && !panels[Panel::LEFT_BOTTOM]->sizeVisible) {
                newPanel = panels[Panel::LEFT_BOTTOM];
            }
            else if (mousePos().x >= viewArea.pos.x + viewArea.width - moveRightTopWidth && !panels[Panel::RIGHT_TOP]->sizeVisible) {
                newPanel = panels[Panel::RIGHT_TOP];
            }
            else {
                newPanel = panels[Panel::RIGHT_BOTTOM];
            }
        }

        // Drop tab into a new panel
        if (!mouseLeftDown()) {
            newPanel->addTab(tab, newTabIndex);
            update();
            tab->setParent(newPanel);
            setInterfaceState(IDLE);
        }
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