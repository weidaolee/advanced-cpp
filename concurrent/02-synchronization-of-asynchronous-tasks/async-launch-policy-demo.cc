#include <functional>
#include <future>
#include <ostream>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>
#include <iostream>


std::string async_task(std::string &data) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::stringstream ss;
    ss << "thread id: " << std::this_thread::get_id() << " process data: " << data;
    return ss.str();
};

void do_something() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "do something..." << std::endl;
}

using Clock = std::chrono::steady_clock;

int main() {
    std::string data = "hello";
    auto start = Clock::now();
    std::future<std::string> task = std::async(std::launch::deferred, async_task, std::ref(data));
    do_something();
    std::cout << task.get() << std::endl;
    auto sec = std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - start).count();
    std::cout << "took " << sec << " seconds total" << std::endl;
    return 0;
}
