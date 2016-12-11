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
                int index, indexWrap;
            };
            
            TextBox(string name,
                    int lines = 1,
                    std::function<bool()> visibleFunc = TRUE_FUNC)
            {
                this->name = name;
                this->lines = lines;
                this->visibleFunc = visibleFunc;
                editCaret.index = -1;
            }
            
            /* Element methods. */
            void update() override;
            void draw() override;
            void mouseEvent() override;
            void keyEvent() override;
            
            void setText(string text);
            
        private:
            Caret editCaret, clickCaret, selectStartCaret, selectEndCaret;
            int lines;
            string text = "", textWrap = "";
            
            void updateWrap();
            Caret caretAtPos(ScreenPos pos);
            Caret caretAtIndex(int index);
            Caret caretAtIndexWrap(int indexWrap);
            int findNextWord(int wrapIndex);
            int findPreviousWord(int wrapIndex);
    };
    
}