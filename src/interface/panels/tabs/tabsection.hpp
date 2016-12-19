#pragma once

#include "base.hpp"

#include "interface/controls/control.hpp"
#include "interface/controls/button.hpp"


namespace Mineimator
{
    
    /* A section for storing a specific collection of controls. */
    class TabSection : public Element
    {
        public:

            TabSection(string name, bool open, std::vector<Control*> controls);
            
            /* Element methods. */
            void update() override;
            void draw() override;
            void mouseEvent() override;
            void keyEvent() override;
            void setParent(Element* parent) override;

            string name;
            bool open;
            IconButton* openButton;
            std::vector<Control*> controls;
    };
    
}