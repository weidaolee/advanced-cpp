#ifndef __HIERARCHICAL_MUTEX_H__
#define __HIERARCHICAL_MUTEX_H__
#include <mutex>

class hierarchical_mutex {
    public:
        explicit hierarchical_mutex(long long mutex_hierarchy);
        virtual ~hierarchical_mutex() noexcept = default;

        void lock();
        void unlock();
        bool try_lock();

    protected:
    private:
        mutable std::mutex m_;
        long long const mutex_hierarchy;
        long long prev_hierarchy;
        static thread_local long long thread_held_hierarchy;
        void check_for_hierarchy();
        void update_hierarchy();
};
#endif // __HIERARCHICAL_MUTEX_H__
