#ifndef __THREADSAVE_DEQUE_H__
#define __THREADSAVE_DEQUE_H__
#include <condition_variable>
#include <queue>
#include <mutex>
#include <memory>
template<typename E>
class ThreadSafeQueue {
    private:
        mutable std::mutex m_;
        std::queue<E> q;
        std::condition_variable not_empty;
    public:
        ThreadSafeQueue() {};
        ThreadSafeQueue(const ThreadSafeQueue &other) {
            std::lock_guard<std::mutex> guard(other.m_);
            q = other.q;
        };

        virtual ~ThreadSafeQueue() noexcept = default;

        void push(const E &element) {
            std::lock_guard<std::mutex> guard(m_);
            q.push(element);
            not_empty.notify_one();
        };

        std::shared_ptr<E> pop() {
            std::lock_guard<std::mutex> guard(m_);
            not_empty.wait(m_, [this]{ return !q.empty(); });

            std::shared_ptr<E> ptr = std::shared_ptr<E>(q.front());
            q.pop();
            return ptr;
        };

        bool try_pop(E *ptr) {
            std::lock_guard<std::mutex> guard(m_);
            if (q.empty()) {
                return false;
            }
            ptr = &q.front();
            q.pop();
            return true;
        }

        bool empty() const {
            std::lock_guard<std::mutex> guard(m_);
            return q.empty();
        };
};
#endif // __THREADSAVE_DEQUE_H__
