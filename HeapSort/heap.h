#ifndef HEAP_H__
#define HEAP_H__

#include <iterator>
#include <algorithm>

namespace 
{
    template <class RandomIt>
    void max_heap(RandomIt first, RandomIt last, RandomIt root)
    {
        RandomIt largest = root;
        RandomIt left = first + 2 * std::distance(first, root) + 1;
        RandomIt right = left + 1;

        if(left < last && *left > *largest)
            largest = left;
        if(right < last && *right > *largest)
            largest = right;
        if(largest != root)
        {
            std::iter_swap(root, largest);
            max_heap(first, last, largest);
        }
    }
}   // namespace

template <class RandomIt>
void make_heap(RandomIt first, RandomIt last)
{
    RandomIt it = first + std::distance(first, last) / 2 - 1;
   
    while(it >= first)
        max_heap(first, last, it--);
}

template <class RandomIt>
bool is_heap(RandomIt first, RandomIt last, RandomIt root)
{
    bool result = false;
    
    if(root > first + (std::distance(first, last) - 2) / 2)
        result = true;
    else
    {
        RandomIt left = first + 2 * std::distance(first, root) + 1;
        RandomIt right = left + 1;
        
        if(*root >= *left && *root >= *right && 
            is_heap(first, last, left) && is_heap(first, last, right))
            result = true;
    }
    
    return result;
}

template <class RandomIt>
void heap_sort(RandomIt first, RandomIt last)
{
    while(--last >= first)
    {
        std::iter_swap(first, last);
        max_heap(first, last, first);
    }
}

#endif
