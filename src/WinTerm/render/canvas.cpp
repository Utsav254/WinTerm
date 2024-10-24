#include "WinTerm/render/canvas.hpp"
#include <stdexcept>
#include <array>

namespace winTerm
{
	void canvas::resize(const int newWidth , const int newHeight) noexcept
	{
		buffer_.resize(newHeight , std::vector<cell>(newWidth , cell()));	
	}

	cell& canvas::at(const unsigned int row , const unsigned int column)
	{
		if(row < height_ && column < width_) {
			return buffer_[row][column];
		}
		else {
			throw std::out_of_range(fmt::format("index ({:d} , {:d}) out of range" , row , column));
		}
	}

	void canvas::clear() noexcept
	{
		for(std::vector<cell>& row : buffer_) {
			for(cell& elem : row) {
				elem.character = L' ';
			}
		}
	}

	void canvas::clear(const cell& cl) noexcept
	{
		for(std::vector<cell>& row : buffer_) {
			for(cell& elem : row) {
				elem = cl;
			}
		}
	}

	void canvas::setBackground(const fmt::color col) noexcept
	{
		for(std::vector<cell>& row : buffer_) {
			for(cell& elem : row) {
				elem.bgColor = col;
			}
		}
		background_ = col;
		// maybe needed but performance testing showed that for loop is better
		// std::for_each(std::execution::par_unseq , buffer_.begin() , buffer_.end() , [col](cell& cl) {cl.bgColor = col;});	
	}

	void canvas::addText(const std::string& str , unsigned int row , unsigned int column , 
					  const fmt::color fg , const fmt::color bg , fmt::emphasis style)
	{
		if(row > height_ || column > width_) 
			throw std::out_of_range(fmt::format("index ({:d} , {:d}) out of range" , row , column));
		else {
			for(unsigned int i = 0 ; i < (width_ - row - 1) ; i++) {
				buffer_[row][i] = cell(str[i] , fg , bg , style);
			}
		}
	}

	void canvas::drawRect(const rect& rectangle , const fmt::color bg , const borderStyle bs , const bool erase)
	{
		if(rectangle.bottom > height_ || rectangle.right > width_)
			throw std::out_of_range(fmt::format("rectangle size({:d} , {:d}) out of range" , rectangle.bottom , rectangle.right));
		
		std::array<wchar_t , 6> borderChars;
		switch(bs) {
			case borderStyle::NONE:
				borderChars = {' ' , ' ' , ' ' , ' ' , ' ' , ' '};
				break;
			case borderStyle::THIN:
				borderChars = {L'─', L'│', L'┌', L'┐', L'└', L'┘'};
				break;
			case borderStyle::THICK:
				borderChars = {L'━', L'┃', L'┏', L'┓', L'┗', L'┛'};
				break;
			case borderStyle::DOUBLE:
				borderChars = {L'═', L'║', L'╔', L'╗', L'╚', L'╝'};
				break;
			default:
				borderChars = {'?', '?', '?', '?', '?', '?'};
				break;
		}
		drawRectangleImpl(borderChars , rectangle , bg , erase);
    }

	void canvas::drawRectangleImpl(const std::array<wchar_t, 6>& border_chars , const rect& rectangle,
									const fmt::color bg , const bool erase) {
		// fixed for indexing variables
		const unsigned int corrLeft = rectangle.left -1;
		const unsigned int corrTop = rectangle.top - 1;
		const unsigned int corrRight = rectangle.right - 1;
		const unsigned int corrBottom = rectangle.bottom -1;

		for (unsigned int y = corrTop ; y <= corrBottom ; ++y) {
			if(y > height_ - 1) break;
			for (unsigned int x = corrLeft ; x <= corrRight ; ++x) {
				if(x > width_ - 1) break;
				if (y == corrTop && x == corrLeft) {
					buffer_[y][x].character = border_chars[2]; // Top-left corner
					buffer_[y][x].bgColor = bg;
				}
				else if (y == corrTop && x == corrRight) {
					buffer_[y][x].character = border_chars[3]; // Top-right corner
					buffer_[y][x].bgColor = bg;
				}
				else if (y == corrBottom && x == corrLeft) {
					buffer_[y][x].character = border_chars[4]; // Bottom-left corner
					buffer_[y][x].bgColor = bg;
				}
				else if (y == corrBottom && x == corrRight) {
					buffer_[y][x].character = border_chars[5]; // Bottom-right corner
					buffer_[y][x].bgColor = bg;
				}
				else if (y == corrTop || y == corrBottom) {
					buffer_[y][x].character = border_chars[0]; // Horizontal border
					buffer_[y][x].bgColor = bg;
				}
				else if (x == corrLeft || x == corrRight) {
					buffer_[y][x].character = border_chars[1]; // Vertical border
					buffer_[y][x].bgColor = bg;
				}
				else {
					if(erase) {
						buffer_[y][x].character = L' '; // Background character
						buffer_[y][x].bgColor = bg;
					}
					else {
						buffer_[y][x].bgColor = bg;
					}
				}
			}
		}
	}
}
