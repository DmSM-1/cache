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
    size_t elemscount = 0;
    
    using ListIt = typename std::list<Cache_Page<key_t, page_t>>::iterator;
    
    std::list<Cache_Page<key_t, page_t>> cache; 
    std::unordered_map<key_t, ListIt> hashmap; 
    std::unordered_map<key_t, std::queue<int>> indexmap;
    std::vector<ListIt> indexvect;
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


    int Bin(int index){
        int low = 0;
        int high = elemscount - 1;
        int mean = 0;

        while (high - low > 1){
            mean = (high+low)/2;
            if (indexvect[mean]->next > index)
                high = mean;
            else
                low = mean;
        }
        if (indexvect[low]->next >= index)
            return low;
        return high;
        
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


    int Add(const key_t& key, Cache_Page<key_t, page_t> page, bool new_elem = false){
        
        int next = GetNextIndex(key);
        if (next == input_size)
            return 1;
        page.next = next;

        if (!elemscount || page.next < cache.front().next){
            cache.push_front(page);
            indexvect.insert(indexvect.begin(),cache.begin());
            hashmap[key] = cache.begin();
        }else{
            int pos = Bin(next);
            if (indexvect[pos]->next > next){
                cache.insert(indexvect[pos], page);
                indexvect.insert(indexvect.begin()+pos, std::prev(indexvect[pos]));
                hashmap[key] = (indexvect[pos]);
            }else{
                cache.insert(std::next(indexvect[pos]), page);
                indexvect.insert(indexvect.begin()+pos+1, std::next(indexvect[pos]));
                hashmap[key] = std::next(indexvect[pos]);
            }
        }
        elemscount++;

        if (elemscount > buffer_size){
            hashmap.erase(cache.back().key);
            cache.pop_back();
            indexvect.pop_back();
            elemscount--;
        }
        return 0;
    }


    Cache_Page<key_t, page_t> Remove(key_t key){
        int pos = Bin(in_cache->next);
        Cache_Page<key_t, page_t> page = *indexvect[pos];
        cache.erase(indexvect[pos]);
        indexvect.erase(indexvect.begin()+pos);
        elemscount--;
        return page;
    }  

public:
    P_Cache(int bs, int is, std::vector<key_t> input) : buffer_size(bs) , input_size(is){
        CreateIndexMap(is, input);
    }

    //The LookUpdate function searches for an element in the cache or adds it if not found
    bool LookUpdate(const key_t& key, Cache_Page<key_t, page_t> slow_get_page(key_t, int)){
        bool contains = Contains(key);
        if(!contains)
            Add(key, slow_get_page(key, 0), true);
        else
            Add(key, Remove(key));

        return contains;
    }


    void Dump() {
        std::cout << "______________________________\n";
        std::cout << "Class: LFU_CACHE\n";
        std::cout << "Buffer size:" << buffer_size << '\n';
        std::cout << "Count of elements:" << elemscount << '\n';

        std::cout<<'[';
        for (ListIt i = cache.begin(); i!=cache.end(); i++){
            std::cout<<'('<<i->key<<','<<i->next<<"), ";
        }
        std::cout<<"]\n";

        std::cout<<'[';
        for (int i = 0; i<indexvect.size(); i++){
            std::cout<<'('<<indexvect[i]->key<<','<<indexvect[i]->next<<"), ";
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
