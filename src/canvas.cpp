#include <winTerm.hpp>

using namespace winTerm;

cell::cell(): 
	character(' '), 
	fgColor(colour::white),
	bgColor(colour::black),
	emph(static_cast<emphasis>(0))
{}

cell::cell(wchar_t c):
	character(c),
	fgColor(colour::white),
	bgColor(colour::black),
	emph(static_cast<emphasis>(0))
{}

cell::cell(wchar_t c , const enum colour fg , const enum colour bg , emphasis emph):
	character(c),
	fgColor(fg),
	bgColor(bg),
	emph(emph)
{}

static constexpr std::array<wchar_t , 6 * 4>borderChars =
{
	L' ' , L' ' , L' ' , L' ' , L' ' , L' ', // borderStyles::NONE
	L'─' , L'│' , L'┌' , L'┐' , L'└' , L'┘', // borderStyles::THIN
	L'━' , L'┃' , L'┏' , L'┓' , L'┗' , L'┛', // borderStyles::THICK
	L'═' , L'║' , L'╔' , L'╗' , L'╚' , L'╝', // borderStyle::DOUBLE
};

rect::rect(unsigned int left , unsigned int top , unsigned int right , unsigned int bottom):
	left(left) , top(top) , right(right) , bottom(bottom)
{
	if(right < left || bottom < top) {

		#ifdef _DEBUG
		throw std::out_of_range
		(
			std::format("rectangle bottom/right less than top/left({:d}, {:d}, {:d}, {:d})", left, top, right, bottom)
		);
		#endif

		right = left;
		bottom = top;
	}
}

rect& rect::operator &=(const rect& other)
{
	left = std::max(left, other.left);
	top = std::max(top, other.top);
	right = std::min(right, other.right);
	bottom = std::min(bottom, other.bottom);

	if (right < left || bottom < top) {
		left = top = right = bottom = 0;
	}

	return *this;
}

rect& rect::operator|=(const rect& other)
{
	left = std::min(left, other.left);
	top = std::min(top, other.top);
	right = std::max(right, other.right);
	bottom = std::max(bottom, other.bottom);

	return *this;
}

bool rect::operator&&(const rect& other) const noexcept {
	return !(left >= other.right || right <= other.left || 
			 top >= other.bottom || bottom <= other.top);
}

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
		throw std::out_of_range(std::format("at: index ({:d} , {:d}) out of range" , row , column));
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

void canvas::setBackground(const colour col) noexcept
{
	for(std::vector<cell>& row : buffer_) {
		for(cell& elem : row) {
			elem.bgColor = col;
		}
	}
	background_ = col;
}

void canvas::addText(const std::string& str , unsigned int row , unsigned int column , 
				  const colour fg , const colour bg , emphasis style)
{
	if(row > height_ || column > width_) 
		throw std::out_of_range(std::format("addText: index ({:d} , {:d}) out of range" , row , column));
	else {
		for(unsigned int x = 0 ; x < str.size() ; x++) {
			if(x + column >= width_) break;
			buffer_[row][column + x] = cell(str[x] , fg , bg , style);
		}
	}
}

void canvas::drawRect(const rect& rectangle , const colour bg , const borderStyle bs , const bool erase) noexcept
{
	unsigned int offset = static_cast<unsigned int>(bs);

	for (unsigned int y = rectangle.top; y <= rectangle.bottom; ++y) {
		if(y >= height_) break;
		for (unsigned int x = rectangle.left; x <= rectangle.right; ++x) {
			if(x >= width_) break;
			if (y == rectangle.top && x == rectangle.left) {
				buffer_[y][x].character = borderChars[offset+2]; // Top-left corner
				buffer_[y][x].bgColor = bg;
			}
			else if (y == rectangle.top && x == rectangle.right) {
				buffer_[y][x].character = borderChars[offset+3]; // Top-right corner
				buffer_[y][x].bgColor = bg;
			}
			else if (y == rectangle.bottom && x == rectangle.left) {
				buffer_[y][x].character = borderChars[offset+4]; // Bottom-left corner
				buffer_[y][x].bgColor = bg;
			}
			else if (y == rectangle.bottom && x == rectangle.right) {
				buffer_[y][x].character = borderChars[offset+5]; // Bottom-right corner
				buffer_[y][x].bgColor = bg;
			}
			else if (x == rectangle.left || x == rectangle.right) {
				buffer_[y][x].character = borderChars[offset+1]; // Vertical border
				buffer_[y][x].bgColor = bg;
			}
			else if (y == rectangle.top || y == rectangle.bottom) {
				buffer_[y][x].character = borderChars[offset+0]; // Horizontal border
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
	unsigned int offset = static_cast<unsigned int>(bs);
	// corners 
	buffer_[0][0].character = borderChars[offset+2];
	buffer_[0][width_ - 1].character = borderChars[offset+3];
	buffer_[height_ - 1][0].character = borderChars[offset+4];
	buffer_[height_ - 1][width_ - 1].character = borderChars[offset+5];
	
	// horizontal borders
	for(unsigned int i = 1 ; i < width_ - 1 ; i++) {
		buffer_[0][i].character = borderChars[offset];
		buffer_[height_ - 1][i].character = borderChars[offset];
	}
	
	// vertical borders
	for(unsigned int j = 1 ; j < height_ - 1 ; j++) {
		buffer_[j][0].character = borderChars[offset+1];
		buffer_[j][width_ - 1].character = borderChars[offset+1];
	}
}

void canvas::setPosition(const unsigned int x, const unsigned int y)
{
	x_ = x;
	y_ = y;
}


void canvas::renderStringGenerate(std::string& out) const noexcept
{
	char multiByteChar[4] = {0};

	colour currFg = buffer_[0][0].fgColor;
	colour currBg = buffer_[0][0].bgColor;
	emphasis currEmphasis = buffer_[0][0].emph;

	// initialise ansi string for render // TODO: add empphasis support
	out += std::format("\x1b[{:d};{:d}H\x1b[48;2;{:d};{:d};{:d}m\x1b[38;2;{:d};{:d};{:d}m\x1b[{:c}m",
					(y_), (x_),
					(currBg & colour::red) >> 16, (currBg & colour::green) >> 8 , (currBg & colour::blue),
					(currFg & colour::red) >> 16, (currFg & colour::green) >> 8 , (currFg & colour::blue),
					attribute_codes[currEmphasis]
				);
	for(int j = 0 ; j < (int)buffer_.size() ; j++) {
		for (int i = 0 ; i < (int)buffer_[j].size() ; i++) {

			int bytes = wctomb(multiByteChar , buffer_[j][i].character); // TODO optimise this 
			
			if(buffer_[j][i].fgColor != currFg) {
				currFg = buffer_[j][i].fgColor;
				out += std::format("\x1b[38;2;{:d};{:d};{:d}m",
					(currFg & colour::red) >> 16, (currFg & colour::green) >> 8 , (currFg & colour::blue)
				);
			}

			if(buffer_[j][i].bgColor != currBg) {
				currBg = buffer_[j][i].bgColor;
				out += std::format("\x1b[48;2;{:d};{:d};{:d}m",
					(currBg & colour::red) >> 16, (currBg & colour::green) >> 8 , (currBg & colour::blue)
				); 
			}

			if(buffer_[j][i].emph != currEmphasis) {
				currEmphasis = buffer_[j][i].emph;
				out += std::format("\x1b[{:c}m",attribute_codes[currEmphasis]);
			}
			
			if(bytes > 0) { out.append(multiByteChar , bytes); }

		}
		out += renderStrGenResetSeq;
	}
}
