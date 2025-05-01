#pragma once
#include <format>
#include <utility>
#include <vector>
#include <stdexcept>
#include <memory>
#include <type_traits>
#include <utility>
#include <new>
#include <cstddef>

namespace winTerm {

	enum message
	{
		// message recieved when window is created
		// param : TBD
		CREATE,

		// message recieved before window is destroyed
		// param : TBD
		DESTROY,

		// message recieved when window needs to be painted
		// param : TBD
		PAINT,

		// message recieved for keyboard event
		// param : enum winTerm::keyboard
		KEYBOARD,
	
		// message recieved for window resize event
		// param : long {MSBs: new x dimension} , { LSBs : new y dimension}
		RESIZE,

		// message recieved for quit event
		// param : long {return code specified}
		QUIT,

		NONE,
	};


	enum keyboard
	{
		CTRL_A = 1, CTLR_B, CTRL_C , CTRL_D , CTRL_E , CTRL_F ,
		CTRL_G , CTRL_H , CTRL_I , CTRL_J , CTRL_K , CTRL_L ,
		CTRL_M , CTRL_N , CTRL_O , CTRL_P , CTRL_Q , CTRL_R ,
		CTLR_S , CTRL_T , CTRL_U , CTRL_V , CTRL_W , CTRL_X , CTRL_Y , CTRL_Z ,
		CTRL_4 = 28 , CTRL_5 , CTRL_6 , CTRL_7 , 

		ESCAPE = 27,

		// alpha numeric keys ranging 32 - 127 dont need an enumerator
		// use ASCII values

		// other keys kept well out of range of ascii
		ARROW_LEFT = 1000,
		ARROW_RIGHT,
		ARROW_UP,
		ARROW_DOWN,
		DELETE,
		PAGE_DOWN,
		PAGE_UP,
		HOME,
		END,

		FN_1 , FN_2 , FN_3 , FN_4 , FN_5 , FN_6 ,
		FN_7 , FN_8 , FN_9 , FN_10 , FN_11 , FN_12 ,

		// TODO: alt keys...
	};

	enum colour
	{
		maroon = 0x800000,
		dark_red = 0x8B0000,
		brown = 0xA52A2A,
		firebrick = 0xB22222,
		crimson = 0xDC143C,
		red = 0xFF0000,
		tomato = 0xFF6347,
		coral = 0xFF7F50,
		indian_red = 0xCD5C5C,
		light_coral = 0xF08080,
		dark_salmon = 0xE9967A,
		salmon = 0xFA8072,
		light_salmon = 0xFFA07A,
		orange_red = 0xFF4500,
		dark_orange = 0xFF8C00,
		orange = 0xFFA500,
		gold = 0xFFD700,
		dark_golden_rod = 0xB8860B,
		golden_rod = 0xDAA520,
		pale_golden_rod = 0xEEE8AA,
		dark_khaki = 0xBDB76B,
		khaki = 0xF0E68C,
		olive = 0x808000,
		yellow = 0xFFFF00,
		yellow_green = 0x9ACD32,
		dark_olive_green = 0x556B2F,
		olive_drab = 0x6B8E23,
		lawn_green = 0x7CFC00,
		chartreuse = 0x7FFF00,
		green_yellow = 0xADFF2F,
		dark_green = 0x006400,
		green = 0x00FF00,
		forest_green = 0x228B22,
		lime = 0x008000,
		lime_green = 0x32CD32,
		light_green = 0x90EE90,
		pale_green = 0x98FB98,
		dark_sea_green = 0x8FBC8F,
		medium_spring_green = 0x00FA9A,
		spring_green = 0x00FF7F,
		sea_green = 0x2E8B57,
		medium_aqua_marine = 0x66CDAA,
		medium_sea_green = 0x3CB371,
		light_sea_green = 0x20B2AA,
		dark_slate_gray = 0x2F4F4F,
		teal = 0x008080,
		dark_cyan = 0x008B8B,
		aqua = 0x00FFFF,
		cyan = 0x00FFFF,
		light_cyan = 0xE0FFFF,
		dark_turquoise = 0x00CED1,
		turquoise = 0x40E0D0,
		medium_turquoise = 0x48D1CC,
		pale_turquoise = 0xAFEEEE,
		aqua_marine = 0x7FFFD4,
		powder_blue = 0xB0E0E6,
		cadet_blue = 0x5F9EA0,
		steel_blue = 0x4682B4,
		corn_flower_blue = 0x6495ED,
		deep_sky_blue = 0x00BFFF,
		dodger_blue = 0x1E90FF,
		light_blue = 0xADD8E6,
		sky_blue = 0x87CEEB,
		light_sky_blue = 0x87CEFA,
		midnight_blue = 0x191970,
		navy = 0x000080,
		dark_blue = 0x00008B,
		medium_blue = 0x0000CD,
		blue = 0x0000FF,
		royal_blue = 0x4169E1,
		blue_violet = 0x8A2BE2,
		indigo = 0x4B0082,
		dark_slate_blue = 0x483D8B,
		slate_blue = 0x6A5ACD,
		medium_slate_blue = 0x7B68EE,
		medium_purple = 0x9370DB,
		dark_magenta = 0x8B008B,
		dark_violet = 0x9400D3,
		dark_orchid = 0x9932CC,
		medium_orchid = 0xBA55D3,
		purple = 0x800080,
		thistle = 0xD8BFD8,
		plum = 0xDDA0DD,
		violet = 0xEE82EE,
		magenta_fuchsia = 0xFF00FF,
		orchid = 0xDA70D6,
		medium_violet_red = 0xC71585,
		pale_violet_red = 0xDB7093,
		deep_pink = 0xFF1493,
		hot_pink = 0xFF69B4,
		light_pink = 0xFFB6C1,
		pink = 0xFFC0CB,
		antique_white = 0xFAEBD7,
		beige = 0xF5F5DC,
		bisque = 0xFFE4C4,
		blanched_almond = 0xFFEBCD,
		wheat = 0xF5DEB3,
		corn_silk = 0xFFF8DC,
		lemon_chiffon = 0xFFFACD,
		light_golden_rod_yellow = 0xFAFAD2,
		light_yellow = 0xFFFFE0,
		saddle_brown = 0x8B4513,
		sienna = 0xA0522D,
		chocolate = 0xD2691E,
		peru = 0xCD853F,
		sandy_brown = 0xF4A460,
		burly_wood = 0xDEB887,
		rosy_brown = 0xBC8F8F,
		moccasin = 0xFFE4B5,
		navajo_white = 0xFFDEAD,
		peach_puff = 0xFFDAB9,
		misty_rose = 0xFFE4E1,
		lavender_blush = 0xFFF0F5,
		linen = 0xFAF0E6,
		old_lace = 0xFDF5E6,
		papaya_whip = 0xFFEFD5,
		sea_shell = 0xFFF5EE,
		mint_cream = 0xF5FFFA,
		slate_gray = 0x708090,
		light_slate_gray = 0x778899,
		light_steel_blue = 0xB0C4DE,
		lavender = 0xE6E6FA,
		floral_white = 0xFFFAF0,
		alice_blue = 0xF0F8FF,
		ghost_white = 0xF8F8FF,
		honeydew = 0xF0FFF0,
		ivory = 0xFFFFF0,
		azure = 0xF0FFFF,
		snow = 0xFFFAFA,
		black = 0x000000,
		dim_grey = 0x696969,
		grey = 0x808080,
		dark_grey = 0xA9A9A9,
		silver = 0xC0C0C0,
		light_grey = 0xD3D3D3,
		gainsboro = 0xDCDCDC,
		white_smoke = 0xF5F5F5,
		white = 0xFFFFFF,
	};

	enum borderStyle {
		none = 0,
		thick = 6,
		thin = 12,
		two = 18,
	};

	enum emphasis {
		norm		= 0,
		bold		= 1,
		faint 		= 2,
		italic 		= 2,
		underline 	= 3,
		blink 		= 4,
		reverse 	= 5,
		conceal 	= 6,
		strikethrough = 7,
	};

	static constexpr char attribute_codes[7] = {
		'1',    // Bold
		'3',    // Italic
		'4',    // Underline
		'5',    // Blink Slow
		'6',    // Blink Rapid
		'7',    // Reverse
		'8'     // Hidden
	};

	struct rect
	{
		unsigned int left;
		unsigned int top;
		unsigned int right;
		unsigned int bottom;

		rect(unsigned int left , unsigned int top , unsigned int right , unsigned int bottom);
		rect& operator &=(const rect& other);
        rect& operator|=(const rect& other);
	};

	struct cell
	{
		wchar_t character;
		colour fgColor;
		colour bgColor;
		emphasis emph;

		cell();
		cell(wchar_t c);
		cell(wchar_t c , const enum colour fg , const enum colour bg , emphasis emph);
	};

	enum class canvMsg
	{
		OPTIMISE,
		FORCE_RERENDER,
		END,
	};

	class canvas final {
	public:
		canvas(
			const unsigned int width, const unsigned int height,
			const unsigned int x = 0, const unsigned int y = 0,
			const cell& cl = {L' ', colour::white, colour::black, emphasis::norm},
			const canvMsg msg = canvMsg::OPTIMISE):

			width_(width) , height_(height),
			x_(x), y_(y),
			buffer_(height , std::vector<cell>(width , cl)),
			renderStrGenResetSeq(std::format("\x1b[1B\x1b[{:d}D", width_)),
			message_(msg)
		{
			if((width < 1 || height < 1) && msg != canvMsg::END)
				throw std::out_of_range(std::format("invalid size({:d} , {:d})" , width , height));
		}

		~canvas() = default;

		canvas(const canvas& ) = delete;
		canvas& operator=(const canvas&) = delete;

		// resize the buffer
		// this will ruin the data held in the vector
		void resize(const int newWidth , const int newHeight) noexcept;
		
		// fetch cell reference at row or column
		// throw std::out_of_range if not in buffer
		cell& at(const unsigned int row , const unsigned int column);

		// clear the buffer completely with default cell
		void clear() noexcept;
		// clear the buffer and put a specified cell in place
		void clear(const cell& cl) noexcept;
		
		// setBackGround of entire buffer
		// will not modify character content of buffer
		void setBackground(const colour col) noexcept;
		inline colour getBackground() const noexcept { return background_; }

		// add text to a window
		// throw std::out_of_range if given string is completely out of range
		// no automatic wrapping will simply cut string off at the end of the buffer
		void addText(const std::string& str , unsigned int row , unsigned int column , 
			   const colour fg , const colour bg , emphasis emph);

		// draw a rectangle at given location (window coordinates)
		// throw std::out_of_range if given rect is completely out of range
		// if escaping buffer, will cutoff at border
		// set erase true if the rectangle should erase existing characters in specified area
		void drawRect(const rect& rectangle , const colour bg , const borderStyle bs , const bool erase) noexcept;
		
		// set the border of the window buffer with given border style
		void setBorder(const borderStyle bs) noexcept;


		//
		void getBuffer(std::vector<std::vector<cell>>& buffer) const { buffer = buffer_; }
		void updateRenderScheme(canvMsg in) noexcept { message_ = in; }

		void setPosition(const unsigned int x, const unsigned int y);
		
		//generate render string based on render schema
		void renderStringGenerate(std::string& out) const noexcept;

		inline canvMsg getMessage() const noexcept {return message_;}

	private:


		unsigned int width_ , height_;
		unsigned int x_, y_;
		std::vector<std::vector<cell>> buffer_;
		colour background_;

		const std::string renderStrGenResetSeq;

		canvMsg message_;
	};
	
	struct msg
	{
		message m;

		union param {
			keyboard kbd;
			int quitVal;
			long l;

			param() noexcept : l(0) {}
		} param;

		msg() noexcept : m(NONE), param() {}

		template <typename T>
		msg(const message m, T&& value) noexcept : m(m), param() {
			switch(m) {
				case message::KEYBOARD:
					param.kbd = static_cast<keyboard>(value);
					break;
				case message::QUIT:
					param.quitVal = static_cast<int>(value);
					break;
				case message::PAINT:
					param.l = static_cast<long>(0);
					break;
				default:
					param.l = static_cast<long>(0);
					break;
			}
		}
	};

	#define QUEUE_INCREMENT(idx) (N_is_pow2 ? ((idx + 1) & (N - 1)) : ((idx + 1) % N))
	template<typename T, std::size_t N>
	class queue
	{
	private:
		static_assert(N > 0, "Please provide a valid queue size");
		static constexpr bool N_is_pow2 = (N != 0) && ((N & (N - 1)) == 0);

	public:
		queue() = default;
		~queue()
		{
			for (std::size_t i = 0; i < count; ++i)
				std::destroy_at(std::launder(reinterpret_cast<T*>(&storage[(head + i) % N])));
		}
		
		
		template<typename U>
		void push(U&& value) noexcept(std::is_nothrow_constructible_v<T, U>)
		{
			if (full()) return;
			new (&storage[tail]) T(std::forward<U>(value));
			tail = QUEUE_INCREMENT(tail);
			++count;
		}

		template<typename... Args>
		void emplace(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
		{
			if (full()) return;
			new (&storage[tail]) T(std::forward<Args>(args)...);
			tail = QUEUE_INCREMENT(tail);
			++count;
		}

		void pop()
		{
			if (empty()) return;
			std::destroy_at(std::launder(reinterpret_cast<T*>(&storage[head])));
			head = QUEUE_INCREMENT(head);
			--count;
		}

		T& front()
		{
			return *std::launder(reinterpret_cast<T*>(&storage[head]));
		}

		const T& front() const
		{
			return *std::launder(reinterpret_cast<const T*>(&storage[head]));
		}

		T& back() noexcept 
		{ 
			// Fix back() to return the last inserted element
			std::size_t back_idx = tail == 0 ? N - 1 : tail - 1;
			return *std::launder(reinterpret_cast<T*>(&storage[back_idx]));
		}
		
		const T& back() const noexcept 
		{ 
			std::size_t back_idx = tail == 0 ? N - 1 : tail - 1;
			return *std::launder(reinterpret_cast<const T*>(&storage[back_idx]));
		}

		bool empty() const { return count == 0; }
		
		bool full() const { return count == N; }
		
		std::size_t size() const { return count; }
		
	private:
		alignas(T) std::byte storage[sizeof(T) * N]{};
		std::size_t head = 0;
		std::size_t tail = 0;
		std::size_t count = 0;
	};
	#undef QUEUE_INCREMENT


	//////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////

	// initilise winTerm context
	// will open new terminal buffer and terminal will enter raw mode
	// only call once at the start of the program
	int initialise();

	// cleanup entire winTerm context
	// will close alternative terminal buffer and restore previous terminal settings
	int destroy();

	// use to bind message to quitting program
	// set the return Code if required
	// if unable to post, will force push by popping extra event and pushing
	void postQuitMessage(int returnCode);

	// use to bind message to a painting operation
	// return true on success and false on failure
	void postPaintMessage();
	
	// use to get events from event queue
	// return value is 0 if QUIT message
	// positive if no error
	// negative if error
	int getMessage(msg *m) noexcept;

	// begin painting by aquiring a canvas
	// will return null if canvas could not be aquired
	canvas* beginPaint(int rows , int columns) noexcept;

	// finish painting by releasing the canvas with render data
	// this will notify the render thread and push the task onto the queue
	void endPaint(canvas* canv) noexcept;
}
