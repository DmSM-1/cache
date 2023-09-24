import os
import sys
from random import randint


MAX_BUFFER_SIZE = 256
MAX_INPUTS      = MAX_BUFFER_SIZE * 16
MAX_INTEGER     = 256
TESTS           = 25


def calculate_lfu_cache_hits(buffer_size, input_size, input_buffer)->int:
    if len(input_buffer) != input_size:
        return -1 
    hits = 0
    cache = []
    
    for i in input_buffer:
        if len(cache) == buffer_size and i not in [j[0] for j in cache]:
            cache.pop()
        if i not in [j[0] for j in cache] or len(cache) == 0:
            cache.append([i, 1])
        else:
            cache[[j[0] for j in cache].index(i)][1] += 1
            hits +=1
        cache = sorted(cache, key = lambda x: x[1], reverse=True)
    return hits


def calculate_perfect_cache_hits(buffer_size, input_size, input_buffer)->int:
    if len(input_buffer) != input_size:
        return -1 
    hits = 0
    cache = []

    for i in range(len(input_buffer)):
        if input_buffer[i] in [j[0] for j in cache]: 
            hits += 1
        elif len(cache) == buffer_size: 
            cache[-1] = [input_buffer[i], 0]
        else:
            cache.append([input_buffer[i], 0])
        for j in range(len(cache)):
            if cache[j][0] not in input_buffer[i+1::]:
                cache[j][1] = len(input_buffer[i+1::])
            else:
                cache[j][1] = input_buffer[i+1::].index(cache[j][0])
        cache = sorted(cache,key = lambda x: x[1])

    return hits


if __name__ == '__main__':
    if len(sys.argv) != 2: 
        exit("Usage: python script.py RUN_FILE TEST_DIR")
        
    test_dir = sys.argv[1]
    os.makedirs(test_dir, exist_ok = True)

    for i in range(TESTS):
        buffer_size, input_size = randint(1, MAX_BUFFER_SIZE), randint(1, MAX_INPUTS)
        ampl = randint(1, MAX_INTEGER)
        input_buffer = [randint(-ampl, ampl) for i in range(input_size)]

        file_name = os.path.join(test_dir, f"test_{i}_ans_{calculate_lfu_cache_hits(buffer_size, input_size, input_buffer)}_{calculate_perfect_cache_hits(buffer_size, input_size, input_buffer)}")

        with open(file_name, 'w') as f:
            f.write(f"{buffer_size} {input_size} ")
            f.write(' '.join([str(i) for i in input_buffer]))

    sys.exit(0)