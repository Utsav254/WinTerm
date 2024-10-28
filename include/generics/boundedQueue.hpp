#pragma once

#include <array>
#include <memory>
#include <mutex>
#include <condition_variable>

// bounded queue template class
// will hold unique_ptrs of the specifier type
// intended for multithreaded environments
// no exceptions are thrown
template <typename T , std::size_t N>
class boundedQueue
{
public:
	boundedQueue() noexcept : head_(0) , tail_(0) , count_(0) {}
	~boundedQueue() noexcept
	{
		clear();
	}
	
	// push an element into the queue
	// will std::move the element into the queue
	// return a false on failure
	bool push(std::unique_ptr<T> inPtr) noexcept
	{
		std::unique_lock<std::mutex> lock(mtx_);

		if(count_ == N) return false;

		queue_[head_] = std::move(inPtr);
		head_ = (head_ + 1) % N;
		count_++;

		condVar_.notify_one();

		return true;
	}
	
	// emplace an element into the queue
	// return false on failure
	// throw through std::make_unique or the conustructor of object
	template <typename ... Args>
	bool emplace(Args&& ... args)
	{
		std::unique_lock<std::mutex> lock(mtx_);

		if(count_ == N) return false;
		
		queue_[head_] = std::make_unique<T>(std::forward<Args>(args)...);
		head_ = (head_ + 1) % N;
		count_++;

		condVar_.notify_one();

		return true;
	}
	
	// pop and element from the queue
	// aquire ownership of the front most element in queue
	// will wait and block if queue is empty
	// safe to assume returned value is not nullptr
	std::unique_ptr<T> pop(std::atomic<bool>& override) noexcept
	{
		std::unique_lock<std::mutex> lock(mtx_);
		condVar_.wait(lock , [&]{ return count_ > 0 || override.load(); });

		std::unique_ptr<T> ptr = std::move(queue_[tail_]);
		tail_ = (tail_ + 1) % N;
		count_--;

		return ptr;
	}
	
	// clear the queue
	void clear() noexcept
	{
		std::unique_lock<std::mutex> lock(mtx_);
		for(unsigned int i = 0 ; i < N ; i++) {
			queue_[i].reset();
		}
		head_ = 0;
		tail_ = 0;
		count_ = 0;
		condVar_.notify_all();
	}
	
	// helper functions for checking queue status
	inline bool isFull() const noexcept
	{
		std::unique_lock<std::mutex> lock(mtx_);
		return count_ == N;
	}

	inline bool isEmpty() const noexcept
	{
		std::unique_lock<std::mutex> lock(mtx_);
		return count_ == 0;
	}
	
	inline std::size_t size() const noexcept
	{
		std::unique_lock<std::mutex> lock(mtx_);
		return count_;
	}

private:
	// queue buffer
	std::array<std::unique_ptr<T> , N> queue_;
	
	// vars for tracking queue state
	std::size_t head_;
	std::size_t tail_;
	std::size_t count_;

	// threading members	
	mutable std::mutex mtx_;
	std::condition_variable condVar_;
};

