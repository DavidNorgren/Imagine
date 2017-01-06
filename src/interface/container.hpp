#pragma once

#include "interface/element.hpp"
#include "interface/tabs/tab.hpp"


namespace Imagine
{

    /* Represents a container with resizable and movable child elements. */
    class Container : public Element
    {
        public:
            enum InsertPosition
            {
                LEFT,
                RIGHT,
                TOP,
                BOTTOM,
                TAB
            };
            enum Layout
            {
                HORIZONTAL,
                VERTICAL,
                OTHER
            };
            enum SizeMode
            {
                FIXED,
                FILL
            };

            Container(Layout layout, std::vector<Container*> subContainers, SizeMode sizeMode = FILL)
            {
                this->layout = layout;
                this->subContainers = subContainers;
                this->sizeMode = sizeMode;
                insertOffset[LEFT] = insertOffset[RIGHT] = insertOffset[TOP] = insertOffset[BOTTOM] = -CONTAINER_INSERT_SIZE;
            }

            /* Adds a tab to the container. */
            virtual void addTab(Tab* tab, InsertPosition position = TAB, int index = -1);

            /* Add a subcontainer */
            void addSubContainer(Container* container, int index = -1);

            /* Remove a subcontainer. */
            void removeSubContainer(Container* container);

            /* Replaces a subcontainer. */
            void replaceSubContainer(Container* oldContainer, Container* newContainer);
                
            /* Element methods. */
            void update() override;
            void draw() override;
            void mouseEvent() override;
            void keyEvent() override;
            void setParent(Element* parent) override;
            
            Layout layout;
            std::vector<Container*> subContainers;

            SizeMode sizeMode;
            bool visible = true;
            int sizeFixed = CONTAINER_START_SIZE;
            float sizeFill = 1.f;

            ScreenArea insertBox[4];
            int insertOffset[4];
    };

}