#pragma once
#include "WinTerm/render/misc.hpp"
#include <memory>
#include <variant>

namespace winTerm
{
	enum class keyboard;

	struct message
	{
		enum type
		{
			KEYBOARD,		// param: winTerm::keyboard
			PAINT,			// param: std::unique_ptr<rect>
			RESIZE,			// param: long {MSBs: new x dimension} , { LSBs : new y dimension}
			QUIT,			// param: long {return code}
		
			NONE,			// param: don't care
		};

		type t;
		std::variant<keyboard , std::unique_ptr<rect> , long> param;

		message() noexcept : t(NONE) , param(0) {}

		template <typename T>
		message(const type t , T&& value) noexcept : t(t) , param(std::forward<T>(value)) {}

		~message() = default;

		message(const message& other) = delete;
		message& operator=(const message& other) = delete;
	};

	enum class keyboard
	{
		// control combos
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


}

