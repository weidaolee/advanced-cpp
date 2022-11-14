#ifndef UNORDERED_MAP_H_
#define UNORDERED_MAP_H_
#include <algorithm>
#include <cstddef>
#include <functional>
#include <utility>
#include <list>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <memory>
#include <thread>
#include <future>

// class K {
//     public:
//     bool operator==(const K&) const = default;
// };
// class V {};
// template<typename K, typename V, typename Hash=std::hash<K>>
// class HashMap;

// template<>
// struct std::hash<K> {
//     std::size_t operator()(K const &k) const noexcept {
//         return 0;
//     };
// };

// using Hash = std::hash<K>;

template<typename K, typename V, typename Hash=std::hash<K>>
class ConcurrentHashMap {
private:
    class Bucket {
    private:
        using Pair = std::pair<K, V>;
        using List = std::list<Pair>;
        using Iter = typename List::iterator;
        List list;
        Iter find(const K &k)  {
            return std::find_if(list.begin(), list.end(), [&](const Pair &pair) -> bool {return k == pair.first;});
        };

    public:
        mutable std::shared_mutex m_;
        std::shared_ptr<V> get(const K &k){
            std::shared_lock slock(m_);
            auto it = find(k);
            return it == list.end() ? nullptr : std::make_shared<V>(it->second);
        };

        void set(const K& k, const V &v) {
            std::unique_lock xlock(m_);
            auto it = find(k);
            if (it == list.end()) {
                list.push_back(Pair(k, v));
            } else {
                it->second = v;
            }
        }
        std::shared_ptr<V> remove(const K& k) {
            std::unique_lock xlock(m_);
            auto it = find(k);
            if (it == list.end()) {
                return nullptr;
            } else {
                auto p = std::make_shared<V>(it->second);
                list.erase(it);
                return p;
            }
        }
        void dump() {

        }

    };
private:
    std::vector<std::unique_ptr<Bucket>> buckets;
    Hash hasher;
    Bucket& get_bucket(const K &k) {
        const auto i = hasher(k) % buckets.size();
        return *buckets[i];
    };
public:
    explicit ConcurrentHashMap(size_t num_buckets = 19) : buckets(num_buckets) {
        for (int i = 0; i < buckets.size(); i++) {
            buckets[i].reset(new Bucket);
        }
    };
    ConcurrentHashMap(const ConcurrentHashMap&) = delete;
    ConcurrentHashMap& operator=(const ConcurrentHashMap&) = delete;

    std::shared_ptr<V> get(const K &k) {
        return get_bucket(k).get(k);
    };
    void set(const K &k, const V &v) {
        get_bucket(k).set(k, v);
    };
    std::shared_ptr<V> remove(const K &k) {
        return get_bucket(k).remove(k);
    };
    void dump() {
        std::vector<std::shared_lock<std::shared_mutex>> locks; locks.reserve(buckets.size());
        std::vector<std::future<void>> tasks; tasks.reserve(buckets.size());
        for (int i = 0; i < buckets.size(); i++) {
            locks.push_back(std::shared_lock(buckets[i]->m_));
        }
        for (int i = 0; i < buckets.size(); i++) {
            tasks.push_back(std::async([this, i]{buckets[i]->dump();}));
        }
        for (int i = 0; i < buckets.size(); i++) {
            tasks[i].wait();
        }
    }
};
#endif // UNORDERED_MAP_H_
