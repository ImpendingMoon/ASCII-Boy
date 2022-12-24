/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : app/program.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 17 Dec 2022
 EDITED : 23 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Handles the main program loop/state
 ******************************************************************************/

#include "program.hpp"

using std::unique_ptr, std::make_unique;

ProgramState programState = MENU;

unique_ptr<GBSystem> gb{};


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
    Logger::log("Program starting.", Logger::VERBOSE);
}



// Safely closes parts of the program that need cleanup
void exitProgram()
{
    Logger::log("Program exiting.", Logger::VERBOSE);
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
            case MENU:
            {
                break;
            } // End Menu

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
                exitProgram();
                break;
            } // End Exiting
        } // End Switch

        // TODO: Render

    } // End Loop

}
