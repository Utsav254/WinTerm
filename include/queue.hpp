#pragma once
#include <memory>
#include <utility>

#define QUEUE_INCREMENT(idx) (N_is_pow2 ? ((idx + 1) & (N - 1)) : ((idx + 1) % N))

namespace winTerm {

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

}

#undef QUEUE_INCREMENT
