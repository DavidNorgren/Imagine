#pragma once

#include "interface/container.hpp"
#include "interface/controls/button.hpp"
#include "interface/tabs/tab.hpp"


namespace Imagine
{

    /* A container for a number of tabs, each with their sections with settings. */
    class TabCollection : public Container
    {
        public:
            TabCollection(std::vector<Tab*> tabs) :
                Container(OTHER, {}, FIXED)
            {
                visible = false;
                for (Tab* tab : tabs) {
                    addTab(tab);
                }
            }
            
            /* Adds an initial tab to the panel. */
            void addTab(Tab* tab, int index = -1);

            /* Removes a given tab from the panel. */
            void removeTab(Tab* tab);

            /* Element methods. */
            void update() override;
            void draw() override;
            void mouseEvent() override;
            void keyEvent() override;
            void setParent(Element* parent) override;

            Tab* selectedTab = nullptr;
            std::vector<Tab*> tabs;

            Cursor resizeCursor;
            float moveGlow = 0.f;
    };

}