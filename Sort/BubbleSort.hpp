#ifndef BubbleSort_hpp
#define BubbleSort_hpp

template <typename T> //向量的起泡排序（跳跃版）
void bubbleSort( T* _elem, int lo, int hi ){ //assert: 0 <= lo < hi <= size
    for( int last = --hi; lo < hi; hi = last )
        for( int i = last = lo; i < hi; i++ )
            if( _elem[i] > _elem[i + 1] ){ //若逆序，则
                T tmp = _elem[ last = i ];
                _elem[i] = _elem[i + 1];
                _elem[ i + 1 ] = tmp; //经交换使局部有序
            }
}

#endif