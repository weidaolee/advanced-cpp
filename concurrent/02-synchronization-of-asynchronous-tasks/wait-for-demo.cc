#include <ctime>
#include <future>
#include <chrono>
#include <iostream>
#include <thread>
using namespace std::chrono_literals;

void io_task() {
    std::this_thread::sleep_for(10s);
};
void do_process_data() {
    std::cout << "do process io" << std::endl;
};

int main() {
    auto aio_read_task = std::async(io_task);
    while (aio_read_task.wait_for(1s) == std::future_status::timeout) {
        std::this_thread::yield();
    }
    do_process_data();
    return 0;
}
