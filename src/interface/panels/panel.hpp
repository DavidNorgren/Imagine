#pragma once

#include "interface/element.hpp"
#include "interface/controls/button.hpp"
#include "interface/panels/tabs/tab.hpp"


namespace Imagine
{

    /* A panel that can hold a number of tabs. */
    class Panel : public Element
    {
        public:
            Panel(int size, Cursor resizeCursor)
            {
                this->size = size;
                this->resizeCursor = resizeCursor;
                this->selectedTab = nullptr;
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

            int size, sizeVisible, sizeResize;
            Cursor resizeCursor;
            float moveGlow = 0.f;
            ScreenArea resizeBox;

            Tab* selectedTab;
            std::vector<Tab*> tabs;
            bool visible = false;
    };

}