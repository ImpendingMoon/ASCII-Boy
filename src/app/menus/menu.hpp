/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : app/menus/menu.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 18 Dec 2022
 EDITED : 18 Dec 2022
 ******************************************************************************/

/******************************************************************************
 The base abstract class for all menus.
 ******************************************************************************/

#pragma once

#include "../../core.hpp"
#include "../renderer.hpp"

class Menu
{
public:
    // Initializes and renders a menu
    virtual void initMenu() = 0;
    // Exits a menu
    virtual void exitMenu() = 0;
    // Switches to another menu
    virtual void gotoMenu(Menu& menu) = 0;

private:

    // Used to store cursor position and button positions
    struct point
    {
        uint32_t x;
        uint32_t y;
    };

    // Interactive button
    struct button
    {
        point position = {0,0};
        // Option to disable the button
        bool enabled = true;
        // What is displayed on the screen
        std::string display = "Button";
        // Function to be run on interaction
        std::function<void()> on_click;
    };

    point cursor_pos;
    std::vector<button> buttons;

    // Moves the cursor to the next valid position
    virtual void moveCursor() = 0;
    // Puts the terminal into cooked mode and gets a string from the user
    virtual std::string getString() = 0;
};