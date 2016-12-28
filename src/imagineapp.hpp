#pragma once

#include <base.hpp>

#include "interface/interfacehandler.hpp"
#include "project/projecthandler.hpp"


namespace Imagine
{

    /* Contains the methods and variables for the application. */
    class App : public AppHandler
    {
        public:
            /* Program startup. */
            App();

            /* Handlers. */
            InterfaceHandler* interfaceHandler;
            ProjectHandler* projectHandler;

        private:
            /* Called by the main window once per frame. */
            void loopEvent() override;
            
            /* Called when the mouse is moved or clicked. */
            void mouseEvent() override;
            
            /* Called when a key is pressed. */
            void keyEvent() override;

            /* Called by the main window when resized. */
            void resizeEvent() override;
    };

}

/* Used to access the program from other class scopes. */
extern Imagine::App* app;