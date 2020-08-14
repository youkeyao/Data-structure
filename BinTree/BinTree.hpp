#ifndef BinTree_hpp
#define BinTree_hpp

#include "BinNode.hpp" //引入二叉树节点类

template <typename T>
class BinTree { //二叉树模板类

    protected:
        int _size; BinNode<T>* _root; //规模、根节点
        virtual int updateHeight ( BinNode<T>* x )
        { return x->height = 1 + (stature ( x->lc )>stature ( x->rc ) ? stature ( x->lc ):stature ( x->rc )); } //更新节点x的高度
        void updateHeightAbove ( BinNode<T>* x ) { while ( x ) { updateHeight ( x ); x = x->parent; } } //更新节点x及其祖先的高度

    public:
        BinTree() : _size ( 0 ), _root ( NULL ) { } //构造函数
        ~BinTree() { if ( 0 < _size ) remove ( _root ); } //析构函数
        int size() const { return _size; } //规模
        bool empty() const { return !_root; } //判空
        BinNode<T>* root() const { return _root; } //树根
        BinNode<T>* insertAsRoot ( T const& e ) { removeAt(_root); _size = 1; return _root = new BinNode<T> ( e ); } //插入根节点
        BinNode<T>* insertAsLC ( BinNode<T>* x, T const& e ); //e作为x的左孩子（原无）插入
        BinNode<T>* insertAsRC ( BinNode<T>* x, T const& e ); //e作为x的右孩子（原无）插入
        BinNode<T>* attachAsLC ( BinNode<T>* x, BinTree<T>* &S ); //T作为x左子树接入
        BinNode<T>* attachAsRC ( BinNode<T>* x, BinTree<T>* &S ); //T作为x右子树接入
        int remove ( BinNode<T>* x ); //删除以位置x处节点为根的子树，返回该子树原先的规模
        BinTree<T>* secede ( BinNode<T>* x ); //将子树x从当前树中摘除，并将其转换为一棵独立子树
        template <typename VST> //操作器
        void travLevel ( VST& visit ) { if ( _root ) _root->travLevel ( visit ); } //层次遍历
        template <typename VST> //操作器
        void travPre ( VST& visit ) { if ( _root ) _root->travPre ( visit ); } //先序遍历
        template <typename VST> //操作器
        void travIn ( VST& visit ) { if ( _root ) _root->travIn ( visit ); } //中序遍历
        template <typename VST> //操作器
        void travPost ( VST& visit ) { if ( _root ) _root->travPost ( visit ); } //后序遍历
        bool operator< ( BinTree<T> const& t ) { return _root && t._root && _root->data < t._root->data; }//比较器
        bool operator== ( BinTree<T> const& t ) { return _root && t._root && ( _root == t._root ); }//判等器

    //静态方法
     //删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
    static int removeAt ( BinNode<T>* x ) { //assert: x为二叉树中的合法位置
        if ( !x ) return 0; //递归基：空树
        int n = 1 + removeAt ( x->lc ) + removeAt ( x->rc ); //递归释放左、右子树
        delete x; return n; //释放被摘除节点，并返回删除节点总数
    }
};

template <typename T>
BinNode<T>* BinTree<T>::insertAsLC ( BinNode<T>* x, T const& e ){ //e插入为x的左孩子
    _size++;
    x->insertAsLC ( e );
    updateHeightAbove ( x );
    return x->lc;
}

template <typename T>
BinNode<T>* BinTree<T>::insertAsRC ( BinNode<T>* x, T const& e ){ //e插入为x的右孩子
    _size++;
    x->insertAsRC ( e );
    updateHeightAbove ( x );
    return x->rc;
}

template <typename T> //二叉树子树接入算法：将S当作节点x的左子树接入，S本身置空
BinNode<T>* BinTree<T>::attachAsLC ( BinNode<T>* x, BinTree<T>* &S ) { //x->lc == NULL
    if ( x->lc = S->_root ) x->lc->parent = x; //接入
    _size += S->_size; //更新全树规模
    updateHeightAbove ( x ); //更新x所有祖先的高度
    S->_size = 0;
    S = NULL; //释放原树
    return x; //返回接入位置
}

template <typename T> //二叉树子树接入算法：将S当作节点x的右子树接入，S本身置空
BinNode<T>* BinTree<T>::attachAsRC ( BinNode<T>* x, BinTree<T>* &S ) { //x->rc == NULL
    if ( x->rc = S->_root ) x->rc->parent = x; //接入
    _size += S->size(); //更新全树规模
    updateHeightAbove ( x ); //更新x所有祖先的高度
    S->_size = 0;
    S = NULL; //释放原树
    return x; //返回接入位置
}

template <typename T> //删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
int BinTree<T>::remove ( BinNode<T>* x ) { //assert: x为二叉树中的合法位置
    if ( !x ) return 0;
    FromParentTo ( *x ) = NULL; //切断来自父节点的指针
    updateHeightAbove ( x->parent ); //更新祖先高度
    int n = removeAt ( x ); _size -= n; return n; //删除子树x，更新规模，返回删除节点总数
}

template <typename T> //二叉树子树分离算法：将子树x从当前树中摘除，将其封装为一棵独立子树返回
BinTree<T>* BinTree<T>::secede ( BinNode<T>* x ) { //assert: x为二叉树中的合法位置
    FromParentTo ( *x ) = NULL; //切断来自父节点的指针
    updateHeightAbove ( x->parent ); //更新原树中所有祖先的高度
    BinTree<T>* S = new BinTree<T>; S->_root = x; x->parent = NULL; //新树以x为根
    S->_size = x->size(); _size -= S->_size; return S; //更新规模，返回分离出来的子树
}

#endif