#pragma once
#include <memory>
#include <functional>
#include <type_traits>

template<typename T>
using handle = std::unique_ptr<T, std::function<void(T*)>>;

template<typename T, std::size_t N>
class memAlloc final {
    static_assert(N > 1, "Specify a valid size (must be greater than one)");

private:

    struct alignas(64) block_t {
        T obj_;
        block_t* next_;

        block_t() : next_(nullptr) {}
    };

    void* buffer_;
    block_t* freeList_;
    std::size_t freeCount_;

    void deallocate(block_t* block_p) {
        if constexpr(!std::is_trivially_destructible_v<T>) block_p->obj_.~T();
        block_p->next_ = freeList_;
        freeList_ = block_p;
        ++freeCount_;
    }

public:
    explicit memAlloc() : freeList_(nullptr), freeCount_(N) {
		buffer_ = std::aligned_alloc(alignof(block_t) , sizeof(block_t) * N);
		if(!buffer_) throw std::bad_alloc();

        for (std::size_t i = 0; i < N; ++i) {
            block_t* newBlock_p = reinterpret_cast<block_t*>(static_cast<char*>(buffer_) + i * sizeof(block_t));
            newBlock_p->next_ = freeList_;
            freeList_ = newBlock_p;
        }
    }

    ~memAlloc() { std::free(buffer_); }

    [[nodiscard]] handle<T> allocate() {
        if (__builtin_expect(freeCount_ == 0 , 0)) return nullptr;

        block_t* block_p = freeList_;
        freeList_ = freeList_->next_;
        --freeCount_;

        new (&block_p->obj_) T();
		return handle<T>(&block_p->obj_, [this, block_p](T* ptr) {(void)ptr; this->deallocate(block_p); });
    }

    template<typename ... Args>
    [[nodiscard]] handle<T> allocate(Args&& ... args) {
        if (__builtin_expect(freeCount_ == 0 , 0)) return nullptr;

        block_t* block_p = freeList_;
        freeList_ = freeList_->next_;
        --freeCount_;

        new (&block_p->obj_) T(std::forward<Args>(args)...);
		return handle<T>(&block_p->obj_, [this, block_p](T* ptr) {(void)ptr; this->deallocate(block_p); });
    }

    inline std::size_t capacity() const { return N; }

    inline std::size_t avail() const { return freeCount_; }

    inline std::size_t occupied() const { return N - freeCount_; }
};

