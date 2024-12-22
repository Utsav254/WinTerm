#pragma once
#include "generics/allocator.hpp"
#include <mutex>
#include <condition_variable>
#include <array>

template<typename T, std::size_t N>
class channel final {
private:

	memAlloc<T , N> allocator_;
	std::array<handle<T> , N> queue_;

	std::size_t head_  = 0, tail_ = 0, count_ = 0;

	mutable std::mutex mutex_;
	std::condition_variable isEmpty_ , isFull_;

public:

	channel() = default;
	
	// will await for channel to get emptied
	~channel() noexcept
	{
        std::unique_lock<std::mutex> lock(mutex_);
        isFull_.wait(lock, [this](){ return count_ == 0; });
        
        for(handle<T>& h: queue_) {
            h.reset();    
        }
	}

	[[nodiscard]] inline handle<T> handleGet()
	{
		return allocator_.allocate();
	}

	template<typename ... Args>
	[[nodiscard]] inline handle<T> handleGet(Args&& ... args)
	{
		return allocator_.allocate(std::forward<Args>(args)...);
	}

	void push(handle<T> inHandle) noexcept
	{
		std::unique_lock<std::mutex> lock(mutex_);
		isFull_.wait(lock , [this](){ return count_ < N; }); 
		
		queue_[head_] = std::move(inHandle);
		head_ = (head_ + 1) % N;
		++count_;

		isEmpty_.notify_one();
	}
	
	template<typename ... Args>
	void emplace(Args&& ... args)
	{
		std::unique_lock<std::mutex> lock(mutex_);
		isFull_.wait(lock , [this]() { return count_ < N; }); 
		
		queue_[head_] = std::move(allocator_.allocate(std::forward<Args>(args)...));
		head_ = (head_ + 1) % N;
		++count_;

		isEmpty_.notify_one();
	}

	[[nodiscard]] handle<T> pop()
	{
		std::unique_lock<std::mutex> lock(mutex_);
		isEmpty_.wait(lock , [this](){ return count_ > 0; });

		handle<T> h = std::move(queue_[tail_]);
		tail_ = (tail_ + 1) % N;
		--count_;

		isFull_.notify_one();

		return h;
	}

    void close()
    {
    }

	std::size_t size() const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return count_;
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return count_ == 0;
	}

	bool full() const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return count_ == N;
	}

	constexpr std::size_t capacity() const
	{
		return N;
	}

	//
	// future features i will try myself if i need include try push try emplace which wont neccassarily block
	// they will return some code if unable to 
	//
	// force push object for high prioirty items which need to be pushed into the queue
	//
	// time out push and pop operations ??
	//
	// anyways these are just features im thinking of doing they are low priority as i say

};
