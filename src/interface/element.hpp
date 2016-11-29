#pragma once

#include "base.hpp"
using namespace Base;


/* An element of the interface. Defines event and drawing functions. */
class Element
{
    public:
        
        /* Updates the positions and sizes of the child elements */
        virtual void update() = 0;
        
        /* Render the element on the screen, using the position and size. */
        virtual void draw() = 0;
        
        /* Handles user input. */
        void mouseEvent(MouseEvent* event)  {};
        void keyEvent(KeyEvent* event)  {};
        
        /* Sets the parent element and continues with the child elements. */
        virtual void setParent(Element* parent) = 0;
        
        /* A box that represents the element's screen presence. */
        ScreenArea box;
        ScreenPos& pos = box.pos;
        
        /* The parent element. */
        Element* parent;
};