#include <future>

class ReturnType {};
ReturnType async_task();
void do_something();
void process_result(ReturnType &result);

void demo() {
    std::future<ReturnType> task = std::async(async_task);
    do_something();
    ReturnType reslut = task.get();
    process_result(reslut);
}

