#pragma once
#include <cstdint>
#include <cstdlib>
#include <format>
#include <sys/types.h>
#include <utility>
#include <vector>
#include <stdexcept>
#include <memory>
#include <type_traits>
#include <utility>
#include <new>
#include <cstddef>

namespace winTerm {

	enum message {
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


	enum keyboard {
		CTRL_A = 1, CTLR_B, CTRL_C , CTRL_D , CTRL_E , CTRL_F ,
		CTRL_G , CTRL_H , CTRL_I , CTRL_J , CTRL_K , CTRL_L ,
		CTRL_M , CTRL_N , CTRL_O , CTRL_P , CTRL_Q , CTRL_R ,
		CTLR_S , CTRL_T , CTRL_U , CTRL_V , CTRL_W , CTRL_X , CTRL_Y , CTRL_Z ,
		CTRL_4 = 28 , CTRL_5 , CTRL_6 , CTRL_7 , 

		ESCAPE = 27,

		// alpha numeric keys ranging 32 - 127 dont need an enumerator
		// use ASCII values

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
	};

	enum colour {
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

	struct rect {
		unsigned int left;
		unsigned int top;
		unsigned int right;
		unsigned int bottom;
		
		rect() = default;
		rect(unsigned int left , unsigned int top , unsigned int right , unsigned int bottom);
		rect& operator &=(const rect& other);
        rect& operator|=(const rect& other);
		bool operator&&(const rect& other) const noexcept;
	};

	struct cell {
		wchar_t character;
		colour fgColor;
		colour bgColor;
		emphasis emph;

		cell();
		cell(wchar_t c);
		cell(wchar_t c , const enum colour fg , const enum colour bg , emphasis emph);
	};

	enum class canvMsg {
		OPTIMISE,
		FORCE_RERENDER,
		END,
	};

	struct canvas {
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

		void resize(const int newWidth , const int newHeight) noexcept;
		
		cell& at(const unsigned int row , const unsigned int column);

		void clear() noexcept;
		void clear(const cell& cl) noexcept;
		
		void setBackground(const colour col) noexcept;
		inline colour getBackground() const noexcept { return background_; }

		void addText(const std::string& str , unsigned int row , unsigned int column , 
			   const colour fg , const colour bg , emphasis emph);

		void drawRect(const rect& rectangle , const colour bg , const borderStyle bs , const bool erase) noexcept;
		
		void setBorder(const borderStyle bs) noexcept;

		void getBuffer(std::vector<std::vector<cell>>& buffer) const { buffer = buffer_; }
		void updateRenderScheme(canvMsg in) noexcept { message_ = in; }

		void setPosition(const unsigned int x, const unsigned int y);
		
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
	
	struct msg {
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
					param.l = static_cast<long>(value);
					break;
			}
		}
	};

	#define QUEUE_INCREMENT(idx) (N_is_pow2 ? ((idx + 1) & (N - 1)) : ((idx + 1) % N))
	template<typename T, std::size_t N>
	class queue {
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


	template<typename T>
	struct handle {
	private:
		uint32_t value;

		static constexpr uint8_t INDEX_BITS = 24u;
		static constexpr uint32_t INDEX_MASK = (1u << INDEX_BITS) - 1;
		static constexpr uint32_t VERSION_MASK = ~INDEX_MASK;
		static constexpr uint32_t VERSION_SHIFT = INDEX_BITS;

		static constexpr uint32_t makeHandle(uint32_t idx, uint32_t version) noexcept {
			return (version << VERSION_SHIFT) | (idx & INDEX_MASK);
		}
	public:
		constexpr handle() noexcept : value(INDEX_MASK) {}
		constexpr handle(std::nullptr_t) noexcept : value(INDEX_MASK) {}

		inline constexpr uint32_t index() const noexcept {
			return value & INDEX_MASK;
		}

		inline constexpr uint32_t version() const noexcept {
			return (value & VERSION_MASK) >> (VERSION_SHIFT);
		}

		inline constexpr bool valid() const noexcept {
			return index() != INDEX_MASK;
		}

		inline bool operator==(const handle& other) const noexcept {
			return value == other.value;
		}

		inline bool operator!=(const handle& other ) const noexcept {
			return value != other.value;
		}

		constexpr bool operator==(std::nullptr_t) const noexcept {
			return !valid();
		}

		constexpr bool operator!=(std::nullptr_t) const noexcept {
			return valid();
		}

	private:
		template<typename U, std::size_t N>
		friend struct registry;

		constexpr explicit handle(uint32_t idx, uint32_t version) noexcept : value(makeHandle(idx, version)) {}
	};

	template<typename T, std::size_t N>
	struct registry {
		static_assert(N > 0, "Capacity of registry must be valid (greater than zero)");
		static_assert(N < (1u << 24), "Capacity of registry must be valid (less than 2 ^ 24)");
	private:
		static constexpr uint32_t NULL_INDEX = static_cast<uint32_t>(-1);

		struct slot_info {
			uint32_t version;
			u_int32_t directIndex;

			slot_info() noexcept : version(0), directIndex(NULL_INDEX) {}
		};

		std::array<T,N> objects;
		std::array<slot_info,N> slots;
		std::array<uint32_t,N> freeSlots;

		uint32_t objectCount = 0;
		uint32_t freeSlotCount = N;
	public:
		using iterator = T*;
		using const_iterator = const T*;

		registry() {
			for(uint32_t i = 0 ; i < N ; i++) { 
				slots[i].version = 0;
				slots[i].directIndex = NULL_INDEX;
				freeSlots[N - i - 1] = i;	
			}
		}

		template<typename ... Args>
		handle<T> allocate(Args&& ... args) noexcept(std::is_nothrow_constructible_v<T, Args...>) {
			if(freeSlotCount == 0)
				throw std::bad_alloc();

			uint32_t slotIdx = freeSlots[--freeSlotCount];

			if constexpr (std::is_nothrow_constructible_v<T, Args...>) {
				new (&objects[objectCount]) T(std::forward<Args>(args)...);
			}
			else {
				try{
					new (&objects[objectCount]) T(std::forward<Args>(args)...);
				}
				catch(...) {
					freeSlots[freeSlotCount++] = slotIdx;
					throw;
				}

			}

			slots[slotIdx].directIndex = objectCount++;
			return handle<T>(slotIdx, slots[slotIdx].version);
		}

		void deallocate(const handle<T>& h) noexcept {
			uint32_t slotIdx = h.index();
			uint32_t objIdx = slots[slotIdx].directIndex;

			if(objIdx < objectCount - 1) {
				objects[objIdx] = std::move(objects[objectCount-1]);
				for(uint32_t i = 0 ; i < N ; i++) {
					if(slots[i].directIndex == objectCount-1) {
						slots[i].directIndex = objIdx;
						break;
					}
				}
			}

			if constexpr (!std::is_trivially_destructible_v<T>) {
				objects[--objectCount].~T();
			} else {
				--objectCount;
			}

			slots[slotIdx].directIndex = NULL_INDEX;
			slots[slotIdx].version++;
			freeSlots[freeSlotCount++] = slotIdx;
		}

		inline T& get(const handle<T>& h) noexcept {
			return objects[slots[h.index()].directIndex];
		}

		inline const T& get(const handle<T>& h) const noexcept {
			return objects[slots[h.index()].directIndex];
		}

		inline bool valid(const handle<T>& h) const {
			const uint32_t idx = h.index();
			return h.valid() && 
				   idx < slots.size() && 
				   slots[idx].direct_index != NULL_INDEX && 
				   slots[idx].version == h.version();
		}
		
		inline T& tryGet(const handle<T>& h) {
			if(valid(h)) return objects[slots[h.index()].directIndex];
			else throw std::out_of_range("tryGet on invalid handle");
		}

		inline const T& tryGet(const handle<T>& h) const {
			if(valid(h)) return objects[slots[h.index()].directIndex];
			else throw std::out_of_range("tryGet on invalid handle");
		}

		inline std::size_t size() const noexcept {return objectCount;}

		inline constexpr std::size_t capacity() const noexcept {return N;}

		inline bool empty() const noexcept {return objectCount == 0;}

		inline bool full() const noexcept {return objectCount == N;}

		void clear() {
			if constexpr(!std::is_trivially_destructible_v<T>) {
				for(uint32_t i = 0 ; i < objectCount ; i++) {
					objects[i].~T();
				}
			}

			for(uint32_t i = 0 ; i < N ; i++) {
				slots[i].directIndex = NULL_INDEX;
				slots[i].version++;
				freeSlots[N - i - 1] = i;
			}

			objectCount = 0;
			freeSlotCount = N;
		}

		inline iterator begin() noexcept {
			return &objects[0];
		}

		inline iterator end() noexcept {
			return &objects[0] + objectCount;
		}

		inline const_iterator begin() const noexcept {
			return &objects[0];
		}

		inline const_iterator end() const noexcept {
			return &objects[0] + objectCount;
		}
	};

	
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
	void postQuitMessage(int returnCode) noexcept;

	// use to bind message to a painting operation
	// return true on success and false on failure
	void postPaintMessage() noexcept;
	
	// use to get events from event queue
	// return value is 0 if QUIT message
	// positive if no error
	// negative if error
	int getMessage(msg *m) noexcept;

	// post a message to the message queue
	// silent on failure (unlikely to fail)
	void postMessage(const msg *m) noexcept;

	// send message and get message response (int)
	// sent to relevant message handling function
	int sendMessage(const msg* m) noexcept;

	// begin painting by aquiring a canvas
	// will return null if canvas could not be aquired
	canvas* beginPaint(int rows , int columns) noexcept;

	// finish painting by releasing the canvas with render data
	// this will notify the render thread and push the task onto the queue
	void endPaint(canvas* canv) noexcept;

	enum class wndStyle {
		STANDARD,
		POP_UP,
	};

	using wndProcFunc_t = int(*)(msg *m);

	struct window {
	
		window() = default;
		window(const unsigned int x, const unsigned int y,
				const unsigned int columns, const unsigned int rows,
				const wchar_t* windowTitle, wndStyle style,
				wndProcFunc_t wndProc,
				const handle<window> parentWindow,
				void *userData):
			x(x), y(y), columns(columns), rows(rows),
			windowTitle(windowTitle), style(style),
			wndProc(wndProc),
			parentWindow(parentWindow),
			userData(userData) {}

		~window() = default;

		unsigned int x, y;
		unsigned int columns, rows;
		const wchar_t * windowTitle;
		wndStyle style;
		wndProcFunc_t wndProc;
		handle<window> parentWindow;
		void *userData;

		handle<rect> mainRect;
	};

	handle<window> createWindow(const unsigned int x, const unsigned int y,
								const unsigned int columns, const unsigned int rows,
								const wchar_t* windowTitle, wndStyle style,
								wndProcFunc_t wndProc,
								const handle<window> parentWindow,
								void *userData) noexcept;

	void destroyWindow(handle<window> wnd) noexcept;
}
