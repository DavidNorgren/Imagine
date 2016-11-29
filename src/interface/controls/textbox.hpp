#pragma once

#include "interface/controls/control.hpp"


namespace Mineimator
{
    
    /* Editable textbox. */
    class Textbox : public Element
    {
        public:
            
            /* Element methods. */
            void draw() override;
            //void mouseEvent(MouseEvent* event) override;
            //void keyEvent(KeyEvent* event) override;
    };
    
}