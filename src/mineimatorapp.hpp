#pragma once

#include "base.hpp"
using namespace Base;

#include "interface/interfacehandler.hpp"
#include "project/projecthandler.hpp"


namespace Mineimator
{

    /* Contains the methods and variables for Mine-imator. */
    class App : public AppHandler
    {
        public:

            /* Program startup. */
            App();

            //// Handlers ////

            InterfaceHandler* interfaceHandler;
            ProjectHandler* projectHandler;

        private:

            /* Called by the main window once per frame. */
            void loopEvent() override;
            
            /* Called when the mouse is moved or clicked. */
            void mouseEvent(MouseEvent* event) override;
            
            /* Called when a key is pressed. */
            void keyEvent(KeyEvent* event) override;

            /* Called by the main window when resized. */
            void resizeEvent() override;

            /* Default resource*/
            //Resource* resMinecraft;
    };

}

/* Used to access the program from other class scopes. */
extern Mineimator::App* app;