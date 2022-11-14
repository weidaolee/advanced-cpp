#include <thread>
#include <chrono>
#include <iostream>

void func_A() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "end of func A" << std::endl;
};

void func_B() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "end of func B" << std::endl;
}

int main() {
    auto t1 = std::thread(func_A);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    t1 = std::thread(func_B);
    t1.join();
    return 0;
}
