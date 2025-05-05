#pragma once
#include <cstdint>
#include <stdexcept>
#include "handle.hpp"

namespace winTerm {

	template<typename T, std::size_t N>
	struct registry {
		static_assert(N > 0, "Capacity of registry must be valid (greater than zero)");
		static_assert(N < (1u << 24), "Capacity of registry must be valid (less than 2 ^ 24)");

	private:
		struct object_slot {
			T data;
			uint32_t version;

			object_slot() = default;
			
			template<typename... Args>
			object_slot(uint32_t ver, Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
				: data(std::forward<Args>(args)...), version(ver) {}
		};
		
		object_slot objects[N];
		uint32_t objectCount = 0;
		uint32_t nextVersion = 1; // Start from 1 to distinguish from default handle version 0

	public:
		using iterator = T*;
		using const_iterator = const T*;
		
		registry() = default;
		
		template<typename... Args>
		handle<T> allocate(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>) {
			if (objectCount >= N)
				throw std::bad_alloc();
			
			uint32_t idx = objectCount;
			uint32_t version = nextVersion++;
			
			new (&objects[idx]) object_slot(version, std::forward<Args>(args)...);
			
			objectCount++;
			return handle<T>(idx, version);
		}
		
		void deallocate(const handle<T>& h) noexcept {
			if (!valid(h)) return;
			
			uint32_t idx = h.index();
			
			// Invalidate the version to prevent further access
			objects[idx].version = 0;
			
			// If it's not the last object, move the last object to fill the gap
			if (idx < objectCount - 1) {
				objects[idx] = std::move(objects[objectCount - 1]);
				// Note: No need to destroy the last object since we moved it
			}
			
			objectCount--;
		}
		
		inline T& get(const handle<T>& h) noexcept {
			return objects[h.index()].data;
		}
		
		inline const T& get(const handle<T>& h) const noexcept {
			return objects[h.index()].data;
		}
		
		inline bool valid(const handle<T>& h) const {
			const uint32_t idx = h.index();
			return h.valid() && 
				   idx < objectCount && 
				   objects[idx].version == h.version() &&
				   objects[idx].version != 0; // 0 indicates invalid/deleted object
		}
		
		inline T& tryGet(const handle<T>& h) {
			if (valid(h)) return objects[h.index()].data;
			else throw std::out_of_range("tryGet on invalid handle");
		}
		
		inline const T& tryGet(const handle<T>& h) const {
			if (valid(h)) return objects[h.index()].data;
			else throw std::out_of_range("tryGet on invalid handle");
		}
		
		inline std::size_t size() const noexcept { return objectCount; }
		inline constexpr std::size_t capacity() const noexcept { return N; }
		inline bool empty() const noexcept { return objectCount == 0; }
		inline bool full() const noexcept { return objectCount == N; }
		
		void clear() {
			if constexpr (!std::is_trivially_destructible_v<T>) {
				for (uint32_t i = 0; i < objectCount; i++) {
					objects[i].data.~T();
				}
			}
			objectCount = 0;
		}
		
		inline iterator begin() noexcept {
			iterator result = nullptr;
			if (objectCount > 0) {
				result = &(objects[0].data);
			}
			return result;
		}
		
		inline iterator end() noexcept {
			iterator result = nullptr;
			if (objectCount > 0) {
				result = &(objects[0].data) + objectCount;
			}
			return result;
		}
		
		inline const_iterator begin() const noexcept {
			const_iterator result = nullptr;
			if (objectCount > 0) {
				result = &(objects[0].data);
			}
			return result;
		}
		
		inline const_iterator end() const noexcept {
			const_iterator result = nullptr;
			if (objectCount > 0) {
				result = &(objects[0].data) + objectCount;
			}
			return result;
		}
	};
}

