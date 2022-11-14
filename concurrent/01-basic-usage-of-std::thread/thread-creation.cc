#include <thread>

void func();

class Task {
    public:
        void operator()() const {
            // do something...
        };
};

int main(int argc, char *argv[]) {

    //std::thread my_thread {Task()};
    auto my_thread = std::thread(Task());
    my_thread.join();
    return 0;
}
