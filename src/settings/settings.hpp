#pragma once

#include <base.hpp>

#define TRUE_FUNC   []() { return true; }
#define FALSE_FUNC  []() { return false; }

#define PANEL_START_WIDTH       380
#define PANEL_START_HEIGHT      210
#define PANEL_MIN_WIDTH         300
#define PANEL_MIN_HEIGHT        30
#define PANEL_RESIZE_BOX_SIZE   5

#define TAB_CONTENT_PADDING     8
#define TAB_SELECT_HEIGHT       30

#define CONTROL_MARGIN_RIGHT    2
#define CONTROL_MARGIN_LEFT     2
#define CONTROL_MARGIN_BOTTOM   8
#define SCROLLBAR_SIZE          12
#define SCROLLBAR_MIN_SIZE      5
#define TEXTBOX_BOX_PADDING     4

#define VIEW_HEADER_HEIGHT      24

#define SETTING_INTERFACE_COLOR_MAIN                Color(200, 200, 200)
#define SETTING_INTERFACE_COLOR_BACKGROUND          SETTING_INTERFACE_COLOR_MAIN * Color(0.9f)
#define SETTING_INTERFACE_COLOR_TEXT                Color(10, 10, 10)
#define SETTING_INTERFACE_COLOR_BUTTONS             Color(67,103,163)
#define SETTING_INTERFACE_COLOR_BUTTONS_PRESSED     SETTING_INTERFACE_COLOR_BUTTONS * Color(0.9f)
#define SETTING_INTERFACE_COLOR_BUTTONS_TEXT        COLOR_WHITE
#define SETTING_INTERFACE_COLOR_BOXES               COLOR_WHITE
#define SETTING_INTERFACE_COLOR_BOXES_PRESSED       SETTING_INTERFACE_COLOR_BOXES * Color(0.9f)
#define SETTING_INTERFACE_COLOR_BOXES_TEXT          SETTING_INTERFACE_COLOR_TEXT
#define SETTING_INTERFACE_COLOR_HIGHLIGHT           Color(132, 159, 204)
#define SETTING_INTERFACE_COLOR_HIGHLIGHT_TEXT      COLOR_WHITE