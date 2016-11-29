#pragma once

#include "interface/controls/control.hpp"


namespace Mineimator
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
            //void mouseEvent(MouseEvent* event) override;
            //void keyEvent(KeyEvent* event) override;
            void setParent(Element* parent) override;

            std::vector<Control*> controls;
    };
    
}