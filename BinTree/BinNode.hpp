#ifndef BinNode_hpp
#define BinNode_hpp

#include "C:\Users\youykeyao\Desktop\code\VC\c++\Stack\Stack_Vector\Stack_Vector.h"
#include "C:\Users\youykeyao\Desktop\code\VC\c++\Queue\Queue.h"

typedef enum { RB_RED, RB_BLACK} RBColor; //节点颜色

#define stature(p) ((p) ? (p)->height : -1) //节点高度（与“空树高度为-1”的约定相统一）
#define IsRoot(x) ( ! ( (x).parent ) )
#define IsLChild(x) ( ! IsRoot(x) && ( & (x) == (x).parent->lc ) )
#define IsRChild(x) ( ! IsRoot(x) && ( & (x) == (x).parent->rc ) )
#define sibling(p) ( IsLChild( * (p) ) ? (p)->parent->rc : (p)->parent->lc ) //兄弟
#define uncle(x) ( IsLChild( * ( (x)->parent ) ) ? (x)->parent->parent->rc : (x)->parent->parent->lc ) //叔叔
#define FromParentTo(x) ( IsRoot(x) ? this->_root : ( IsLChild(x) ? (x).parent->lc : (x).parent->rc ) ) //来自父亲的引用

template <typename T>
struct BinNode { //二叉树节点模板类
    T data; //数值
    BinNode<T>* parent; BinNode<T>* lc; BinNode<T>* rc; //父节点及左、右孩子
    int height; //高度（通用）
    int npl; //NULL Path Length（左式堆，也可直接用height代替）
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

template <typename T>
int BinNode<T>::size() { //统计当前节点后代总数，即以其为根的子树规模
    int s = 1; //计入本身
    if ( lc ) s += lc->size(); //递归计入左子树规模
    if ( rc ) s += rc->size(); //递归计入右子树规模
    return s;
}

template <typename T>
BinNode<T>* BinNode<T>::succ() { //定位节点v的直接后继
    BinNode<T>* s = this; //记录后继的临时变量
    if ( rc ) { //若有右孩子，则直接后继必在右子树中，具体地就是
       s = rc; //右子树中
       while ( s->lc ) s = s->lc; //最靠左（最小）的节点
    } else { //否则，直接后继应是“将当前节点包含于其左子树中的最低祖先”，具体地就是
       while ( IsRChild( *s ) ) s = s->parent; //逆向地沿右向分支，不断朝左上方移动
       s = s->parent; //最后再朝右上方移动一步，即抵达直接后继（如果存在）
    }
    return s;
}

template <typename T> template <typename VST> //元素类型、操作器
void BinNode<T>::travLevel ( VST& visit ) { //二叉树层次遍历算法
   Queue<BinNode<T>*> Q; //辅助队列
   Q.enqueue ( this ); //根节点入队
   while ( !Q.empty() ) { //在队列再次变空之前，反复迭代
      BinNode<T>* x = Q.dequeue(); visit ( x->data ); //取出队首节点并访问之
      if ( x->lc ) Q.enqueue ( x->lc ); //左孩子入队
      if ( x->rc ) Q.enqueue ( x->rc ); //右孩子入队
   }
}

template <typename T> template <typename VST> //元素类型、操作器
void BinNode<T>::travPre ( VST& visit ) { //二叉树先序遍历算法
   BinNode<T>* x = this;
   Stack<BinNode<T>*> S; //辅助栈
   if ( x ) S.push ( x ); //根节点入栈
   while ( !S.empty() ) { //在栈变空之前反复循环
      x = S.pop(); visit ( x->data ); //弹出并访问当前节点
      if ( x->rc ) S.push ( x->rc );//其非空孩子的入栈次序为先右后左
      if ( x->lc ) S.push ( x->lc );
   }
}

template <typename T> template <typename VST> //元素类型、操作器
void BinNode<T>::travIn ( VST& visit ) { //二叉树中序遍历算法
   BinNode<T>* x = this;
   Stack<BinNode<T>*> S; //辅助栈
   while ( true )
      if ( x ) {
         S.push ( x ); //根节点进栈
         x = x->lc; //深入遍历左子树
      }
      else if ( !S.empty() ) {
         x = S.pop(); //尚未访问的最低祖先节点退栈
         visit ( x->data ); //访问该祖先节点
         x = x->rc; //遍历祖先的右子树
      }
      else break; //遍历完成
}

template <typename T> template <typename VST>
void BinNode<T>::travPost ( VST& visit ) { //二叉树的后序遍历
   BinNode<T>* x = this;
   Stack<BinNode<T>*> S; //辅助栈
   if ( x ) S.push ( x ); //根节点入栈
   while ( !S.empty() ) {
      if ( S.top() != x->parent ) { //若栈顶非当前节点之父（则必为其右兄）
         while ( BinNode<T>* x = S.top() ) //自顶而下，反复检查当前节点（即栈顶）
            if ( x->lc ) { //尽可能向左
               if ( x->rc ) S.push ( x->rc ); //若有右孩子，优先入栈
               S.push ( x->lc ); //然后才转至左孩子
            }
            else S.push ( x->rc ); //才向右
         S.pop(); //返回之前，弹出栈顶的空节点
      }
      x = S.pop(); //弹出栈顶（即前一节点之后继）
      visit ( x->data ); //并访问之
   }
}

#endif