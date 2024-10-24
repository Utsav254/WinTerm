#include <unistd.h>
#include <vector>
#include "WinTerm/winTerm.hpp"
#include "WinTerm/render/canvas.hpp"
#include "WinTerm/render/cell.hpp"
#include "WinTerm/render/misc.hpp"
#include <cwchar>

int rectRight = 2;
int rectBottom = 2;

void termProc(const winTerm::event& e) {
	switch (e.t_) {
		case winTerm::event::KEYBOARD:
			switch(e.param_) {
				case static_cast<long>(winTerm::keyboard::CTRL_Q):
					winTerm::postQuitEvent(0);
					break;
				case static_cast<long>(winTerm::keyboard::ARROW_RIGHT):
					rectRight++;
					break;
				case static_cast<long>(winTerm::keyboard::ARROW_DOWN):
					rectBottom++;
					break;					
			}
			winTerm::postPaintEvent();
			break;
        case winTerm::event::PAINT:
			{
				setlocale(LC_ALL, "");
				char moveCursorHome[4] = "\x1b[H";
				write(STDOUT_FILENO , moveCursorHome , sizeof(moveCursorHome));
				
				constexpr unsigned int height = 35 , width = 50;
				
				winTerm::canvas cv(width , height);
				cv.clear(winTerm::cell(L'@'));

				winTerm::rect r(1 , 1 , rectRight , rectBottom);

				cv.drawRect<winTerm::borderStyle::THIN>(r , fmt::color::black , false);

				std::vector<std::vector<winTerm::cell>> buffer;
				cv.getBuffer(buffer);

				char mbstr[128];
			
				for(int j = 0 ; j < (int)buffer.size() ; j++) {
					for (int i = 0 ; i < (int)buffer[j].size() ; i++) {
						int bytes = wctomb(mbstr, buffer[j][i].character);
						if(bytes == -1) throw std::runtime_error("wctomb error bytes -1");
						write(STDOUT_FILENO, mbstr, bytes);

					}
					char newline[] = "\n\r";
					write(STDOUT_FILENO , newline , 2);
				}
			}
			break;
		case winTerm::event::RESIZE:
		case winTerm::event::NONE:
        case winTerm::event::QUIT:
            break;
          break;
        }
}

int main()
{
	winTerm::initialise();

	winTerm::event e;
	int getEventResult;
	while ((getEventResult = winTerm::getEvent(e)))
	{
		termProc(e);
	}

	winTerm::destroy();
	
	if(getEventResult == 0) return e.param_;
	else return EXIT_FAILURE;
}


