/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : app/menus/menumain.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 18 Dec 2022
 EDITED : 18 Dec 2022
 ******************************************************************************/

/******************************************************************************
 The main starting menu for the program. Allows the user to load a ROM,
 go to options, or exit.
 ******************************************************************************/

#pragma once

#include "menu.hpp"

class MenuMain : Menu
{
public:

    MenuMain();
    virtual ~MenuMain();

    // Initializes and renders the menu
    void initMenu() override;
    // Exits the menu
    void exitMenu() override;
    // Goes to another menu
    void gotoMenu(Menu& menu) override;

private:

    // Moves the cursor to the next valid position
    void moveCursor() override;
    // Puts the terminal into cooked move and gets a string from the user
    std::string getString() override;

    // The logo broken down into lines of strings
    std::array<std::string, 17> logo
    {
        "  /$$$$$$   /$$$$$$   /$$$$$$  /$$$$$$ /$$$$$$",
        " /$$__  $$ /$$__  $$ /$$__  $$|_  $$_/|_  $$_/",
        "| $$  \\ $$| $$  \\__/| $$  \\__/  | $$    | $$  ",
        "| $$$$$$$$|  $$$$$$ | $$        | $$    | $$  ",
        "| $$__  $$ \\____  $$| $$        | $$    | $$  ",
        "| $$  | $$ /$$  \\ $$| $$    $$  | $$    | $$  ",
        "| $$  | $$|  $$$$$$/|  $$$$$$/ /$$$$$$ /$$$$$$",
        "|__/  |__/ \\______/  \\______/ |______/|______/",
        "",
        "______               ",
        "| ___ \\              ",
        "| |_/ /  ___   _   _ ",
        "| ___ \\ / _ \\ | | | |",
        "| |_/ /| (_) || |_| |",
        "\\____/  \\___/  \\__, |",
        "                __/ |",
        "               |___/ ",
    };
};