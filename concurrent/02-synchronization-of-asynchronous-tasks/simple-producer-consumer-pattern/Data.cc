#include "Data.h"

Data::Data(int id, const std::string info) : id(id), info(info) {};
Data::Data() : id(rand()), info(std::to_string(rand())) {};
void Data::process() {
    // std::cout << "id: " << id << ", info: " << info << std::endl;
};
