#pragma once

#include "base.hpp"
using namespace Base;

#include "interface/element.hpp"
#include "interface/controls/button.hpp"
#include "interface/panels/tabs/tab.hpp"


namespace Mineimator
{

    /* A panel that can hold a number of tabs. */
    class Panel : public Element
    {
        public:
            
            enum Location
            {
                LEFT_BOTTOM,
                RIGHT_BOTTOM,
                BOTTOM,
                TOP,
                LEFT_TOP,
                RIGHT_TOP
            };

            Panel(Location location, int size)
            {
                this->location = location;
                this->size = size;
                this->selectedTab = nullptr;
            }
            
            /* Adds an initial tab to the panel. */
            void addTab(Tab* tab);

            /* Element methods. */
            void update() override;
            void draw() override;
            void mouseEvent() override;
            void keyEvent() override;
            void setParent(Element* parent) override;

            Location location;
            int size, size_visible;

            Tab* selectedTab;
            std::vector<Tab*> tabs;
    };

}