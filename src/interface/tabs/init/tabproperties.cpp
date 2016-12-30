#include "interface/interfacehandler.hpp"

#include <iostream>


Imagine::Tab* Imagine::InterfaceHandler::createTabProperties()
{
    return
        new Tab("Project properties", true, false, {
            new TabSection("Section A", true, {
                new LabeledGroup({
                    new TextField("Name"),
                    new TextField("Author"),
                    new TextField("Description", 4)
                })
            }),
            new TabSection("Section B", true, {
                new Row({
                    new CheckBox("Hello"),
                    new CheckBox("Hello2"),
                    new CheckBox("Hello3"),
                    new CheckBox("Hello4"),
                    new CheckBox("Hello5")
                }),
                new Button(
                    "Click Me!", [](){ std::cout << "Hello!" << std::endl; }, SAVE_PROJECT
                ),
                new Row({
                    new Button("Hello"),
                    new Button("Hello2"),
                    new Button("Hello3")
                })
            })
        });
}