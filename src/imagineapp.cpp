#include <iostream>

#include "imagineapp.hpp"


int main()
{ 
    new Imagine::App();
    return 0;
}


Imagine::App* app;
Imagine::InterfaceHandler* interface;

Imagine::App::App()
{
    app = this;
    interface = interfaceHandler = new InterfaceHandler();
    mainWindow->setTitle("Imagine");
    launch();
}


void Imagine::App::loopEvent()
{
    interfaceHandler->draw();
}


void Imagine::App::mouseEvent()
{
    interfaceHandler->mouseEvent();
}


void Imagine::App::keyEvent()
{
    interfaceHandler->keyEvent();
}


void Imagine::App::resizeEvent()
{
    interfaceHandler->update();
}