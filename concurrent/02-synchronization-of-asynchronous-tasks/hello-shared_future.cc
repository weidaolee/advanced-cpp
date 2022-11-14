#include <iostream>
#include <mutex>
#include <ostream>
#include <unordered_map>
#include <string>
#include <thread>
#include <future>
#include <vector>

class Table {
    private:
        mutable std::mutex m_;
        std::string table_name;
        std::vector<std::string> table;
    public:
        Table(const std::string table_name) : table_name(table_name) {};
        std::string get_name() { return table_name;};
        void insert(std::string data) {
            std::lock_guard<std::mutex> lk(m_);
            table.push_back(data);
        };
};

class DB {
    private:
        mutable std::mutex m_;
    public:
        std::unordered_map<std::string, Table*> tables;
        void create_table(const std::string table_name) {
            std::lock_guard<std::mutex> lk(m_);
            if (tables.find(table_name) == tables.end()) {
                tables[table_name] = new Table(table_name);
            }
        };
};

int main() {
    std::promise<void> connect_db_promise;
    std::shared_future<void> connect_db_future(connect_db_promise.get_future());
    DB *db = nullptr;

    auto open_db = [&] {
        db = new DB();
        connect_db_promise.set_value();
    };

    auto create_table = [&](const std::string table_name) {
        connect_db_future.wait();
        db->create_table(table_name);
    };

    auto t1 = std::thread(create_table, "T1");
    auto t2 = std::thread(create_table, "T2");
    open_db();
    t1.join();
    t2.join();

    for (auto &[key, table] : db->tables) {
        std::cout << table->get_name() << std::endl;
    }
    delete db;
    return 0;
}
