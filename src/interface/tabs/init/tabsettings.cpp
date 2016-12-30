#include "interface/interfacehandler.hpp"


Imagine::Tab* Imagine::InterfaceHandler::createTabSettings()
{
    return
        new Tab("Settings", true, false, {
            new TabSection("Section A", true, {
                new CheckBox("Test")
            })
        });
}