#include <iostream>
#include <cassert>
#include "LFU_Cache.hpp"


LFU::LFU_Page<int, int> slow_get_page(int key){
    LFU::LFU_Page<int, int> Page {key, 1};
    return Page;
}


int main(){
    size_t buffer_size;
    size_t input_size;
    int elem_id;

    std::cin >> buffer_size >> input_size;
    assert(std::cin.good());

    LFU::LFU_Cache<int, int> cache(buffer_size);

    int hits = 0;
    for(int i = 0; i < input_size; i++){
        std::cin >> elem_id;
        hits += cache.LookUpdate(elem_id, slow_get_page);
    }

    std::cout << "Hits:" << hits << '\n';
}