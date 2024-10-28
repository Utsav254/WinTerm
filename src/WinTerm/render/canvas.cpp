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
			throw std::out_of_range(fmt::format("at: index ({:d} , {:d}) out of range" , row , column));
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
			throw std::out_of_range(fmt::format("addText: index ({:d} , {:d}) out of range" , row , column));
		else {
			for(unsigned int x = 0 ; x < str.size() ; x++) {
				if(x + column >= width_) break;
				buffer_[row][column + x] = cell(str[x] , fg , bg , style);
			}
		}
	}

	void canvas::drawRect(const rect& rectangle , const fmt::color bg , const borderStyle bs , const bool erase) noexcept
	{
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
									const fmt::color bg , const bool erase) noexcept {

		for (unsigned int y = rectangle.top; y <= rectangle.bottom; ++y) {
			if(y >= height_) break;
			for (unsigned int x = rectangle.left; x <= rectangle.right; ++x) {
				if(x >= width_) break;
				if (y == rectangle.top && x == rectangle.left) {
					buffer_[y][x].character = border_chars[2]; // Top-left corner
					buffer_[y][x].bgColor = bg;
				}
				else if (y == rectangle.top && x == rectangle.right) {
					buffer_[y][x].character = border_chars[3]; // Top-right corner
					buffer_[y][x].bgColor = bg;
				}
				else if (y == rectangle.bottom && x == rectangle.left) {
					buffer_[y][x].character = border_chars[4]; // Bottom-left corner
					buffer_[y][x].bgColor = bg;
				}
				else if (y == rectangle.bottom && x == rectangle.right) {
					buffer_[y][x].character = border_chars[5]; // Bottom-right corner
					buffer_[y][x].bgColor = bg;
				}
				else if (x == rectangle.left || x == rectangle.right) {
					buffer_[y][x].character = border_chars[1]; // Vertical border
					buffer_[y][x].bgColor = bg;
				}
				else if (y == rectangle.top || y == rectangle.bottom) {
					buffer_[y][x].character = border_chars[0]; // Horizontal border
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

	void canvas::setBorder(const borderStyle bs) noexcept
	{
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
		drawBorderImpl(borderChars);
    }

	void canvas::drawBorderImpl(const std::array<wchar_t , 6>& borderChars) noexcept
	{
		// corners 
		buffer_[0][0].character = borderChars[2];
		buffer_[0][width_ - 1].character = borderChars[3];
		buffer_[height_ - 1][0].character = borderChars[4];
		buffer_[height_ - 1][width_ - 1].character = borderChars[5];
		
		// horizontal borders
		for(unsigned int i = 1 ; i < width_ - 1 ; i++) buffer_[0][i].character = borderChars[0];
		for(unsigned int i = 1 ; i < width_ - 1 ; i++) buffer_[height_ - 1][i].character = borderChars[0];
		
		// vertical borders
		for(unsigned int j = 1 ; j < height_ - 1 ; j++) {
			buffer_[j][0].character = borderChars[1];
			buffer_[j][width_ - 1].character = borderChars[1];
		}
	}


}
