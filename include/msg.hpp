#pragma once
#include "handle.hpp"
#include "defs.hpp"
#include "queue.hpp"

namespace winTerm {

	struct window;

	struct msg {
		handle<window> hWnd;
		message m;

		union param {
			keyboard kbd;
			int quitVal;
			long l;

			param() noexcept : l(0) {}
		} param;

		msg() noexcept : hWnd(nullptr), m(NONE), param() {}

		template<typename T>
		msg(const message m, T && value) noexcept : hWnd(nullptr), m(m), param() {
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

		template <typename T>
		msg(const handle<window>hWnd, const message m, T&& value) noexcept : hWnd(hWnd), m(m), param() {
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

#ifndef WINTERM_DONT_DEFINE_GLOBALS
	extern winTerm::queue<winTerm::msg, 8> _messageQueue;
#endif
}

