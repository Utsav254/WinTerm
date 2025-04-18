#include "WinTerm/winTerm.hpp"
#include <termios.h>
#include <unistd.h>

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
	rawTermios.c_cc[VMIN] = 0;
	rawTermios.c_cc[VTIME] = 1;
	tcsetattr(STDIN_FILENO , TCSAFLUSH , &rawTermios);

	_oldLocale = std::setlocale(LC_ALL, "");


	constexpr char moveCursorHome[4] = "\x1b[H";
	write(STDOUT_FILENO , moveCursorHome , sizeof(moveCursorHome));
		
	_renderThread = std::thread(&winTerm::renderCanvas);
	_stdinReaderThread = std::thread(&winTerm::stdinReader);

	return 0;
}

int winTerm::destroy()
{
	if(_stdinReaderThread .joinable()) _stdinReaderThread.join();
	if(_renderThread .joinable()) _renderThread.join();

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
