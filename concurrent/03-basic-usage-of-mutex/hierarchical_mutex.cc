#include "hierarchical_mutex.h"
#include <climits>
#include <stdexcept>

thread_local long long hierarchical_mutex::thread_held_hierarchy = LLONG_MIN;

hierarchical_mutex::hierarchical_mutex(long long mutex_hierarchy) :
    mutex_hierarchy(mutex_hierarchy), prev_hierarchy(LLONG_MAX) {};

void hierarchical_mutex::check_for_hierarchy() {
    if (thread_held_hierarchy >= mutex_hierarchy) {
        throw std::logic_error("hierarchical_mutex permission denied");
    }
};

void hierarchical_mutex::update_hierarchy() {
    prev_hierarchy = thread_held_hierarchy;
    thread_held_hierarchy = mutex_hierarchy;
};

void hierarchical_mutex::lock() {
    check_for_hierarchy();
    m_.lock();
    update_hierarchy();
};

void hierarchical_mutex::unlock() {
    if (mutex_hierarchy != thread_held_hierarchy) {
        throw std::logic_error("hierarchic_mutex unlock violated");
    }
    thread_held_hierarchy = prev_hierarchy;
    m_.unlock();
}

bool hierarchical_mutex::try_lock() {
    check_for_hierarchy();
    if (!m_.try_lock()) {
        return false;
    }
    update_hierarchy();
    return true;
}
