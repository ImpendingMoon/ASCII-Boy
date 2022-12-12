/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : main.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 11 Dec 2022
 ******************************************************************************/

/******************************************************************************
 The main entrypoint for ASCII-Boy.
 ******************************************************************************/

#include "main.hpp"

// Debug Stuff. Dump your own ROMs, kids.
auto gb = std::make_unique<GBSystem>("./roms/Tetris.gb");

int main()
{
// Handle exit signals
#ifdef _WIN32
    // NOTE: This doesn't seem to work terribly well.
    SetConsoleCtrlHandler(reinterpret_cast<PHANDLER_ROUTINE>(exitHandler), TRUE);
#else
    signal(SIGINT, exitHandler);
#endif

    programState = RUNNING;

	Logger::instance().log("ASCII-Boy Started.", Logger::VERBOSE);

    using std::this_thread::sleep_for;
    using std::chrono::milliseconds;

    while(programState != EXITING)
    {
        // TODO: Input handling

        switch(programState)
        {
        case RUNNING:
        {
            int cycles_per_frame = gb->getCyclesPerFrame();

            for(int i = 0; i < cycles_per_frame; i++)
            {
                try {
                    gb->step();
                    sleep_for(milliseconds(100));

                } catch(std::invalid_argument& ex) {

                    Logger::instance().log(ex.what(), Logger::ERRORS);
                }
            }

            break;
        }

        case PAUSED:
        {
            sleep_for(milliseconds(1));
            break;
        }

        case STOPPED:
        {
            // Destroy the GBSystem and go to prompts/menu
            gb.reset();

            // TODO: Get user prompts
        }

        case EXITING:
        {
            break;
        }

        } // End Switch

        // TODO: Rendering
    }

	exit(0);
}


// Safely exits the program when an exit signal is called
void exitHandler(int signal)
{
    Logger::instance().log(gb->mem.dumpMemory(), Logger::DEBUG);

    Logger::instance().log("ASCII-Boy exited with code " + signal,
                           Logger::VERBOSE);

    exit(0);
}