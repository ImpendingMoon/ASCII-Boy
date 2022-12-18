/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : app/program.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 17 Dec 2022
 EDITED : 17 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Handles the main program loop/state
 ******************************************************************************/

#pragma once

#include "../core.hpp"
#include "renderer.hpp"

enum ProgramState
{
    STOPPED, // Emulator is not running - at user prompts
    RUNNING, // Emulator is running
    PAUSED,  // Emulator is paused
    EXITING, // Program is preparing to exit
};

// Gets the current program state
ProgramState getState();

// Sets the current program state
void setState(ProgramState newState);

// Initializes the program with the renderer, config, and logger
void initProgram();

// Safely closes parts of the program that need cleanup
void exitProgram();

// Runs the main program loop, with a given starting state
void runProgram(ProgramState startingState);