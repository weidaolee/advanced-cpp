#include "hierarchical_mutex.h"
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>

void process_across_0_and_1();
void process_across_1_and_2();
int main() {
    auto t1 = std::thread(process_across_0_and_1);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    auto t2 = std::thread(process_across_1_and_2);
    t1.join();
    t2.join();
    return 0;
}

hierarchical_mutex level_0_mutex(0);
hierarchical_mutex level_1_mutex(1);
hierarchical_mutex level_2_mutex(2);
using Clock = std::chrono::steady_clock;

void process_across_0_and_1() {
    auto start = Clock::now();
    std::lock_guard<hierarchical_mutex> guard0(level_0_mutex);
    std::cout << "thread-id: " << std::this_thread::get_id()
              << " held level 0 mutex, spent "
              << std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - start).count()
              << " seconds" << std::endl;
    
    std::lock_guard<hierarchical_mutex> guard1(level_1_mutex);
    std::cout << "thread-id: " << std::this_thread::get_id()
              << " held level 1 mutex, spent "
              << std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - start).count()
              << " seconds" << std::endl;

    // simulate do something
    std::this_thread::sleep_for(std::chrono::seconds(10));

    std::cout << "thread-id: " << std::this_thread::get_id()
              << " process_across_0_and_1_task spent: "
              << std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - start).count()
              << " seconds" << std::endl << std::endl;
};

void process_across_1_and_2() {
    auto start = Clock::now();
    std::lock_guard<hierarchical_mutex> guard1(level_1_mutex);
    std::cout << "thread-id: " << std::this_thread::get_id()
              << " held level 1 mutex, spent "
              << std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - start).count()
              << " seconds" << std::endl;

    std::lock_guard<hierarchical_mutex> guard2(level_2_mutex);
    std::cout << "thread-id: " << std::this_thread::get_id()
              << " held level 2 mutex, spent "
              << std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - start).count()
              << " seconds" << std::endl;

    // simulate do something
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "thread-id: " << std::this_thread::get_id()
              << " process_across_1_and_2_task spent: "
              << std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - start).count()
              << " seconds" << std::endl << std::endl;
};
