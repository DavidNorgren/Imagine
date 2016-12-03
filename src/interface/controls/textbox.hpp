#pragma once

#include "interface/controls/control.hpp"

#define TEXTBOX_BOX_PADDING    5


namespace Mineimator
{
    
    /* Editable textbox. */
    class TextBox : public Control
    {
        public:
            
            TextBox(bool singleLine = true,
                    std::function<bool()> visibleFunc = TRUE_FUNC) {
                this->singleLine = singleLine;
                this->visibleFunc = visibleFunc;
                height = 15 + TEXTBOX_BOX_PADDING * 2;
            }
            
            /* Element methods. */
            void draw() override;
            void mouseEvent() override;
            void keyEvent() override;
            
            bool singleLine;
            string text = "";
    };
    
}