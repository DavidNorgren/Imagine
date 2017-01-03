#include "interface/container.hpp"
#include "interface/interfacehandler.hpp"

#include <iostream>


void Imagine::Container::update()
{
    // Calculate visible size of each subcontainer
    int totalSize = (layout == HORIZONTAL ? box.width : box.height);
    int fillSize = totalSize;

    // Find total size
    int fillContainers = 0, visibleSubContainers = 0;
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
        }
    }

    std::cout << stringRepeat("\t", level) << "container, layout=" << layout << ", fillContainers = " << fillContainers << std::endl;

    // Define subcontainers
    ScreenPos currentPos = pos;
    for (int i = 0; i < subContainers.size(); i++)
    {
        Container* container = subContainers[i];
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
            container->insertLeftSize = container->insertRightSize = container->insertTopSize = container->insertBottomSize = CONTAINER_INSERT_SIZE;

            if (i == 0 || layout != HORIZONTAL) {
                container->insertLeftSize += insertLeftSize;
            }
            if (i == subContainers.size() - 1 || layout != HORIZONTAL) {
                container->insertRightSize += insertRightSize;
            }
            if (i == 0 || layout != VERTICAL) {
                container->insertTopSize += insertTopSize;
            }
            if (i == subContainers.size() - 1 || layout != VERTICAL) {
                container->insertBottomSize += insertBottomSize;
            }

            container->insertLeftBox = {
                container->pos + (ScreenPos){ container->insertLeftSize - CONTAINER_INSERT_SIZE, container->insertTopSize - CONTAINER_INSERT_SIZE },
                CONTAINER_INSERT_SIZE, container->box.height - (container->insertTopSize + container->insertBottomSize - CONTAINER_INSERT_SIZE * 2)
            };
            container->insertRightBox = {
                container->pos + (ScreenPos){ container->box.width - container->insertRightSize, container->insertTopSize - CONTAINER_INSERT_SIZE },
                CONTAINER_INSERT_SIZE, container->box.height - (container->insertTopSize + container->insertBottomSize - CONTAINER_INSERT_SIZE * 2)
            };
            container->insertTopBox = {
                container->pos + (ScreenPos){ container->insertLeftSize, container->insertTopSize - CONTAINER_INSERT_SIZE },
                container->box.width - container->insertLeftSize - container->insertRightSize, CONTAINER_INSERT_SIZE
            };
            container->insertBottomBox = {
                container->pos + (ScreenPos){ container->insertLeftSize, container->box.height - container->insertBottomSize },
                container->box.width - container->insertLeftSize - container->insertRightSize, CONTAINER_INSERT_SIZE
            };

            // Update subcontainer
            container->update();
        }
    }
}


void Imagine::Container::draw()
{
    for (Container* container : subContainers)
    {
        if (container->visible) {
            container->draw();
        }
    }

    if (parent) {
        drawBox(insertLeftBox, Color(COLOR_YELLOW, 0.25f));
        drawText("left", insertLeftBox.pos, COLOR_WHITE);
        drawBox(insertRightBox, Color(COLOR_BLUE, 0.25f));
        drawText("right", insertRightBox.pos, COLOR_WHITE);
        drawBox(insertTopBox, Color(COLOR_GREEN, 0.25f));
        drawText("top", insertTopBox.pos, COLOR_WHITE);
        drawBox(insertBottomBox, Color(COLOR_RED, 0.25f));
        drawText("bottom", insertBottomBox.pos, COLOR_WHITE);
    }
}


void Imagine::Container::mouseEvent()
{
    if (isInterfaceState(TAB_MOVE))
    {
        // Drop the tab
        if (!mouseLeftDown())
        {
            for (int i = 0; i < subContainers.size(); i++)
            {
                Container* container = subContainers[i];

                if (mouseInBox(container->insertLeftBox) ||
                    mouseInBox(container->insertRightBox) ||
                    mouseInBox(container->insertTopBox) ||
                    mouseInBox(container->insertBottomBox))
                {
                    TabCollection* newTabCollection = new TabCollection({ (Tab*)getFocused() });

                    if (layout == HORIZONTAL)
                    {
                        if (mouseInBox(container->insertLeftBox)) {
                            addSubContainer(newTabCollection, i);
                        }
                        else if (mouseInBox(container->insertRightBox)) {
                            addSubContainer(newTabCollection, i + 1);
                        }
                        else if (mouseInBox(container->insertTopBox)) {
                            replaceSubContainer(container, new Container(VERTICAL, { newTabCollection, container }));
                        }
                        else if (mouseInBox(container->insertBottomBox)) {
                            replaceSubContainer(container, new Container(VERTICAL, { container, newTabCollection }));
                        }
                    }
                    else
                    {
                        if (mouseInBox(container->insertLeftBox)) {
                            replaceSubContainer(container, new Container(HORIZONTAL, { newTabCollection, container }));
                        }
                        else if (mouseInBox(container->insertRightBox)) {
                            replaceSubContainer(container, new Container(HORIZONTAL, { container, newTabCollection }));
                        }
                        else if (mouseInBox(container->insertTopBox)) {
                            addSubContainer(newTabCollection, i);
                        }
                        else if (mouseInBox(container->insertBottomBox)) {
                            addSubContainer(newTabCollection, i + 1);
                        }
                    }
                    update();
                    setInterfaceState(IDLE);
                    break;
                }
                else {
                    container->mouseEvent();
                }
            }
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
        container->level = level + 1;
        container->setParent(this);
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
}


void Imagine::Container::removeSubContainer(Container* container)
{
    vectorErase(subContainers, container);
    delete container;

    if (parent)
    {
        if (subContainers.size() == 1) {
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