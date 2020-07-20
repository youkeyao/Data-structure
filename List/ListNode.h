#ifndef ListNode_h
#define ListNode_h
    #include <iostream>
    using namespace std;
    template <typename T>
    struct ListNode { //列表节点模板类（以双向链表形式实现）
        // 成员
        T data; ListNode<T>* pred; ListNode<T>* succ; //数值、前驱、后继
        // 构造函数
        ListNode() {} //针对header和trailer的构造
        ListNode ( T e, ListNode<T>* p = NULL, ListNode<T>* s = NULL )
        : data ( e ), pred ( p ), succ ( s ) {} //默认构造器
        // 操作接口
        ListNode<T>* insertAsPred ( T const& e ){ //紧靠当前节点之前插入新节点
            ListNode<T>* x = new ListNode ( e, pred, this ); //创建新节点
            pred->succ = x; pred = x; //设置正向链接
            return x; //返回新节点的位置
        }

        ListNode<T>* insertAsSucc ( T const& e ){ //紧随当前节点之后插入新节点
            ListNode<T>* x = new ListNode ( e, this, succ ); //创建新节点
            succ->pred = x; succ = x; //设置逆向链接
            return x; //返回新节点的位置
        }
    };
#endif