#include "imagineapp.hpp"


Imagine::InterfaceHandler::InterfaceHandler()
{
    workspace = new Workspace();
    
    // This is the root
    setParent(nullptr);
}


void Imagine::InterfaceHandler::update()
{
    box = { { 0, 0 }, app->mainWindow->width, app->mainWindow->height };
    workspace->box = box;
    workspace->update();
}


void Imagine::InterfaceHandler::draw()
{
    drawBegin();
    workspace->draw();
}


void Imagine::InterfaceHandler::mouseEvent()
{
    if (isInterfaceState(IDLE) && mouseLeftPressed()) {
        focus = nullptr;
    }
    workspace->mouseEvent();
}


void Imagine::InterfaceHandler::keyEvent()
{
    workspace->keyEvent();
}


void Imagine::InterfaceHandler::setParent(Element* parent)
{
    this->parent = parent;
    workspace->setParent(this);
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