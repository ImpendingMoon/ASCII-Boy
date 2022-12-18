/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : app/program.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 17 Dec 2022
 EDITED : 17 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Handles the main program loop/state
 ******************************************************************************/

#include "program.hpp"

ProgramState programState = STOPPED;


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

}

// Safely closes parts of the program that need cleanup
void exitProgram()
{

}

// Runs the main program loop, with a given starting state
void runProgram(ProgramState startingState)
{

}