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

#include "menu.hpp"


// Exits a menu by clearing the screen
void Menu::exitMenu()
{
    // Reset cursor position
    cursor_pos = {0, 0};
    wmove(stdscr, cursor_pos.y, cursor_pos.x);

    // Clear window
    wclear(stdscr);
}


// Switches to another menu
void Menu::gotoMenu(Menu& menu)
{
    this->exitMenu();
    menu.initMenu();
}



// Creates a button and adds it to the vector of buttons
void Menu::createButton(point position,
                    std::string active_display,
                    std::string inactive_display,
                    bool enabled,
                    std::function<void()> onClick
                    )
{
    Button temp;
    temp.setPosition(position);
    temp.setActiveDisplay(active_display);
    temp.setInactiveDisplay(inactive_display);
    temp.setEnabled(enabled);
    temp.setOnClick(onClick);

    // If the first button on-screen, set active
    temp.setActive(buttons.empty());

    // Push button onto vector
    buttons.push_back(temp);
}




// Moves the cursor to the next valid position
// NOTE: Expects all buttons to be laid out in order
void Menu::moveCursor(uint32_t key)
{
    // If there are no buttons, go to origin
    if(buttons.empty())
    {
        cursor_pos = {0,0};
        wmove(stdscr, cursor_pos.y, cursor_pos.x);
        return;
    }

    // De-activate previous button
    buttons.at(at_button).setActive(false);

    switch(key)
    {
    case KEY_UP:
    {
        at_button--;
        break;
    }

    case KEY_DOWN:
    {
        at_button++;
        break;
    }
    }

    // Wrap to ensure in-bounds
    at_button = emath::wrap(at_button, 0, buttons.size() - 1);

    // Set cursor position to new button
    cursor_pos = buttons.at(at_button).getPosition();
    wmove(stdscr, cursor_pos.y, cursor_pos.x);

    // Activate new button
    buttons.at(at_button).setActive(true);
}



// Clicks on the currently hovered button
void Menu::clickButton()
{
    if(buttons.empty())
    {
        return;
    }

    if(!buttons.at(at_button).isEnabled())
    {
        return;
    }

    buttons.at(at_button).click();

}



// Puts the terminal into cooked mode and gets a string from the user
std::string Menu::getString()
{
    // getstr() exists as a function, but requires a C-Style string instead
    // of an std::string.

    // Put the terminal back into cooked mode
    nocbreak();
    echo();

    // Get a string from the user until we hit a newline character
    std::string input{};
    int character = getchar();

    while(character != '\n' || character != '\r')
    {
        input.push_back(character);
        character = getchar();
    }

    return input;
    
}