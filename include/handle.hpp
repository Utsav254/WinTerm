#pragma once
#include <cstdint>

namespace winTerm {
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
}
