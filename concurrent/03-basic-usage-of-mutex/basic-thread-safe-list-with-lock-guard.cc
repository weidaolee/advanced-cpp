#include <list>
#include <mutex>

class thread_safe_list {
    private:
        std::mutex front_lock;
        std::mutex back_lock;
        std::list<int> list;

    public:
        void push_front(int val) {
            front_lock.lock();
            std::lock_guard<std::mutex> guard(front_lock);
            list.push_front(val);
        };
        void push_back(int val) {
            std::lock_guard<std::mutex> guard(back_lock);
            list.push_back(val);
        };
};
