#pragma once
#include <variant>
#include "WinTerm/enums.hpp"

namespace winTermNameSpace {
	struct msg
	{
		unsigned int m;	
		std::variant<keyboard , long> param;

		msg() noexcept : m(NONE) , param(0) {}

		template <typename T>
		msg(const unsigned int m , T&& value) noexcept : m(m) , param(std::forward<T>(value)) {}

		~msg() = default;

		msg(const msg& other): m(other.m), param(other.param) {}
		msg& operator=(const msg& other) noexcept
		{
			if (this != &other) {
				m = other.m;
				param = other.param;
			}
			return *this;		
		}
	};
}
