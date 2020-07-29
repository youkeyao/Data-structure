#include "Splay.h"

template <typename NodePosi> inline //在节点*p与*lc（可能为空）之间建立父（左）子关系
void attachAsLChild ( NodePosi p, NodePosi lc ) { p->lc = lc; if ( lc ) lc->parent = p; }

template <typename NodePosi> inline //在节点*p与*rc（可能为空）之间建立父（右）子关系
void attachAsRChild ( NodePosi p, NodePosi rc ) { p->rc = rc; if ( rc ) rc->parent = p; }

template <typename T> //Splay树伸展算法：从节点v出发逐层伸展
BinNode<T>* Splay<T>::splay ( BinNode<T>* v ) { //v为因最近访问而需伸展的节点位置
    if ( !v ) return NULL; BinNode<T>* p; BinNode<T>* g; //*v的父亲与祖父
    while ( ( p = v->parent ) && ( g = p->parent ) ) { //自下而上，反复对*v做双层伸展
        BinNode<T>* gg = g->parent; //每轮之后*v都以原曾祖父（great-grand parent）为父
        if ( IsLChild ( *v ) )
            if ( IsLChild ( *p ) ) { //zig-zig
                attachAsLChild ( g, p->rc ); attachAsLChild ( p, v->rc );
                attachAsRChild ( p, g ); attachAsRChild ( v, p );
            } else { //zig-zag
                attachAsLChild ( p, v->rc ); attachAsRChild ( g, v->lc );
                attachAsLChild ( v, g ); attachAsRChild ( v, p );
            }
        else if ( IsRChild ( *p ) ) { //zag-zag
            attachAsRChild ( g, p->lc ); attachAsRChild ( p, v->lc );
            attachAsLChild ( p, g ); attachAsLChild ( v, p );
        } else { //zag-zig
            attachAsRChild ( p, v->lc ); attachAsLChild ( g, v->rc );
            attachAsRChild ( v, g ); attachAsLChild ( v, p );
        }
        if ( !gg ) v->parent = NULL; //若*v原先的曾祖父*gg不存在，则*v现在应为树根
        else //否则，*gg此后应该以*v作为左或右孩子
            ( g == gg->lc ) ? attachAsLChild ( gg, v ) : attachAsRChild ( gg, v );
        this->updateHeight ( g ); this->updateHeight ( p ); this->updateHeight ( v );
    } //双层伸展结束时，必有g == NULL，但p可能非空
    if ( p = v->parent ) { //若p果真非空，则额外再做一次单旋
        if ( IsLChild ( *v ) ) { attachAsLChild ( p, v->rc ); attachAsRChild ( v, p ); }
        else                   { attachAsRChild ( p, v->lc ); attachAsLChild ( v, p ); }
        this->updateHeight ( p ); this->updateHeight ( v );
    }
    v->parent = NULL; return v;
} //调整之后新树根应为被伸展的节点，故返回该节点的位置以便上层函数更新树根

template <typename T>
BinNode<T>* & Splay<T>::search ( const T& e ) { //在伸展树中查找e
    BinNode<T>* p = searchIn ( this->_root, e, this->_hot = NULL );
    this->_root = splay ( p ? p : this->_hot ); //将最后一个被访问的节点伸展至根
    return this->_root;
} //与其它BST不同，无论查找成功与否，_root都指向最后被访问的节点

template <typename T>
BinNode<T>* Splay<T>::insert ( const T& e ) { //将关键码e插入伸展树中
    if ( !this->_root ) { (this->_size)++; return this->_root = new BinNode<T> ( e ); } //处理原树为空的退化情况
    if ( e == search ( e )->data ) return this->_root; //确认目标节点不存在
    (this->_size)++; BinNode<T>* t = this->_root; //创建新节点。以下调整<=7个指针以完成局部重构
    if ( this->_root->data < e ) { //插入新根，以t和t->rc为左、右孩子
        t->parent = this->_root = new BinNode<T> ( e, NULL, t, t->rc ); //2 + 3个
        if ( t->rc ) { t->rc->parent = this->_root; t->rc = NULL; } //<= 2个
    } else { //插入新根，以t->lc和t为左、右孩子
        t->parent = this->_root = new BinNode<T> ( e, NULL, t->lc, t ); //2 + 3个
        if ( t->lc ) { t->lc->parent = this->_root; t->lc = NULL; } //<= 2个
    }
    this->updateHeightAbove ( t ); //更新t及其祖先（实际上只有_root一个）的高度
    return this->_root; //新节点必然置于树根，返回之
} //无论e是否存在于原树中，返回时总有_root->data == e

template <typename T> bool Splay<T>::remove ( const T& e ) { //从伸展树中删除关键码e
    if ( !this->_root || ( e != search ( e )->data ) ) return false; //若树空或目标不存在，则无法删除
    BinNode<T>* w = this->_root; //assert: 经search()后节点e已被伸展至树根
    if ( ! ( this->_root->lc ) ) { //若无左子树，则直接删除
        this->_root = this->_root->rc; if ( this->_root ) this->_root->parent = NULL;
    } else if ( ! ( this->_root->rc) ) { //若无右子树，也直接删除
        this->_root = this->_root->lc; if ( this->_root ) this->_root->parent = NULL;
    } else { //若左右子树同时存在，则
        BinNode<T>* lTree = this->_root->lc;
        lTree->parent = NULL; this->_root->lc = NULL; //暂时将左子树切除
        this->_root = this->_root->rc; this->_root->parent = NULL; //只保留右子树
        search ( w->data ); //以原树根为目标，做一次（必定失败的）查找
///// assert: 至此，右子树中最小节点必伸展至根，且（因无雷同节点）其左子树必空，于是
        this->_root->lc = lTree; lTree->parent = this->_root; //只需将原左子树接回原位即可
    }
    delete w; (this->_size)--; //释放节点，更新规模
    if ( this->_root ) this->updateHeight ( this->_root ); //此后，若树非空，则树根的高度需要更新
    return true; //返回成功标志
} //若目标节点存在且被删除，返回true；否则返回false