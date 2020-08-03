#ifndef PQ_LeftHeap_h
#define PQ_LeftHeap_h

    #include "PQ\PQ.h" //引入优先级队列ADT
    #include "BinTree\BinTree.cpp" //引入二叉树节点模板类

    template <typename T>
    class PQ_LeftHeap : public PQ<T>, public BinTree<T> { //基于二叉树，以左式堆形式实现的PQ
    public:
        PQ_LeftHeap() { } //默认构造
        PQ_LeftHeap ( T* E, int n ) //批量构造：可改进为Floyd建堆算法
        {  for ( int i = 0; i < n; i++ ) insert ( E[i] );  }
        void insert ( T ); //按照比较器确定的优先级次序插入元素
        T getMax(); //取出优先级最高的元素
        T delMax(); //删除优先级最高的元素
    };

    template <typename T> //根据相对优先级确定适宜的方式，合并以a和b为根节点的两个左式堆
    static BinNode<T>* merge ( BinNode<T>* a, BinNode<T>* b ) {
        if ( ! a ) return b; //退化情况
        if ( ! b ) return a; //退化情况
        if ( a->data < b->data ) swap ( a, b ); //一般情况：首先确保b不大
        ( a->rc = merge ( a->rc, b ) )->parent = a; //将a的右子堆，与b合并
        if ( !a->lc || a->lc->npl < a->rc->npl ) //若有必要
           swap ( a->lc, a->rc ); //交换a的左、右子堆，以确保右子堆的npl不大
        a->npl = a->rc ? a->rc->npl + 1 : 1; //更新a的npl
        return a; //返回合并后的堆顶
    } //本算法只实现结构上的合并，堆的规模须由上层调用者负责更新

#endif
