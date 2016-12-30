/*#include "interface/interfacehandler.hpp"
#include "interface/workspace.hpp"


Imagine::Workspace::Workspace()
{
    // Create panels
    panels.resize(6);
    panels[LEFT_TOP] = new Panel(PANEL_START_WIDTH, WERESIZE);
    panels[RIGHT_TOP] = new Panel(PANEL_START_WIDTH, WERESIZE);
    panels[BOTTOM] = new Panel(PANEL_START_HEIGHT, NSRESIZE);
    panels[TOP] = new Panel(PANEL_START_HEIGHT, NSRESIZE);
    panels[LEFT_BOTTOM] = new Panel(PANEL_START_WIDTH, WERESIZE);
    panels[RIGHT_BOTTOM] = new Panel(PANEL_START_WIDTH, WERESIZE);
    
    // Create tabs and assign to panels
    initTabProperties();
    initTabTimeline();
    initTabSettings();
    panels[RIGHT_TOP]->addTab(tabProperties);
    panels[BOTTOM]->addTab(tabTimeline);
    panels[RIGHT_TOP]->addTab(tabSettings);

    // Create views
    viewMain = new View();
}


void Imagine::Workspace::update()
{
    // Set panel locations and sizes
    for (Panel* panel : panels) {
        panel->sizeVisible = panel->visible ? panel->size : 0;
    }

    // Set minimum size
    int totalWidth = (panels[RIGHT_TOP]->sizeVisible + panels[RIGHT_BOTTOM]->sizeVisible + panels[LEFT_BOTTOM]->sizeVisible + panels[LEFT_TOP]->sizeVisible);
    int totalWidthTabs = (panels[RIGHT_TOP]->visible + panels[RIGHT_BOTTOM]->visible + panels[LEFT_BOTTOM]->visible + panels[LEFT_TOP]->visible);
    int totalHeight = (panels[TOP]->sizeVisible + panels[BOTTOM]->sizeVisible);
    int totalHeightTabs = (panels[TOP]->visible + panels[BOTTOM]->visible);

    if (panels[LEFT_TOP]->visible) {
        panels[LEFT_TOP]->sizeVisible = panels[LEFT_TOP]->sizeVisible - max(0, totalWidth - box.width) / totalWidthTabs;
    }
    if (panels[RIGHT_TOP]->visible) {
        panels[RIGHT_TOP]->sizeVisible = panels[RIGHT_TOP]->sizeVisible - max(0, totalWidth - box.width) / totalWidthTabs;
    }
    if (panels[BOTTOM]->visible) {
        panels[BOTTOM]->sizeVisible = panels[BOTTOM]->sizeVisible - max(0, totalHeight - box.height) / totalHeightTabs;
    }
    if (panels[TOP]->visible) {
        panels[TOP]->sizeVisible = panels[TOP]->sizeVisible - max(0, totalHeight - box.height) / totalHeightTabs;
    }
    if (panels[LEFT_BOTTOM]->visible) {
        panels[LEFT_BOTTOM]->sizeVisible = panels[LEFT_BOTTOM]->sizeVisible - max(0, totalWidth - box.width) / totalWidthTabs;
    }
    if (panels[RIGHT_BOTTOM]->visible) {
        panels[RIGHT_BOTTOM]->sizeVisible = panels[RIGHT_BOTTOM]->sizeVisible - max(0, totalWidth - box.width) / totalWidthTabs;
    }

    // Left top
    panels[LEFT_TOP]->box = {
        pos,
        max(PANEL_MIN_WIDTH, panels[LEFT_TOP]->sizeVisible),
        box.height
    };
    panels[LEFT_TOP]->resizeBox = {
        panels[LEFT_TOP]->pos + (ScreenPos){ panels[LEFT_TOP]->box.width - PANEL_RESIZE_BOX_SIZE, 0 },
        PANEL_RESIZE_BOX_SIZE,
        panels[LEFT_TOP]->box.height
    };
    
    // Right top
    panels[RIGHT_TOP]->box = {
        { pos.x + box.width - panels[RIGHT_TOP]->sizeVisible, pos.y },
        max(PANEL_MIN_WIDTH, panels[RIGHT_TOP]->sizeVisible),
        box.height
    };
    panels[RIGHT_TOP]->resizeBox = {
        panels[RIGHT_TOP]->pos,
        PANEL_RESIZE_BOX_SIZE,
        panels[RIGHT_TOP]->box.height
    };
    
    // Bottom
    panels[BOTTOM]->box = {
        { pos.x + panels[LEFT_TOP]->sizeVisible,
          pos.y + box.height - panels[BOTTOM]->sizeVisible },
        box.width - panels[LEFT_TOP]->sizeVisible - panels[RIGHT_TOP]->sizeVisible,
        max(PANEL_MIN_HEIGHT, panels[BOTTOM]->sizeVisible)
    };
    panels[BOTTOM]->resizeBox = {
        panels[BOTTOM]->pos,
        panels[BOTTOM]->box.width,
        PANEL_RESIZE_BOX_SIZE
    };
    
    // Top
    panels[TOP]->box = {
        { pos.x + panels[LEFT_TOP]->sizeVisible, pos.y },
        box.width - panels[LEFT_TOP]->sizeVisible - panels[RIGHT_TOP]->sizeVisible,
        max(PANEL_MIN_HEIGHT, panels[TOP]->sizeVisible)
    };
    panels[TOP]->resizeBox = {
        panels[TOP]->pos + (ScreenPos){ 0, panels[TOP]->box.height - PANEL_RESIZE_BOX_SIZE },
        panels[TOP]->box.width,
        PANEL_RESIZE_BOX_SIZE
    };
    
    // Left bottom
    panels[LEFT_BOTTOM]->box = {
        { pos.x + panels[LEFT_TOP]->sizeVisible,
          pos.y + panels[TOP]->sizeVisible },
        max(PANEL_MIN_WIDTH, panels[LEFT_BOTTOM]->sizeVisible),
        box.height - panels[TOP]->sizeVisible - panels[BOTTOM]->sizeVisible
    };
    panels[LEFT_BOTTOM]->resizeBox = {
        panels[LEFT_BOTTOM]->pos + (ScreenPos){ panels[LEFT_BOTTOM]->box.width - PANEL_RESIZE_BOX_SIZE, 0 },
        PANEL_RESIZE_BOX_SIZE,
        panels[LEFT_BOTTOM]->box.height
    };
    
    // Right bottom
    panels[RIGHT_BOTTOM]->box = {
        { pos.x + box.width - panels[RIGHT_TOP]->sizeVisible - panels[RIGHT_BOTTOM]->sizeVisible,
          pos.y + panels[TOP]->sizeVisible },
        max(PANEL_MIN_WIDTH, panels[RIGHT_BOTTOM]->sizeVisible),
        box.height - panels[TOP]->sizeVisible - panels[BOTTOM]->sizeVisible
    };
    panels[RIGHT_BOTTOM]->resizeBox = {
        panels[RIGHT_BOTTOM]->pos,
        PANEL_RESIZE_BOX_SIZE,
        panels[RIGHT_BOTTOM]->box.height
    };

    // Update panel elements
    for (Panel* panel : panels)
    {
        panel->box.width =max(PANEL_MIN_WIDTH, panel->box.width);
        panel->box.height = max(PANEL_MIN_HEIGHT, panel->box.height);
        panel->update();
    }

    // Updated the moved tab
    if (isInterfaceState(TAB_MOVE)) {
        getFocused()->update();
    }

    // Update view area
    viewArea = {
        { pos.x + panels[LEFT_TOP]->sizeVisible + panels[LEFT_BOTTOM]->sizeVisible,
          pos.y + panels[TOP]->sizeVisible },
        max(0, box.width - (panels[LEFT_TOP]->sizeVisible + panels[LEFT_BOTTOM]->sizeVisible + panels[RIGHT_TOP]->sizeVisible + panels[RIGHT_BOTTOM]->sizeVisible)),
        max(0, box.height - (panels[TOP]->sizeVisible + panels[BOTTOM]->sizeVisible))
    };
    viewMain->box = viewArea;
    viewMain->update();
}


void Imagine::Workspace::draw()
{
    // Draw each view
    viewMain->draw();
    //viewSecond->draw();

    // Draw each panel
    for (Panel* panel : panels) {
        panel->draw();
    }

    // Draw the moved tab on top
    if (isInterfaceState(TAB_MOVE)) {
        getFocused()->draw();
    }
}


void Imagine::Workspace::mouseEvent()
{
    // Find the panel on the mouse
    Panel* mouseOnPanel = nullptr;
    for (int i = panels.size(); i > 0;)
    {
        Panel* panel = panels[--i];
        if (!panel->visible) {
            continue;
        }
        
        panel->mouseOn = false;
        if (!mouseOnPanel && mouseInBox(panel->box) && isInterfaceState(IDLE)) {
            panel->mouseOn = true;
            mouseOnPanel = panel;
        }
    }

    // Moving tab
    if (isInterfaceState(TAB_MOVE))
    {
        Tab* tab = (Tab*)getFocused();
        tab->mouseEvent();

        // Find the new location
        Panel* newPanel = nullptr;
        int newTabIndex = -1;

        // Mouse is on existing panel
        for (Panel* panel : panels)
        {
            if (panel->visible && mouseInBox(panel->box))
            {
                newPanel = panel;
                for (int t = 0; t < panel->tabs.size(); t++)
                {
                    Tab* tab = panel->tabs[t];
                    if (mouseInBox(tab->selectBox) ||
                        (mouseInBox(tab->box) && panel->selectedTab == tab)) {
                        newTabIndex = t;
                    }
                }
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
            if (!panels[LEFT_TOP]->sizeVisible || !panels[LEFT_BOTTOM]->sizeVisible
             || !panels[RIGHT_TOP]->sizeVisible || !panels[RIGHT_BOTTOM]->sizeVisible) {
                 moveTopHeight *= 0.33;
            }
            else if (!panels[BOTTOM]->sizeVisible) {
                moveTopHeight *= 0.5;
            }

            // Adjust bottom panel check size
            if (!panels[LEFT_TOP]->sizeVisible || !panels[LEFT_BOTTOM]->sizeVisible
             || !panels[RIGHT_TOP]->sizeVisible || !panels[RIGHT_BOTTOM]->sizeVisible) {
                 moveBottomHeight *= 0.33;
            }
            else if (!panels[BOTTOM]->sizeVisible) {
                moveBottomHeight *= 0.5;
            }

            // Adjust top left panel check size
            if (!panels[RIGHT_TOP]->sizeVisible || !panels[RIGHT_BOTTOM]->sizeVisible) {
                moveLeftTopWidth *= 0.5f;
            }
            if (!panels[LEFT_BOTTOM]->sizeVisible) {
                moveLeftTopWidth *= 0.5f;
            }

            // Adjust top right panel check size
            if (!panels[LEFT_TOP]->sizeVisible || !panels[LEFT_BOTTOM]->sizeVisible) {
                moveRightTopWidth *= 0.5f;
            }
            if (!panels[RIGHT_BOTTOM]->sizeVisible) {
                moveRightTopWidth *= 0.5f;
            }

            // Adjust left bottom panel check size
            if (!panels[RIGHT_TOP]->sizeVisible || !panels[RIGHT_BOTTOM]->sizeVisible) {
                moveLeftBottomWidth *= 0.5f;
            }

            // Find panel
            if (mousePos().y <= viewArea.pos.y + moveTopHeight && !panels[TOP]->sizeVisible) {
                newPanel = panels[TOP];
            }
            else if (mousePos().y >= viewArea.pos.y + viewArea.height - moveBottomHeight && !panels[BOTTOM]->sizeVisible) {
                newPanel = panels[BOTTOM];
            }
            else if (mousePos().x <= viewArea.pos.x + moveLeftTopWidth && !panels[LEFT_TOP]->sizeVisible) {
                newPanel = panels[LEFT_TOP];
            }
            else if (mousePos().x <= viewArea.pos.x + moveLeftBottomWidth && !panels[LEFT_BOTTOM]->sizeVisible) {
                newPanel = panels[LEFT_BOTTOM];
            }
            else if (mousePos().x >= viewArea.pos.x + viewArea.width - moveRightTopWidth && !panels[RIGHT_TOP]->sizeVisible) {
                newPanel = panels[RIGHT_TOP];
            }
            else {
                newPanel = panels[RIGHT_BOTTOM];
            }
        }

        // Drop tab into a new panel
        if (!mouseLeftDown())
        {
            newPanel->addTab(tab, newTabIndex);
            update();
            tab->setParent(newPanel);
            setInterfaceState(IDLE);
        }
    }

    // Panel being resized 
    else if (isInterfaceState(PANEL_RESIZE))
    {
        Panel* panel = (Panel*)getFocused();
        PanelLocation location = (PanelLocation)vectorFind(panels, panel);

        if (location == RIGHT_TOP)
        {
            panel->size = clamp(
                panel->sizeResize + (mousePosClick().x - mousePos().x),
                PANEL_MIN_WIDTH,
                box.width - (panels[RIGHT_BOTTOM]->sizeVisible + panels[LEFT_BOTTOM]->sizeVisible + panels[LEFT_TOP]->sizeVisible)
            );
        }
        else if (location == LEFT_TOP)
        {
            panel->size = clamp(
                panel->sizeResize + (mousePos().x - mousePosClick().x),
                PANEL_MIN_WIDTH,
                box.width - (panels[RIGHT_TOP]->sizeVisible + panels[RIGHT_BOTTOM]->sizeVisible + panels[LEFT_BOTTOM]->sizeVisible)
            );
        } 
        else if (location == RIGHT_BOTTOM)
        {
            panel->size = clamp(
                panel->sizeResize + (mousePosClick().x - mousePos().x),
                PANEL_MIN_WIDTH,
                box.width - (panels[RIGHT_TOP]->sizeVisible + panels[LEFT_BOTTOM]->sizeVisible + panels[LEFT_TOP]->sizeVisible)
            );
        }
        else if (location == LEFT_BOTTOM)
        {
            panel->size = clamp(
                panel->sizeResize + (mousePos().x - mousePosClick().x),
                PANEL_MIN_WIDTH,
                box.width - (panels[RIGHT_TOP]->sizeVisible + panels[RIGHT_BOTTOM]->sizeVisible + panels[LEFT_TOP]->sizeVisible)
            );
        }
        else if (location == TOP)
        {
            panel->size = clamp(
                panel->sizeResize + (mousePos().y - mousePosClick().y),
                PANEL_MIN_HEIGHT,
                box.height - panels[BOTTOM]->sizeVisible
            );
        }
        else if (location == BOTTOM)
        {
            panel->size = clamp(
                panel->sizeResize + (mousePosClick().y - mousePos().y),
                PANEL_MIN_HEIGHT,
                box.height - panels[TOP]->sizeVisible
            );
        }

        mouseSetCursor(panel->resizeCursor);
        update();

        // Stop resizing
        if (!mouseLeftDown()) {
            setInterfaceState(IDLE);
        }
    }
    else
    {
        for (Panel* panel : panels) {
            panel->mouseEvent();
        }
    }
}


void Imagine::Workspace::keyEvent()
{
    for (Panel* panel : panels) {
        panel->keyEvent();
    }
}


void Imagine::Workspace::setParent(Element* parent)
{
    this->parent = parent;
    for (Panel* panel : panels) {
        panel->setParent(this);
    }
    viewMain->setParent(this);
}*/