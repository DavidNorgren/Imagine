#pragma once

#include "base.hpp"
using namespace Base;

#include "interface/panels/tabs/tabsection.hpp"


namespace Mineimator
{

    /* A tab for storing a number of expandable sections. */
    class Tab : public Element
    {
        public:

            Tab(string name, bool visible, bool closeable, std::vector<TabSection*> sections)
            {
                this->name = name;
                this->closeable = closeable;
                this->visible = visible;
                this->sections = sections;
            }
            
            void addSection(TabSection* section);
            
            /* Element methods. */
            void update() override;
            void draw() override;
            void mouseEvent() override;
            void keyEvent() override;
            void setParent(Element* parent) override;

            string name;
            bool visible, closeable;
            std::vector<TabSection*> sections;
            ScreenArea selectBox;
    };
    
}
