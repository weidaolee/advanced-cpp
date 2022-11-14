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
    std::cout << "do something..." << std::endl;
}

int main() {
    std::string data = "hello";
    std::future<std::string> task = std::async(async_task, std::ref(data));
    do_something();
    std::cout << task.get() << std::endl;
    return 0;
}
