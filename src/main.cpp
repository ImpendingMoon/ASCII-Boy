/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : main.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 9 Dec 2022
 ******************************************************************************/

/******************************************************************************
 The main entrypoint for ASCII-Boy.
 ******************************************************************************/

#include "main.hpp"

int main()
{
	Logger::instance().log("ASCII-Boy Starting.", Logger::VERBOSE);

	// Debug Stuff. Dump your own ROMs, kids.
	GBSystem* gb = new GBSystem("./roms/Tetris.gb");

	Logger::instance().log(gb->mem.dumpMemory(), Logger::DEBUG);

	Logger::instance().log("ASCII-Boy Exiting.", Logger::VERBOSE);

	delete gb;

	return 0;
}
