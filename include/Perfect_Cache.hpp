#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <unordered_map>
#include <queue>


namespace Perfect{


template<typename key_t, typename page_t>
struct Cache_Page {
        key_t key;
        int next;
        page_t Page;
    };


template<typename key_t, typename page_t>
class P_Cache{
private:
    size_t buffer_size;
    size_t input_size;
    int elemscount = 0;

    using ListIt = typename std::list<Cache_Page<key_t, page_t>>::iterator;
    
    std::list<Cache_Page<key_t, page_t>> cache; 
    std::unordered_map<key_t, ListIt> hashmap; 
    std::unordered_map<key_t, std::queue<int>> indexmap;
    ListIt in_cache; //stores an iterator pointing to the currently found element


    // The CreateIndexMap creak hashmap of queues with indexes of element in the input stream
    void CreateIndexMap(const int& input_size, std::vector<key_t>& input){
        for(int i = 0; i<input_size; i++){
            key_t key = input[i];

            auto elem = indexmap.find(key);
            if (elem == indexmap.end()){
                indexmap[key].push(i);
            }
            else
                elem->second.push(i);
        }
    }

    // The GetNextIndex function get the next index of an element from the input stream
    // if there are no more indexes with this key, it returns 'input_size'
    int GetNextIndex(const key_t& key){
        auto& positions = indexmap.find(key)->second;    
        positions.pop();
    
        if (positions.empty())
            return input_size;

        return positions.front();
    }

    
    bool Contains(const key_t& key) {
        if (!elemscount)
            return false;

        auto elem = hashmap.find(key);
        if (elem == hashmap.end())
            return false;

        in_cache = elem->second;
        return true;
    }


    void Add(const key_t& key, Cache_Page<key_t, page_t> slow_get_page(key_t, int)){
        if (elemscount == buffer_size) {
            hashmap.erase(cache.back().key);
            cache.pop_back();
        }
        else
            elemscount++;

        cache.push_back(slow_get_page(key, 0));
        hashmap[key] = std::prev(cache.end()); 
        auto elem = hashmap.find(key);
        in_cache = elem->second;
    }


    // The Raise function updates the position of an element in the cache in case of a hit or when adding a new element
    void Raise() {
        in_cache->next = GetNextIndex(in_cache->key);
        cache.splice(cache.end(), cache, in_cache);
        int next = in_cache->next;

        ListIt i = in_cache;
        
        for(i--; i != cache.begin() && i->next>next; i--);
        
        if (i->next <= next)
            i++; 
        
        cache.splice(i, cache, in_cache);
    }  

public:
    P_Cache(int bs, int is, std::vector<key_t> input) : buffer_size(bs) , input_size(is){
        CreateIndexMap(is, input);
    }


    ~P_Cache(){}


    //The LookUpdate function searches for an element in the cache or adds it if not found
    bool LookUpdate(const key_t& key, Cache_Page<key_t, page_t> slow_get_page(key_t, int)){
        bool contains = Contains(key);
        if(!contains)
            Add(key, slow_get_page);
        Raise();

        return contains;
    }


    void PrintInfo() {
        std::cout << "______________________________\n";
        std::cout << "Class: LFU_CACHE\n";
        std::cout << "Buffer size:" << buffer_size << '\n';
        std::cout << "Count of elements:" << elemscount << '\n';

        std::cout<<'[';
        for (ListIt i = cache.begin(); i!=cache.end(); i++){
            std::cout<<'('<<i->key<<','<<i->next<<"), ";
        }
        std::cout<<"]\n";
        std::cout << "______________________________\n";
    }
};



// The GetPagesKeys function reads page keys from the input stream and stores them in the 'input_buffer' vector.
void GetPagesKeys(size_t& inputsize, std::vector<key_t>& input_buffer){
    int num;
    for(int i = 0; i<inputsize; i++){
        std::cin >> num;
        input_buffer.push_back(num);
    }
}

}//namespace Perfect