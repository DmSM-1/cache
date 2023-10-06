#include <cassert>
#include <iostream>
#include <vector>
#include <cassert>
#include "Perfect_Cache.hpp"


Perfect::Cache_Page<int, int> slow_get_page(int key, int next){
    Perfect::Cache_Page<int, int> Page{key, next};
    return Page;
}


int main(){
    size_t buffer_size;
    size_t input_size;
    int elem_id;

    std::cin >> buffer_size >> input_size;
    assert(std::cin.good());

    std::vector<int> input_buffer;

    Perfect::GetPagesKeys(input_size, input_buffer);
    Perfect::P_Cache<int, int> cache(buffer_size, input_size, input_buffer);

    int hits = 0;
    for(int i = 0; i < input_size; i++){
        hits += cache.LookUpdate(input_buffer[i], slow_get_page);
    }

    std::cout << "Hits:" << hits << "\n";
}