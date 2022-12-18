/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : app/program.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 17 Dec 2022
 EDITED : 18 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Handles the main program loop/state
 ******************************************************************************/

#include "program.hpp"

using std::unique_ptr, std::make_unique;

ProgramState programState = MENU_MAIN;

unique_ptr<GBSystem> gb{};

MenuMain menuMain{};

// Gets the current program state
ProgramState getState()
{
    return programState;
}



// Sets the current program state
void setState(ProgramState newState)
{
    programState = newState;
}



// Initializes the program with the renderer, config, and logger
void initProgram()
{
    Logger::initLogger();
    initRenderer();
    Logger::log("Program starting.", Logger::VERBOSE);

    // Start at main menu
    menuMain.initMenu();

    render();
}



// Safely closes parts of the program that need cleanup
void exitProgram()
{
    Logger::log("Program exiting.", Logger::VERBOSE);
    exitRenderer();
    Logger::exitLogger();
}



// Runs the main program loop, with a given starting state
void runProgram(ProgramState startingState)
{
    Logger::log("Main loop running.", Logger::VERBOSE);

    programState = startingState;

    while(programState != EXITING)
    {
        // TODO: Process input

        // Finite State Machines ftw
        switch(programState)
        {
            case MENU_MAIN:
            {
                break;
            } // End Menu_Main

            case MENU_ROM:
            {
                break;
            } // End Menu_ROM

            case MENU_CONF:
            {
                break;
            } // End Menu_Conf

            case RUNNING:
            {
                break;
            } // End Running

            case PAUSED:
            {
                break;
            } // End Paused

            case EXITING:
            {
                Logger::log("Program exiting main loop.", Logger::VERBOSE);
                break;
            } // End Exiting
        } // End Switch
    
    } // End Loop

}