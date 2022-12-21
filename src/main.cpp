/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : main.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 21 Dec 2022
 ******************************************************************************/

/******************************************************************************
 The main entrypoint for ASCII-Boy.
 ******************************************************************************/

#include "main.hpp"

int main()
{
// Handle exit signals
#ifdef _WIN32
    // NOTE: This doesn't seem to work terribly well.
    SetConsoleCtrlHandler(reinterpret_cast<PHANDLER_ROUTINE>(exitHandler), TRUE);
#else
    signal(SIGINT, exitHandler);
#endif

    initProgram();

    runProgram(MENU);

    exitProgram();

	exit(0);
}


// Safely exits the program when an exit signal is called
void exitHandler(int signal)
{
    Logger::log("ASCII-Boy exited with code " + signal,
                Logger::VERBOSE);

    exitProgram();

    exit(0);
}