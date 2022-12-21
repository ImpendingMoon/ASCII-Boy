/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : app/menus/menumain.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 18 Dec 2022
 EDITED : 21 Dec 2022
 ******************************************************************************/

/******************************************************************************
 The main starting menu for the program. Allows the user to load a ROM,
 go to options, or exit.
 ******************************************************************************/

#pragma once

#include "../../core.hpp"
#include "menu.hpp"

class MenuMain : Menu
{
public:

    MenuMain();
    virtual ~MenuMain();

    // Initializes and renders the menu
    void initMenu() override;
    // Renders a menu
    void renderMenu() override;

private:

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

    // Renders the logo specified in the array above
    void renderLogo();
};