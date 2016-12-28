#pragma once

#include "interface/controls/control.hpp"


namespace Imagine
{
    
    /* A row of controls that is adjusted to the panel width. */
    class Row : public Control
    {
        public:
            Row(std::vector<Control*> controls,
                std::function<bool()> visibleFunc = TRUE_FUNC)
            {
                this->controls = controls;
                this->visibleFunc = visibleFunc;
            }
            
            /* Element methods. */
            void update() override;
            void draw() override;
            void mouseEvent() override;
            void keyEvent() override;
            void setParent(Element* parent) override;

        private:
            std::vector<Control*> controls;
    };
    
}