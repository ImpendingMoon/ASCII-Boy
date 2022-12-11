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

int main()
{
	Logger::instance().log("ASCII-Boy Starting.", Logger::VERBOSE);

	// Debug Stuff. Dump your own ROMs, kids.
	auto gb = std::make_unique<GBSystem>("./roms/Tetris.gb");

	// For now, walk through 100 instructions
	for(int i = 0; i < 100; i++)
	{
		gb->step();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	Logger::instance().log(gb->mem.dumpMemory(), Logger::DEBUG);

	Logger::instance().log("ASCII-Boy Exiting.", Logger::VERBOSE);

	exit(0);
}
