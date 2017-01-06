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
            TabCollection(std::vector<Tab*> tabs);
            
            /* Adds a tab to the collection. */
            void addTab(Tab* tab, InsertPosition position = TAB, int index = -1) override;

            /* Removes a given tab from the collection. */
            void removeTab(Tab* tab);

            /* Element methods. */
            void update() override;
            void draw() override;
            void mouseEvent() override;
            void keyEvent() override;
            void setParent(Element* parent) override;

            Tab* selectedTab = nullptr;
            std::vector<Tab*> tabs;
            ScreenArea moveLastBox;
            float moveLastGlow = 0.f;
    };

}