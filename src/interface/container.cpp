#include "interface/container.hpp"
#include "interface/interfacehandler.hpp"
#include "imagineapp.hpp"

#include <iostream>


void Imagine::Container::update()
{
    // Calculate visible size of each subcontainer
    int totalSize = (layout == HORIZONTAL ? box.width : box.height);
    int fillSize = totalSize, firstVisible = subContainers.size(), lastVisible = 0;

    // Find total size
    int fillContainers = 0, visibleSubContainers = 0, i = 0;
    for (Container* container : subContainers)
    {
        if (container->visible)
        {
            if (container->sizeMode == FILL) {
                fillContainers++;
            }
            else {
                fillSize -= container->sizeFixed;
            }
            visibleSubContainers++;
            firstVisible = min(i, firstVisible);
            lastVisible = max(i, lastVisible);
        }
        i++;
    }

    // Define subcontainers
    ScreenPos currentPos = pos;
    i = 0;
    for (Container* container : subContainers)
    {
        if (container->visible)
        {
            // Calculate visible size
            int visibleSize;
            if (fillContainers == 0) {
                visibleSize = (totalSize * container->sizeFill) / visibleSubContainers;
            }
            else if (container->sizeMode == FILL) {
                visibleSize = (fillSize * container->sizeFill) / fillContainers;
            }
            else {
                visibleSize = container->sizeFixed;
            }

            // Set sizes and positions
            container->pos = currentPos;
            if (layout == HORIZONTAL)
            {
                container->box.width = visibleSize;
                container->box.height = box.height;
                currentPos.x += visibleSize;
            }
            else
            {
                container->box.width = box.width;
                container->box.height = visibleSize;
                currentPos.y += visibleSize;
            }

            // Set boundaries for inserting
            container->insertOffset[LEFT] = container->insertOffset[RIGHT] = container->insertOffset[TOP] = container->insertOffset[BOTTOM] = 0;

            if (i == firstVisible || layout != HORIZONTAL) {
                container->insertOffset[LEFT] = insertOffset[LEFT] + CONTAINER_INSERT_SIZE;
            }
            if (i == lastVisible || layout != HORIZONTAL) {
                container->insertOffset[RIGHT] = insertOffset[RIGHT] + CONTAINER_INSERT_SIZE;
            }
            if (i == firstVisible || layout != VERTICAL) {
                container->insertOffset[TOP] = insertOffset[TOP] + CONTAINER_INSERT_SIZE;
            }
            if (i == lastVisible || layout != VERTICAL) {
                container->insertOffset[BOTTOM] = insertOffset[BOTTOM] + CONTAINER_INSERT_SIZE;
            }

            container->insertBox[LEFT] = {
                container->pos + (ScreenPos){ container->insertOffset[LEFT], container->insertOffset[TOP] },
                CONTAINER_INSERT_SIZE, container->box.height - (container->insertOffset[TOP] + container->insertOffset[BOTTOM])
            };
            container->insertBox[RIGHT] = {
                container->pos + (ScreenPos){ container->box.width - container->insertOffset[RIGHT] - CONTAINER_INSERT_SIZE, container->insertOffset[TOP] },
                CONTAINER_INSERT_SIZE, container->box.height - (container->insertOffset[TOP] + container->insertOffset[BOTTOM])
            };
            container->insertBox[TOP] = {
                container->pos + (ScreenPos){ container->insertOffset[LEFT] + CONTAINER_INSERT_SIZE, container->insertOffset[TOP] },
                container->box.width - container->insertOffset[LEFT] - container->insertOffset[RIGHT] - CONTAINER_INSERT_SIZE * 2, CONTAINER_INSERT_SIZE
            };
            container->insertBox[BOTTOM] = {
                container->pos + (ScreenPos){ container->insertOffset[LEFT] + CONTAINER_INSERT_SIZE, container->box.height - container->insertOffset[BOTTOM] - CONTAINER_INSERT_SIZE },
                container->box.width - container->insertOffset[LEFT] - container->insertOffset[RIGHT] - CONTAINER_INSERT_SIZE * 2, CONTAINER_INSERT_SIZE
            };

            // Update subcontainer
            container->update();
        }
        i++;
    }
}


void Imagine::Container::draw()
{
    int i = 0;
    for (Container* container : subContainers)
    {
        if (container->visible)
        {
            container->draw();

            if (interface->moveContainer == this && interface->moveIndex == i)
            {
                switch (interface->moveContainerInsertPosition)
                {
                    case LEFT:
                    {
                        drawBox(
                            { container->pos, CONTAINER_INSERT_SIZE, container->box.height },
                            Color(COLOR_YELLOW, 0.25f)
                        );
                        break;
                    }
                    case RIGHT: {
                        drawBox(
                            { { container->pos.x + container->box.width - CONTAINER_INSERT_SIZE, container->pos.y },
                            CONTAINER_INSERT_SIZE, container->box.height },
                            Color(COLOR_YELLOW, 0.25f)
                        );
                        break;
                    }
                    case TOP: {
                        drawBox(
                            { container->pos, container->box.width, CONTAINER_INSERT_SIZE },
                            Color(COLOR_YELLOW, 0.25f)
                        );
                        break;
                    }
                    case BOTTOM: {
                        drawBox(
                            { { container->pos.x, container->pos.y + container->box.height - CONTAINER_INSERT_SIZE },
                            container->box.width, CONTAINER_INSERT_SIZE },
                            Color(COLOR_YELLOW, 0.25f)
                        );
                        break;
                    }
                }
            }
        }
        i++;
    }

    /*if (parent) {
        drawBox(insertBox[LEFT], Color(COLOR_YELLOW, 0.25f));
        drawText("left", insertBox[LEFT].pos, COLOR_WHITE);
        drawBox(insertBox[RIGHT], Color(COLOR_BLUE, 0.25f));
        drawText("right", insertBox[RIGHT].pos, COLOR_WHITE);
        drawBox(insertBox[TOP], Color(COLOR_GREEN, 0.25f));
        drawText("top", insertBox[TOP].pos, COLOR_WHITE);
        drawBox(insertBox[BOTTOM], Color(COLOR_RED, 0.25f));
        drawText("bottom", insertBox[BOTTOM].pos, COLOR_WHITE);
    }*/
}


void Imagine::Container::mouseEvent()
{
    if (isInterfaceState(TAB_MOVE))
    {
        int i = 0;
        for (Container* container : subContainers)
        {
            if (container->visible)
            {
                container->mouseEvent();
    
                // No existing tab selected
                if (!interface->moveContainer && mouseInBox(container->box))
                {
                    if (mouseInBox(container->insertBox[LEFT])) {
                        interface->moveContainerInsertPosition = LEFT;
                        interface->moveContainer = this;
                        interface->moveIndex = i;
                    }
                    else if (mouseInBox(container->insertBox[RIGHT])) {
                        interface->moveContainerInsertPosition = RIGHT;
                        interface->moveContainer = this;
                        interface->moveIndex = i;
                    }
                    else if (mouseInBox(container->insertBox[TOP])) {
                        interface->moveContainerInsertPosition = TOP;
                        interface->moveContainer = this;
                        interface->moveIndex = i;
                    }
                    else if (mouseInBox(container->insertBox[BOTTOM])) {
                        interface->moveContainerInsertPosition = BOTTOM;
                        interface->moveContainer = this;
                        interface->moveIndex = i;
                    }
                }
            }
            i++;
        }
    }
    else
    {
        mouseOn = (parent ? parent->mouseOn && mouseInBox(box) : mouseInBox(box));
        
        for (Container* container : subContainers)
        {
            if (container->visible) {
                container->mouseEvent();
            }
        }
    }
}


void Imagine::Container::keyEvent()
{
    for (Container* container : subContainers)
    {
        if (container->visible) {
            container->keyEvent();
        }
    }
}


void Imagine::Container::setParent(Element* parent)
{
    this->parent = parent;
    for (Container* container : subContainers) {
        container->setParent(this);
    }
}


void Imagine::Container::addTab(Tab* tab, InsertPosition position, int index)
{
    TabCollection* newTabCollection = new TabCollection({ (Tab*)getFocused() });
    Container* container = subContainers[index];

    if (layout == HORIZONTAL)
    {
        if (position == LEFT) {
            addSubContainer(newTabCollection, index);
        }
        else if (position == RIGHT) {
            addSubContainer(newTabCollection, index + 1);
        }
        else if (position == TOP) {
            replaceSubContainer(container, new Container(VERTICAL, { newTabCollection, container }));
        }
        else if (position == BOTTOM) {
            replaceSubContainer(container, new Container(VERTICAL, { container, newTabCollection }));
        }
    }
    else
    {
        if (position == LEFT) {
            replaceSubContainer(container, new Container(HORIZONTAL, { newTabCollection, container }));
        }
        else if (position == RIGHT) {
            replaceSubContainer(container, new Container(HORIZONTAL, { container, newTabCollection }));
        }
        else if (position == TOP) {
            addSubContainer(newTabCollection, index);
        }
        else if (position == BOTTOM) {
            addSubContainer(newTabCollection, index + 1);
        }
    }
}


void Imagine::Container::addSubContainer(Container* container, int index)
{
    // Append
    if (index < 0) {
        index = subContainers.size();
    }

    vectorInsert(subContainers, index, container);

    setParent(parent);
    update();
}


void Imagine::Container::removeSubContainer(Container* container)
{
    vectorErase(subContainers, container);
    delete container;

    if (parent)
    {
        if (subContainers.size() == 1)
        {
            ((Container*)parent)->replaceSubContainer(this, subContainers[0]);
            delete this;
            return;
        }
        if (subContainers.size() == 0) {
            ((Container*)parent)->removeSubContainer(this);
            return;
        }
    }

    setParent(parent);
    update();
}


void Imagine::Container::replaceSubContainer(Container* oldContainer, Container* newContainer)
{
    int index = vectorFind(subContainers, oldContainer);
    subContainers[index] = newContainer;
    newContainer->sizeMode = oldContainer->sizeMode;
    newContainer->sizeFixed = oldContainer->sizeFixed;
    newContainer->sizeFill = oldContainer->sizeFill;

    setParent(parent);
    update();
}