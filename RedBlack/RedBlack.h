#ifndef RedBlack_h
#define RedBlack_h
    #include "BST\BST.cpp" //基于BST实现RedBlack

    #define IsBlack(p) ( ! (p) || ( RB_BLACK == (p)->color ) ) //外部节点也视作黑节点
    #define IsRed(p) ( ! IsBlack(p) ) //非黑即红
    #define BlackHeightUpdated(x) ( /*RedBlack高度更新条件*/ \
        ( stature( (x).lc ) == stature( (x).rc ) ) && \
        ( (x).height == ( IsRed(& x) ? stature( (x).lc ) : stature( (x).lc ) + 1 ) ) \
    )

    template <typename T> class RedBlack : public BST<T> { //RedBlack树模板类
    protected:
        void solveDoubleRed ( BinNode<T>* x ); //双红修正
        void solveDoubleBlack ( BinNode<T>* x ); //双黑修正
        int updateHeight ( BinNode<T>* x ); //更新节点x的高度
    public:
        BinNode<T>* insert ( const T& e ); //插入（重写）
        bool remove ( const T& e ); //删除（重写）
    // BST::search()等其余接口可直接沿用
    };
#endif
