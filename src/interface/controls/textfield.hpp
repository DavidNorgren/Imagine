#pragma once

#include "interface/controls/control.hpp"
#include "interface/controls/scrollbar.hpp"


namespace Mineimator
{
    
    /* Editable textbox. */
    class TextField : public Control
    {
        public:
            struct Caret {
                ScreenPos pos;
                int index, indexWrap;
            };
            
            TextField(string name,
                      int lines = 1,
                      std::function<bool()> visibleFunc = TRUE_FUNC);
            
            /* Element methods. */
            void update() override;
            void draw() override;
            void mouseEvent() override;
            void keyEvent() override;
            void setParent(Element* parent);
            
            void setText(string text);
            
        private:
            Caret editCaret, clickCaret, selectStartCaret, selectEndCaret;
            double clickTime;
            int lines;
            string text = "", textWrap = "";
            ScreenArea textBox;
            ScreenPos textOffset = { 0, 0 };
            ScrollBar* scrollBar = nullptr;
            
            void updateWrap();
            void updateOffset();
            Caret caretAtPos(ScreenPos pos);
            Caret caretAtIndex(int index);
            Caret caretAtIndexWrap(int indexWrap);
            int findNextWord(int wrapIndex);
            int findPreviousWord(int wrapIndex);
    };
    
}