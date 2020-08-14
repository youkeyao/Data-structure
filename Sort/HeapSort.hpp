#ifndef HeapSort_hpp
#define HeapSort_hpp

#include "C:\Users\youykeyao\Desktop\code\VC\c++\PQ\PQ_ComplHeap\PQ_ComplHeap.hpp"

template <typename T>
void heapSort ( T* _elem, int lo, int hi ) { //0 <= lo < hi <= size
    T* A = _elem + lo;
    int n = hi - lo;
    heapify( A, n ); //将待排序区间建成一个完全二叉堆，O(n)
    while ( 0 < --n ){ //反复地摘除最大元并归入已排序的后缀，直至堆空
        T tmp = A[0];
        A[0] = A[n];
        A[n] = tmp;
        percolateDown( A, n, 0 ); //堆顶与末元素对换，再下滤
    }
}

#endif