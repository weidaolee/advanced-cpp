#include <atomic>

class T {};
std::atomic<T> x;

int main() {
    std::atomic<int> x;
    return 0;
}
