#pragma once
#include <array>
#include <memory>
#include <mutex>
#include <condition_variable>


template <typename T , std::size_t N>
class channel
{
public:
	channel() noexcept : head_(0) , tail_(0) , count_(0) {}
	~channel() noexcept
	{
		clear();
	}
	
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
	
	std::unique_ptr<T> pop(std::atomic<bool>& override) noexcept
	{
		std::unique_lock<std::mutex> lock(mtx_);
		condVar_.wait(lock , [&]{ return count_ > 0 || override.load(); });

		std::unique_ptr<T> ptr = std::move(queue_[tail_]);
		tail_ = (tail_ + 1) % N;
		count_--;

		return ptr;
	}
	
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

	std::array<std::unique_ptr<T> , N> queue_;
	
	// vars for tracking queue state
	std::size_t head_;
	std::size_t tail_;
	std::size_t count_;

	// threading members	
	mutable std::mutex mtx_;
	std::condition_variable condVar_;
};

