#ifndef BinTree_h
#define BinTree_h
    #include "BinNode.cpp" //引入二叉树节点类
    template <typename T>
    class BinTree { //二叉树模板类
    protected:
        int _size; BinNode<T>* _root; //规模、根节点
        virtual int updateHeight ( BinNode<T>* x ) { return x->height = 1 + max ( stature ( x->lc ), stature ( x->rc ) ); } //更新节点x的高度
        void updateHeightAbove ( BinNode<T>* x ) { while ( x ) { updateHeight ( x ); x = x->parent; } } //更新节点x及其祖先的高度
    public:
        BinTree() : _size ( 0 ), _root ( NULL ) { } //构造函数
        ~BinTree() { if ( 0 < _size ) remove ( _root ); } //析构函数
        int size() const { return _size; } //规模
        bool empty() const { return !_root; } //判空
        BinNode<T>* root() const { return _root; } //树根
        BinNode<T>* insertAsRoot ( T const& e ) { _size = 1; return _root = new BinNode<T> ( e ); } //插入根节点
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
        bool operator< ( BinTree<T> const& t ) { return _root && t._root && lt ( _root, t._root ); }//比较器
        bool operator== ( BinTree<T> const& t ) { return _root && t._root && ( _root == t._root ); }//判等器
    };
#endif