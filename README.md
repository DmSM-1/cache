# Cache
My realisation lfu and perfect caches

## Overview
In this project, I have realised two algorithms of caching:

1) LFU (Last Frequently Used)

2) Perfect Cache: In this cache, the element that is furthest away in the input stream or is missing from it is evicted

## Build

```
git clone https://github.com/DmSM-1/cache.git

cd cache

mkdir build

cd build

cmake CMakeLists.txt

make
```

to start the LFU cache:
`build/./lfu_cache`

to start the perfect cache:
`build/./prf_cache`

## Testing
If you want to create new tests, you can use test_gen.py for do it

Example:

```
python3 test/test_gen.py test/new_test_dir
```

Where new_test_dir is name of your test directory

There are also several ready-to-use tests in test/tests. 
The tests are common to Perfect and LFU caches, but contain different answers.
Here you can see that Perfect Cache has more hits than LFU.

For testing use test.py

Example:
```
python3 test/test.py build/flu_cache test/tests

python3 test/test.py build/prf_cache test/tests -p
```
>note: using flag `-p` in the end for prf_cache





