#include "PQ_ComplHeap.h"

template <typename T>
void PQ_ComplHeap<T>::insert ( T e ) { //将词条插入完全二叉堆中
    Vector<T>::insert ( e ); //首先将新词条接至向量末尾
    percolateUp ( this->_elem, this->_size - 1 ); //再对该词条实施上滤调整
}

template <typename T>
T PQ_ComplHeap<T>::getMax(){ return this->_root->data; } //按照此处约定，堆顶即优先级最高的词条

template <typename T>
T PQ_ComplHeap<T>::delMax() { //删除非空完全二叉堆中优先级最高的词条
    T maxElem = this->_elem[0]; this->_elem[0] = this->_elem[ --(this->_size) ]; //摘除堆顶（首词条），代之以末词条
    percolateDown ( this->_elem, this->_size, 0 ); //对新堆顶实施下滤
    return maxElem; //返回此前备份的最大词条
}