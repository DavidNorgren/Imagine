#pragma once

#include "base.hpp"

#include "interface/panels/tabs/tabsection.hpp"
#include "interface/controls/scrollbar.hpp"


namespace Imagine
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
                scrollBar = new ScrollBar(ScrollBar::VERTICAL, &sectionsOffset.y);
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
            ScreenArea selectBox, sectionsBox;
            ScreenPos sectionsOffset = { 0, 0 }, moveStartPos, moveSelectStartPos;
            int sectionsSize = 0;
            ScrollBar* scrollBar;
    };
    
}
