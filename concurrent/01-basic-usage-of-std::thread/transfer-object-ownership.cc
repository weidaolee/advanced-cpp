#include <memory>
#include <thread>

class big_object {};
void proccess_big_object(std::unique_ptr<big_object> p);
void demo() {
    auto p = std::unique_ptr<big_object>(new big_object());
    auto my_thread = std::thread(proccess_big_object, std::move(p));
    my_thread.detach();
};
