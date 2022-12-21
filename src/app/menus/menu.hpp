/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : app/menus/menu.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 18 Dec 2022
 EDITED : 21 Dec 2022
 ******************************************************************************/

/******************************************************************************
 The base class for all menus.
 ******************************************************************************/

#pragma once

#include "../../core.hpp"
#include "../renderer.hpp"
#include "comp/button.hpp"

class Menu
{
public:
    // Initializes and renders a menu
    virtual void initMenu() = 0;
    // Renders a menu
    virtual void renderMenu() = 0;
    // Exits a menu by clearing the screen
    void exitMenu();
    // Switches to another menu
    void gotoMenu(Menu& menu);

protected:

    point cursor_pos;
    // List of buttons on the screen
    std::vector<Button> buttons;
    // Currently hovered button
    uint32_t at_button;

    // Creates a button and adds it to the vector of buttons
    void createButton(point position,
                      std::string active_display,
                      std::string inactive_display,
                      bool enabled,
                      std::function<void()> onClick
                     );
    // Moves the cursor to the next valid position
    // NOTE: Expects all buttons to be laid out in order
    void moveCursor(uint32_t key);
    // Clicks on the currently hovered button
    void clickButton();
    // Puts the terminal into cooked mode and gets a string from the user
    std::string getString();
};