#include <mutex>
#include <list>

class thread_safe_list {
    private:
        std::mutex front_lock;
        std::mutex back_lock;
        std::list<int> list;
    public:
        void push_front(int val) {
            front_lock.lock();
            list.push_front(val);
            front_lock.unlock();
        };
        void push_back(int val) {
            back_lock.lock();
            list.push_back(val);
            back_lock.unlock();
        };
};
