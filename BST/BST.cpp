#include "BST.h"

template <typename T>
BinNode<T>* BST<T>::connect34 (
    BinNode<T>* a, BinNode<T>* b, BinNode<T>* c,
    BinNode<T>* T0, BinNode<T>* T1, BinNode<T>* T2, BinNode<T>* T3
) {
    a->lc = T0; if ( T0 ) T0->parent = a;
    a->rc = T1; if ( T1 ) T1->parent = a; updateHeight ( a );
    c->lc = T2; if ( T2 ) T2->parent = c;
    c->rc = T3; if ( T3 ) T3->parent = c; updateHeight ( c );
    b->lc = a; a->parent = b;
    b->rc = c; c->parent = b; updateHeight ( b );
    return b; //该子树新的根节点
}

template <typename T>
BinNode<T>* BST<T>::rotateAt ( BinNode<T>* v ) { //v为非空孙辈节点
    BinNode<T>* p = v->parent; BinNode<T>* g = p->parent; //视v、p和g相对位置分四种情况
    if ( IsLChild ( *p ) ) /* zig */
        if ( IsLChild ( *v ) ) { /* zig-zig */
            p->parent = g->parent; //向上联接
            return connect34 ( v, p, g, v->lc, v->rc, p->rc, g->rc );
        } else { /* zig-zag */
            v->parent = g->parent; //向上联接
            return connect34 ( p, v, g, p->lc, v->lc, v->rc, g->rc );
        }
    else  /* zag */
        if ( IsRChild ( *v ) ) { /* zag-zag */
            p->parent = g->parent; //向上联接
            return connect34 ( g, p, v, g->lc, p->lc, v->lc, v->rc );
        } else { /* zag-zig */
            v->parent = g->parent; //向上联接
            return connect34 ( g, v, p, g->lc, v->lc, v->rc, p->rc );
        }
}

#define EQUAL(e, v)  (!(v) || (e) == (v)->data) //节点v（或假想的通配哨兵）的关键码等于e
template <typename T> //在以v为根的（AVL、SPLAY、rbTree等）BST子树中查找关键码e
static BinNode<T>* & searchIn ( BinNode<T>* & v, const T& e, BinNode<T>* & hot ) {
    if ( EQUAL ( e, v ) ) return v; hot = v; //退化情况：在子树根节点v处命中
    while ( 1 ) { //一般地，反复不断地
        BinNode<T>* & c = ( e < hot->data ) ? hot->lc : hot->rc; //确定深入方向
        if ( EQUAL ( e, c ) ) return c; hot = c; //命中返回，或者深入一层
    } //hot始终指向最后一个失败节点
} //返回时，返回值指向命中节点（或假想的通配哨兵），hot指向其父亲（退化时为初始值NULL）

template <typename T>
BinNode<T>* & BST<T>::search ( const T& e ) //在BST中查找关键码e
{ return searchIn ( this->_root, e, _hot = NULL ); } //返回目标节点位置的引用，以便后续插入、删除操作

template <typename T>
BinNode<T>* BST<T>::insert ( const T& e ) { //将关键码e插入BST树中
    BinNode<T>* & x = search ( e ); if ( x ) return x; //确认目标不存在（留意对_hot的设置）
    x = new BinNode<T> ( e, _hot ); //创建新节点x：以e为关键码，以_hot为父
    (this->_size)++; //更新全树规模
    this->updateHeightAbove ( x ); //更新x及其历代祖先的高度
    return x; //新插入的节点，必为叶子
} //无论e是否存在于原树中，返回时总有x->data == e

template <typename T>
bool BST<T>::remove ( const T& e ) { //从BST树中删除关键码e
    BinNode<T>* & x = search ( e );
    if ( !x ) return false; //确认目标存在（留意_hot的设置）
    removeAt ( x, _hot ); (this->_size)--; //实施删除
    this->updateHeightAbove ( _hot ); //更新_hot及其历代祖先的高度
    return true;
} //删除成功与否，由返回值指示

template <typename T>
static BinNode<T>* removeAt ( BinNode<T>* & x, BinNode<T>* & hot ) {
    BinNode<T>* w = x; //实际被摘除的节点，初值同x
    BinNode<T>* succ = NULL; //实际被删除节点的接替者
    if ( !(x->lc) ) //若*x的左子树为空，则可
        succ = x = x->rc; //直接将*x替换为其右子树
    else if ( !(x->rc) ) //若右子树为空，则可
        succ = x = x->lc; //对称地处理——注意：此时succ != NULL
    else { //若左右子树均存在，则选择x的直接后继作为实际被摘除节点，为此需要
        w = w->succ(); //（在右子树中）找到*x的直接后继*w
        swap ( x->data, w->data ); //交换*x和*w的数据元素
        BinNode<T>* u = w->parent;
        ( ( u == x ) ? u->rc : u->lc ) = succ = w->rc; //隔离节点*w
    }
    hot = w->parent; //记录实际被删除节点的父亲
    if ( succ ) succ->parent = hot; //并将被删除节点的接替者与hot相联
    delete w; return succ; //释放被摘除节点，返回接替者
}