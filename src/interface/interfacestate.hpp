#pragma once


namespace Mineimator
{
    
    /* Defines the possible actions by the user. Only
       one of these can be processed at once. */
    enum InterfaceState
    {
        IDLE,
        TEXTBOX_SELECT,
        TAB_CLICK,
        TAB_MOVE,
        PANEL_RESIZE,
        SCROLLBAR_MOVE
    };
    
}