#pragma once

#include <base.hpp>

#include "settings/settings.hpp"


namespace Imagine
{

    /* An element of the interface. Defines event and drawing functions. */
    class Element
    {
        public:
            /* Updates the positions and sizes of the child elements */
            virtual void update() = 0;

            /* Render the element on the screen, using the position and size. */
            virtual void draw() = 0;

            /* Handles user input. */
            virtual void mouseEvent() = 0;
            virtual void keyEvent() = 0;

            /* Sets the parent element and continues with the child elements (if any). */
            virtual void setParent(Element* parent);

            /* Focus. */
            void focus();
            bool isFocused();

            /* A box that represents the element's screen presence. */
            ScreenArea box;
            ScreenPos& pos = box.pos;
            
            /* Whether the mouse is on the element. */
            bool mouseOn = false; 

            /* The parent element. */
            Element* parent;
    };

}