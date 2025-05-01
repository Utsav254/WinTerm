#include "winTerm.hpp"
#include <termios.h>
#include <unistd.h>

namespace winTerm {
	int _columns , _rows; // size of full terminal window

	static struct termios _origTermios;
	static char *_oldLocale;

}

int winTerm::initialise()
{
	// get current terminal settings
	tcgetattr(STDIN_FILENO , &_origTermios);

	// enable new terminal buffer
	constexpr char alternateBufferEsc[9] = "\x1b[?1049h";
	write(STDOUT_FILENO , alternateBufferEsc , sizeof(alternateBufferEsc));
		
	//set new terminal settings
	struct termios rawTermios;
	tcgetattr(STDIN_FILENO , &rawTermios);
	rawTermios.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	rawTermios.c_oflag &= ~(OPOST);
	rawTermios.c_cflag |= (CS8);
	rawTermios.c_lflag &= ~(ECHO | ICANON | IEXTEN);
	tcsetattr(STDIN_FILENO , TCSAFLUSH , &rawTermios);

	_oldLocale = std::setlocale(LC_ALL, "");


	constexpr char moveCursorHome[4] = "\x1b[H";
	write(STDOUT_FILENO , moveCursorHome , sizeof(moveCursorHome));

	return 0;
}

int winTerm::destroy()
{
	std::setlocale(LC_ALL, _oldLocale);
	
	// restore original terminal settings
	tcsetattr(STDIN_FILENO , TCSAFLUSH , &_origTermios);

#ifdef _DEBUG
	char c;
	read(STDIN_FILENO, &c, 1);
#endif

	// restore original terminal buffer
	constexpr char restoreBufferEsc[9] = "\x1b[?1049l";
	write(STDOUT_FILENO , restoreBufferEsc , sizeof(restoreBufferEsc));

	return 0;
}
