#ifndef __THREADSAFE_STACK_H
#define __THREADSAFE_STACK_H

#include <mutex>
#include <stack>
#include <exception>
#include <memory>

template<typename E>
class threadsafe_stack {
    struct empty_stack: std::exception {
        const char* what() const noexcept {return "stack empty";};
    };

    private:
        std::stack<E> stack;
        mutable std::mutex m_;
    public:
        threadsafe_stack() {};
        threadsafe_stack(const threadsafe_stack& other) {
            std::lock_guard<std::mutex> guard(other.m_);
            stack = other.stack;  // move assign
        };
        threadsafe_stack& operator=(const threadsafe_stack&) = delete;
        void push(const E &element) {
            std::lock_guard<std::mutex> guard(m_);
            stack.push(element);
        };
        std::shared_ptr<E> pop() {
            std::lock_guard<std::mutex> guard(m_);
            if (stack.empty()) {
                throw empty_stack();
            }
            const std::shared_ptr<E> p {std::make_shared<E>(stack.top())};
            stack.pop();
            return p;
        };
        void pop(E* ptr) {
            std::lock_guard<std::mutex> guard(m_);
            if (stack.empty()) {
                throw empty_stack();
            }
            ptr = &stack.top();
            stack.pop();
        };
        bool empty() const {
            std::lock_guard<std::mutex> guard(m_);
            return stack.empty();
        };

};
#endif
