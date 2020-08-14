#ifndef QuickSort_hpp
#define QuickSort_hpp

#include <cstdlib>

template <typename T>
void _swap( T& a, T& b){
    T tmp = a;
    a = b;
    b = tmp;
}

template <typename T> //轴点构造算法：通过调整元素位置构造区间[lo, hi)的轴点，并返回其秩
int partition ( T* _elem, int lo, int hi ) {
    _swap ( _elem[lo], _elem[ lo + rand() % ( hi - lo ) ] ); //任选一个元素与首元素交换
    T pivot = _elem[lo]; //以首元素为候选轴点——经以上交换，等效于随机选取
    int mi = lo;
    for ( int k = lo + 1; k < hi; k++ ) //自左向右扫描
        if ( _elem[k] < pivot ) //若当前元素_elem[k]小于pivot，则
            _swap ( _elem[++mi], _elem[k] ); //将_elem[k]交换至原mi之后，使L子序列向右扩展
    _swap ( _elem[lo], _elem[mi] ); //候选轴点归位
    return mi; //返回轴点的秩
}

template <typename T> //快速排序
void quickSort ( T* _elem, int lo, int hi ) { //0 <= lo < hi <= size
    if ( hi - lo < 2 ) return; //单元素区间自然有序，否则...
    int mi = partition ( _elem, lo, hi ); //在[lo, hi)内构造轴点
    quickSort ( _elem, lo, mi ); //对前缀递归排序
    quickSort ( _elem, mi + 1, hi ); //对后缀递归排序
}

#endif