#include <thread>

class thread_guard {
    public:

        explicit thread_guard(std::thread &t) : t(t) {};

        thread_guard(const thread_guard &other) = delete;
        thread_guard& operator=(const thread_guard  &other) = delete;
        thread_guard(thread_guard &&other) = default;
        

        ~thread_guard() noexcept {
            if (t.joinable()) {
                t.join();
            }
        };

    private:
        std::thread &t;
};

void func() {};
int main() {
    auto my_thread = std::thread(func);
    auto gurad = thread_guard(my_thread);
    try {
        // do something...
    } catch (...) {
        throw;
    }
    return 0;
}
