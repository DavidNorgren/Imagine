#pragma once

#include "interface/controls/control.hpp"

#define TEXTBOX_BOX_PADDING    5


namespace Mineimator
{
    
    /* Editable textbox. */
    class TextBox : public Control
    {
        public:
            
            struct Caret {
                ScreenPos pos;
                int index;
            };
            
            TextBox(bool singleLine = true,
                    int height = 20,
                    std::function<bool()> visibleFunc = TRUE_FUNC)
            {
                this->singleLine = singleLine;
                this->visibleFunc = visibleFunc;
                this->height = height;
                editCaret.index = -1;
            }
            
            /* Element methods. */
            void draw() override;
            void mouseEvent() override;
            void keyEvent() override;
            
            Caret editCaret, clickCaret, selectStartCaret, selectEndCaret;
            bool singleLine;
            string text = "0000000000000000000\n0000000000000\n0000000";
            
        private:
            
            Caret caretAtPos(ScreenPos pos);
            Caret caretAtIndex(int index);
    };
    
}