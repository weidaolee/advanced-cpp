#include <thread>
#include <future>

int func();
void process_result(int result);

int main() {
    std::packaged_task<int(void)> f(func);
    auto task = f.get_future();
    auto my_thread = std::thread(f);
    my_thread.detach();
    int result = task.get();
    process_result(result);
    return 0;
}
