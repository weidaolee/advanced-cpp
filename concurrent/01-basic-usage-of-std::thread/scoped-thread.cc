#include <thread>
#include <stdexcept>

class scoped_thread {
    public:
        scoped_thread(std::thread t) : t(std::move(t)) {
            if (!t.joinable()) {
                throw std::logic_error("not joinable.");
            }
        };
        scoped_thread(const scoped_thread &other) = delete;
        scoped_thread& operator=(const scoped_thread &other) = delete;


        scoped_thread(scoped_thread &&other) noexcept = default;

        virtual ~scoped_thread() noexcept {
            t.join();
        };

    private:
        std::thread t;
};
