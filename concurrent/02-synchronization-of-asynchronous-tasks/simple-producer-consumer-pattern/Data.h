#ifndef __DATA_H__
#define __DATA_H__
#include <cstdlib>
#include <string>
#include <iostream>

class Data {
    private:
        int id;
        std::string info;
    public:
        explicit Data(int id, const std::string info);
        Data();
        Data(const Data &other) = default;
        Data(Data &&other) noexcept = default;
        virtual ~Data() noexcept = default;
        Data& operator=(const Data &other) = default;
        Data& operator=(Data &&other) noexcept = default;

        void process();
        int get_id() { return id; };
        const std::string get_info() { return info; };
};
#endif // __DATA_H__
