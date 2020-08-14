#ifndef MergeSort_hpp
#define MergeSort_hpp

template <typename T> //有序向量（区间）的归并
void merge ( T* _elem, int lo, int mi, int hi ) { //各自有序的子向量[lo, mi)和[mi, hi)
    T* A = _elem + lo; //合并后的向量A[0, hi - lo) = _elem[lo, hi)
    int lb = mi - lo;
    T* B = new T[lb]; //前子向量B[0, lb) = _elem[lo, mi)
    for ( int i = 0; i < lb; i++ ) B[i] = A[i]; //复制前子向量
    int lc = hi - mi;
    T* C = _elem + mi; //后子向量C[0, lc) = _elem[mi, hi)
    for ( int i = 0, j = 0, k = 0; j < lb; ) //归并：反复从B和C首元素中取出更小者
        A[i++] = ( lc <= k || B[j] <= C[k] ) ? B[j++] : C[k++]; //将其归入A中
    delete [] B; //释放临时空间B
}

template <typename T> //归并排序
void mergeSort ( T* _elem, int lo, int hi ) { //0 <= lo < hi <= size
    if ( hi - lo < 2 ) return; //单元素区间自然有序，否则...
    int mi = ( lo + hi ) >> 1; //以中点为界
    mergeSort ( _elem, lo, mi ); mergeSort ( _elem, mi, hi ); //分别排序
    merge ( _elem, lo, mi, hi ); //归并
}

#endif