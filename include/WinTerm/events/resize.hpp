#pragma once

namespace winTerm
{
	extern int columns , rows;

	void updateTerminalSize();
	void signalHandler(int);
}
