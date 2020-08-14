#ifndef SelectionSort_hpp
#define SelectionSort_hpp

template <typename T>
int max( T* _elem, int lo, int hi ){ //在[lo, hi]内找出最大者
    int mx = hi;
    while(lo < hi--)mx = _elem[mx]<_elem[hi] ? hi:mx; //在max有多个时保证后者优先，进而保证selectionSort稳定
    return mx;
}

template <typename T> //选择排序
void selectionSort ( T* _elem, int lo, int hi ) { //0 <= lo < hi <= size
    while ( lo < --hi ){
        int mx = max ( _elem, lo, hi );
        T tmp = _elem[hi];
        _elem[hi] = _elem[mx];
        _elem[mx] = tmp; //将[hi]与[lo, hi]中的最大者交换
    }
}

#endif