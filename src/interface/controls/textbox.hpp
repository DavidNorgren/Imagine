#pragma once

#include "interface/controls/control.hpp"


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
                editCaret.index = selectStartCaret.index = selectEndCaret.index = 0;
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