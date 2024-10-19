#include "WinTerm/event.hpp"

namespace winTerm
{
	event::event() noexcept : t_(type::NONE) , param_(0) {}

	event::event(const type t , const long param) noexcept :
		t_(t) , param_(param)
	{}

	event::event(const event& other) noexcept :
		t_(other.t_) , param_(other.param_)
	{}

	event& event::operator=(const event& other) noexcept {
		if (this != &other) {
			t_ = other.t_;
			param_ = other.param_;
		}
		return *this;
	}


}
