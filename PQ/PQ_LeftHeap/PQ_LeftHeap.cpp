#include "PQ_LeftHeap.h"

template <typename T>
void PQ_LeftHeap<T>::insert ( T e ) {
    this->_root = merge( this->_root, new BinNode<T>( e, NULL ) ); //将e封装为左式堆，与当前左式堆合并
    (this->_size)++; //更新规模
}

template <typename T>
T PQ_LeftHeap<T>::getMax()
{ return this->_root->data; } //按照此处约定，堆顶即优先级最高的词条

template <typename T> T PQ_LeftHeap<T>::delMax() {
    BinNode<T>* lHeap = this->_root->lc; if (lHeap) lHeap->parent = NULL; //左子堆
    BinNode<T>* rHeap = this->_root->rc; if (rHeap) rHeap->parent = NULL; //右子堆
    T e = this->_root->data; delete this->_root; (this->_size)--; //删除根节点
    this->_root = merge ( lHeap, rHeap ); //合并原左、右子堆
    return e; //返回原根节点的数据项
}