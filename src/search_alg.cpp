#include "../inc/search_alg.hpp"

bool BinarySearch(int * arr, size_t right, int searchNum , size_t& index){
    if(0 == right || NULL == arr){
        return false;
    }
    bool res =  IterateSearch<int * , int>(arr , 0 , right , searchNum , index);
    return res;
}

