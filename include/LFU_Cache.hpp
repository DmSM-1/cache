#include <iostream>
#include <list>
#include <iterator>
#include <unordered_map>

namespace LFU{

    
template<typename key_t, typename page_t>
struct LFU_Page {
        key_t key;
        int freq;
        page_t Page;
    };


template<typename key_t, typename page_t> 
class LFU_Cache { 
private:
    size_t buffer_size = 0;
    int elemscount = 0;

    using ListIt = typename std::list<LFU_Page<key_t, page_t>>::iterator;
    
    std::list<LFU_Page<key_t, page_t>> cache;
    std::unordered_map<key_t, ListIt> hashmap;
    ListIt in_cache;
    

    bool Contains(const key_t& key) {
        if (!elemscount)
            return false;

        auto elem = hashmap.find(key);
        if (elem == hashmap.end())
            return false;

        in_cache = elem->second;
        return true;
    }


    void Add(const key_t& key, LFU::LFU_Page<int, int> slow_get_page(key_t)){
        if (elemscount == buffer_size) {
            hashmap.erase(cache.back().key);
            cache.pop_back();
        }
        else
            elemscount++;

        cache.push_back(slow_get_page(key));
        hashmap[key] = std::prev(cache.end()); 
    }


    // The Raise function updates the position of an element in the cache in case of a hit or when adding a new element
    void Raise() {
        auto elem = in_cache;
        elem->freq++;
        int freq = elem->freq;

        ListIt i = elem;
        for(i--; i != cache.begin() && i->freq<freq; i--);
        
        if (i != elem){
            if (i->freq >= elem->freq)
                i++; 
            
            cache.splice(i, cache, elem);
        }
    }   


public:
    LFU_Cache(int bs) : buffer_size(bs){}


    ~LFU_Cache(){}


    //The LookUpdate function searches for an element in the cache or adds it if not found
    bool LookUpdate(const key_t& key, LFU::LFU_Page<int, int> slow_get_page(key_t)){
        if(!Contains(key)){
            Add(key, slow_get_page);
            return false;
        }
        Raise();
        return true;
    }


    void PrintInfo() {
        std::cout << "______________________________\n";
        std::cout << "Class: LFU_CACHE\n";
        std::cout << "Buffer size:" << buffer_size << '\n';
        std::cout << "Count of elements:" << elemscount << '\n';

        std::cout<<'[';
        for (ListIt i = cache.begin(); i!=cache.end(); i++){
            std::cout<<'('<<i->key<<','<<i->freq<<"), ";
        }
        std::cout<<"]\n";
        std::cout << "______________________________\n";
    }
};

}//namespace LFU;