#ifndef BinNode_h
#define BinNode_h
    typedef enum { RB_RED, RB_BLACK} RBColor; //节点颜色

    #define stature(p) ((p) ? (p)->height : -1) //节点高度（与“空树高度为-1”的约定相统一）
    #define IsRoot(x) ( ! ( (x).parent ) )
    #define IsLChild(x) ( ! IsRoot(x) && ( & (x) == (x).parent->lc ) )
    #define IsRChild(x) ( ! IsRoot(x) && ( & (x) == (x).parent->rc ) )
    #define sibling(p) ( IsLChild( * (p) ) ? (p)->parent->rc : (p)->parent->lc ) //兄弟
    #define uncle(x) ( IsLChild( * ( (x)->parent ) ) ? (x)->parent->parent->rc : (x)->parent->parent->lc ) //叔叔
    #define FromParentTo(x) ( IsRoot(x) ? _root : ( IsLChild(x) ? (x).parent->lc : (x).parent->rc ) ) //来自父亲的引用

    #include <iostream>
    using namespace std;
    template <typename T>
    struct BinNode { //二叉树节点模板类
        T data; //数值
        BinNode<T>* parent; BinNode<T>* lc; BinNode<T>* rc; //父节点及左、右孩子
        int height; //高度（通用）
        int npl; //Null Path Length（左式堆，也可直接用height代替）
        RBColor color; //颜色（红黑树）
        // 构造函数
        BinNode(): parent ( NULL ), lc ( NULL ), rc ( NULL ), height ( 0 ), npl ( 1 ), color ( RB_RED ) { }
        BinNode ( T e, BinNode<T>* p = NULL, BinNode<T>* lc = NULL, BinNode<T>* rc = NULL, int h = 0, int l = 1, RBColor c = RB_RED ):
        data ( e ), parent ( p ), lc ( lc ), rc ( rc ), height ( h ), npl ( l ), color ( c ) { }
        // 操作接口
        int size(); //统计当前节点后代总数，亦即以其为根的子树的规模
        BinNode<T>* insertAsLC ( T const& e ) { return lc = new BinNode ( e, this ); } //作为当前节点的左孩子插入新节点
        BinNode<T>* insertAsRC ( T const& e ) { return rc = new BinNode ( e, this ); } //作为当前节点的右孩子插入新节点
        BinNode<T>* succ(); //取当前节点的直接后继
        template <typename VST> void travLevel ( VST& ); //子树层次遍历
        template <typename VST> void travPre ( VST& ); //子树先序遍历
        template <typename VST> void travIn ( VST& ); //子树中序遍历
        template <typename VST> void travPost ( VST& ); //子树后序遍历
        // 比较器、判等器（各列其一，其余自行补充）
        bool operator< ( BinNode const& bn ) { return data < bn.data; } //小于
        bool operator== ( BinNode const& bn ) { return data == bn.data; } //等于
    };
#endif