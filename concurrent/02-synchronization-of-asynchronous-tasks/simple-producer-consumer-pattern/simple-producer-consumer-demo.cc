#include "Data.h"

#include <iostream>
#include <mutex>
#include <ostream>
#include <thread>
#include <condition_variable>
#include <deque>

std::deque<Data> data_que;
std::mutex que_mutex;
std::condition_variable que_not_empty;

void prepare_data(int num_data) {
    while (num_data-- > 0) {
        auto data = Data();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        {
            std::lock_guard<std::mutex> guard(que_mutex);
            data_que.push_back(data);
        }
        std::cout << "thread: " << std::this_thread::get_id()
                  << " produce data id: " << data.get_id()
                  << std::endl;
        que_not_empty.notify_one();
    }
    std::cout << "producer done." << std::endl;
}

void process_data() {
    while (true) {
        std::unique_lock<std::mutex> lk(que_mutex);
        que_not_empty.wait(lk, [&]{return !data_que.empty();});
        auto &data = data_que.front(); data_que.pop_front();
        lk.unlock();
        data.process();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "thread: " << std::this_thread::get_id()
                  << " consume data id: " << data.get_id()
                  << ", info: " << data.get_info()
                  << std::endl;
    }
}

int main(int argc, char *argv[]) {
    auto t1 = std::thread(prepare_data, 10);
    auto t2 = std::thread(process_data);
    t1.join();
    t2.join();
    return 0;
}
