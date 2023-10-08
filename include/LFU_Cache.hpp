#include <iostream>
#include <list>
#include <iterator>
#include <unordered_map>

namespace LFU{

template<typename key_t, typename page_t>
struct LFU_Page {
        key_t key;
        int freq;  
        typename std::list<std::list<LFU_Page<key_t, page_t>>>::iterator L2It; 
    };


template<typename key_t, typename page_t> 
class LFU_Cache { 
private:
    size_t copacity= 0;
    int elemscount = 0;

    using L1Cache = typename std::list<LFU_Page<key_t, page_t>>;
    using L2Cache = typename std::list<L1Cache>;
    using L2It = typename L2Cache::iterator;
    using L1It = typename L1Cache::iterator;
    
    std::unordered_map<key_t, L1It> hashmap;
    L2Cache cache;
    L1It in_cache;

    
    bool Contains(const key_t& key) {
        if (!elemscount)
            return false;

        auto elem = hashmap.find(key);
        if (elem == hashmap.end()){
            return false;
        }

        in_cache = elem->second;
        return true;
    }


    void Add(const key_t& key, LFU::LFU_Page<key_t, page_t> slow_get_page(key_t)){
        if (elemscount == copacity) {
            hashmap.erase(cache.back().back().key);
            cache.back().pop_back();
            elemscount--;
        }
        if (cache.back().empty())
            cache.pop_back();

        if (cache.empty() || (cache.back().back().freq != 1))
            cache.push_back(L1Cache());

        elemscount++;

        auto page = slow_get_page(key);
        page.L2It = std::prev(cache.end());

        cache.back().push_back(page);
        hashmap[key] = std::prev(cache.back().end()); 
    }

    void Raise(){
        L2It old_list = (*in_cache).L2It;

        if (old_list == cache.begin() || (*std::prev(old_list)).front().freq != in_cache->freq + 1)
            cache.insert(old_list,L1Cache());

        L2It new_list = std::prev(old_list);

        LFU_Page<key_t, page_t> page = {in_cache->key, in_cache->freq+1, new_list};
        (*new_list).push_back(page);

        hashmap[page.key] = std::prev((*new_list).end());
        (*old_list).erase(in_cache);

        if ((*old_list).empty())
            cache.erase(old_list);
    }


public:
    LFU_Cache(int bs) : copacity(bs){}


    bool LookUpdate(const key_t& key, LFU::LFU_Page<int, int> slow_get_page(key_t)){
        if(!Contains(key)){
            Add(key, slow_get_page);
            return false;
        }
        Raise();
        return true;
    }


    void Dump(){
        std::cout << "______________________________\n";
        std::cout << "Class: LFU_CACHE\n";
        std::cout << "Copacity:" << copacity << '\n';
        std::cout << "Count of elements:" << elemscount << '\n';
        std::cout << "Count of lists:" << cache.size() << '\n';

        for(L2It i = cache.begin(); i!= cache.end(); i++){
            if ((*i).empty()){
                std::cout << "empty\n";
                continue;
            }
            std::cout << (*i).front().freq << " " << (*i).size() << ": ";
            for(L1It j = (*i).begin(); j != (*i).end(); j++){
                std::cout << (*j).key << ' ';
            }
            std::cout << '\n'; 
        }
        std::cout << "______________________________\n";
}
};

}