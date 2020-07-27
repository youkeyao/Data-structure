#ifndef AVL_h
#define AVL_h
    #include "C:\Users\youykeyao\Desktop\code\VC\c++\BST\BST.cpp" //基于BST实现AVL树

    #define Balanced(x) ( stature( (x).lc ) == stature( (x).rc ) ) //理想平衡条件
    #define BalFac(x) ( stature( (x).lc ) - stature( (x).rc ) ) //平衡因子
    #define AvlBalanced(x) ( ( -2 < BalFac(x) ) && ( BalFac(x) < 2 ) ) //AVL平衡条件

    template <typename T> class AVL : public BST<T> { //由BST派生AVL树模板类
    public:
        BinNode<T>* insert ( const T& e ); //插入（重写）
        bool remove ( const T& e ); //删除（重写）
    // BST::search()等其余接口可直接沿用
    };
#endif