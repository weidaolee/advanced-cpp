#include <exception>
#include <future>
#include <iostream>
#include <ostream>
#include <thread>

void divide_task(std::promise<double> &promise, double a, double b) {
    try {
        promise.set_value(a / b);
    } catch (...) {
        promise.set_exception(std::current_exception());
    }
};

int main() {
    std::promise<double> promise;
    auto t = std::thread(divide_task, std::ref(promise), 1, 0);
    t.detach();
    auto f = promise.get_future();
    try {
        std::cout << f.get() << std::endl;
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
