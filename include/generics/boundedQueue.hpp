#pragma once

#include <array>
#include <stdexcept>

// bounded queue with standard push pop methods
// will not push if queue if full
// optimised to work for queue size of 256
template <typename T , std::size_t N>
class boundedQueue
{
public:
	boundedQueue() noexcept : head_(0) , tail_(0) , count_(0) {}
	~boundedQueue() noexcept = default;
	
	// add an element to the queue
	// throw std::overflow if queue is full
	void push(const T& value) 
	{
		if(isFull()) throw std::overflow_error("boundedQueue is full");

		queue_[head_] = value;

		head_ = (head_ + 1) % N;
		count_++;
	}
	
	// pop and element from the queue
	// throw std::underflow if queue is empty
	void pop(T& returnVal)
	{
		if(isEmpty()) throw std::underflow_error("boundedQueue is empty");

		returnVal = queue_[tail_];

		tail_ = (tail_ + 1) % N;
		count_--;
	}
	
	// peak at the element to be removed
	// throw std::underflow if queue is empty
	const T& peek() const
	{
		if(isEmpty()) throw std::underflow_error("boundedQueue is empty");

		return queue_[tail_];
	}

	void clear() noexcept
	{
		head_ = 0;
		tail_ = 0;
		count_ = 0;
	}
	
	// helper functions for checking queue status
	inline bool isFull() const noexcept { return count_ == N; }
	inline bool isEmpty() const noexcept { return count_ == 0; }
	
	inline std::size_t size() const noexcept { return count_; }

private:
	std::array<T , N> queue_;

	std::size_t head_;
	std::size_t tail_;
	std::size_t count_;
};

