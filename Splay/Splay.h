#ifndef Splay_h
#define Splay_h
    #include "BST\BST.cpp" //基于BST实现Splay
    template <typename T> class Splay : public BST<T> { //由BST派生的Splay树模板类
    protected:
        BinNode<T>* splay ( BinNode<T>* v ); //将节点v伸展至根
    public:
        BinNode<T>* & search ( const T& e ); //查找（重写）
        BinNode<T>* insert ( const T& e ); //插入（重写）
        bool remove ( const T& e ); //删除（重写）
    };
#endif
