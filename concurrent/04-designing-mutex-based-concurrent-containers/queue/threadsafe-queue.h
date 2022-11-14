#ifndef __THREADSAFEQUEUE_H__
#define __THREADSAFEQUEUE_H__
#include <algorithm>
#include <condition_variable>
#include <memory>
#include <mutex>

template <typename T> class ThreadsafeQueue {
private:
    struct Node {
        std::shared_ptr<T> data;
        std::unique_ptr<Node> next;
        Node() = default;
        Node(const T &data) : data(std::make_shared<T>(data)), next(nullptr) {}
        ~Node() = default;
    };
    mutable std::mutex head_m_;
    mutable std::mutex tail_m_;
    std::unique_ptr<Node> head;
    Node *tail;
    std::condition_variable not_empty;

    Node *back() {
        std::lock_guard<std::mutex> lk(tail_m_);
        return tail;
    };

    std::unique_ptr<Node> pop_front() {
        std::unique_ptr<Node> p = std::move(head);
        head = std::move(head->next);
        return p;
    };

    std::unique_ptr<Node> try_pop_front() {
        if (head.get() == back()) {
            return nullptr;
        }
        return pop_front();
    }

    std::unique_lock<std::mutex> acquire_front() {
        std::unique_lock<std::mutex> head_lk(head_m_);
        not_empty.wait(head_lk, [this] { return head.get() == back(); });
        return head_lk; // std::move(head_lk);
    }

public:
    ThreadsafeQueue() : head(new Node), tail(head.get()) {}
    ThreadsafeQueue(const ThreadsafeQueue &other) = delete;
    ThreadsafeQueue &operator=(const ThreadsafeQueue &other) = delete;
    std::shared_ptr<T> try_pop() {
        std::unique_ptr<Node> p;
        {
            std::lock_guard<std::mutex> lk(head_m_);
            p = try_pop_front();
        }
        return p ? std::move(p->data) : nullptr;
    };
    std::shared_ptr<T> wait_and_pop() {
        auto head_lk = acquire_front();
        return std::move(pop_front()->data);
    };
    void push(const T &data) {
        std::unique_ptr<Node> p(new Node(data));
        {
            std::lock_guard<std::mutex> lk(tail_m_);
            tail->data = std::move(p->data);
            tail->next = std::move(p);
            tail = p.get();
        }
        not_empty.notify_one();
    };
    bool empty() {
        std::lock_guard<std::mutex> lk(head_m_);
        return head.get() == back();
    };
};

#endif // __THREADSAFEQUEUE_H__
