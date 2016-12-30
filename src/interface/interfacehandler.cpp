#include "imagineapp.hpp"


Imagine::InterfaceHandler::InterfaceHandler()
{
    // Setup interface
    rootContainer =
        new Container(Container::HORIZONTAL, {
            new Container(Container::VERTICAL, {
                new Container(Container::HORIZONTAL, {
                    new View(),
                    new TabCollection({
                        createTabSettings()
                    }, PANEL_START_WIDTH)
                }),
                new TabCollection({
                    createTabTimeline()
                }, PANEL_START_HEIGHT)
            }),
            new TabCollection({
                createTabProperties()
            }, PANEL_START_WIDTH)
        });
    
    // This is the root
    setParent(nullptr);
}


void Imagine::InterfaceHandler::update()
{
    rootContainer->box = { { 0, 0 }, app->mainWindow->width, app->mainWindow->height };
    rootContainer->update();
}


void Imagine::InterfaceHandler::draw()
{
    drawBegin();
    rootContainer->draw();
}


void Imagine::InterfaceHandler::mouseEvent()
{
    if (isInterfaceState(IDLE) && mouseLeftPressed()) {
        focus = nullptr;
    }
    rootContainer->mouseEvent();
}


void Imagine::InterfaceHandler::keyEvent()
{
    rootContainer->keyEvent();
}


void Imagine::InterfaceHandler::setParent(Element* parent)
{
    this->parent = parent;
    rootContainer->setParent(this);
}


void Imagine::setInterfaceState(InterfaceState state)
{
    app->interfaceHandler->state = state;
}


Imagine::InterfaceState Imagine::getInterfaceState()
{
    return app->interfaceHandler->state;
}


bool Imagine::isInterfaceState(InterfaceState state)
{
    return getInterfaceState() == state;
}


void Imagine::setFocused(Element* element)
{
    app->interfaceHandler->focus = element;
}


Imagine::Element* Imagine::getFocused()
{
    return app->interfaceHandler->focus;
}


bool Imagine::isFocused(Element* element)
{
    return getFocused() == element;
}