#ifndef List_hpp
#define List_hpp

#include "ListNode.h" //引入列表节点类

#ifndef NULL
#define NULL 0
#endif

template <typename T>
class List { //列表模板类
    private:
        int _size; ListNode<T>* header; ListNode<T>* trailer; //规模、头哨兵、尾哨兵
        
    protected:
        void init(); //列表创建时的初始化
        int clear(); //清除所有节点
        void copyNodes ( ListNode<T>*, int ); //复制列表中自位置p起的n项
        void merge ( ListNode<T>*&, int, List<T>&, ListNode<T>*, int ); //归并
        void mergeSort ( ListNode<T>*&, int ); //对从p开始连续的n个节点归并排序
        void selectionSort ( ListNode<T>*, int ); //对从p开始连续的n个节点选择排序
        void insertionSort ( ListNode<T>*, int ); //对从p开始连续的n个节点插入排序
        ListNode<T>* rank( int r ) {ListNode<T>* p = first(); while ( r-- ) p = p->succ; return p;}
        
    public:
        // 构造函数
        List() { init(); } //默认
        List ( List<T> const& L ){ copyNodes ( L.first(), L._size ); } //整体复制列表L
        List ( List<T> const& L, int r, int n ); //复制列表L中自第r项起的n项
        List ( ListNode<T>* p, int n ){ copyNodes ( p, n ); } //复制列表中自位置p起的n项
        // 析构函数
        ~List(){ clear(); delete header; delete trailer; } //释放（包含头、尾哨兵在内的）所有节点
        // 只读访问接口
        int size() const { return _size; } //规模
        bool empty() const { return _size <= 0; } //判空
        ListNode<T>* first() const { return header->succ; } //首节点位置
        ListNode<T>* last() const { return trailer->pred; } //末节点位置
        bool valid ( ListNode<T>* p ){ return p && ( trailer != p ) && ( header != p ); } //判断位置p是否对外合法
        ListNode<T>* find ( T const& e ) const{ return find ( e, _size, trailer ); } //无序列表查找
        ListNode<T>* find ( T const& e, int n, ListNode<T>* p ) const; //无序区间查找
        ListNode<T>* search ( T const& e ) const{ return search ( e, _size, trailer ); } //有序列表查找
        ListNode<T>* search ( T const& e, int n, ListNode<T>* p ) const; //有序区间查找
        ListNode<T>* selectMax ( ListNode<T>* p, int n ); //在p及其n-1个后继中选出最大者
        ListNode<T>* selectMax() { return selectMax ( header->succ, _size ); } //整体最大者
        // 可写访问接口
        T& operator[] ( int r ) ; //重载，支持循秩访问（效率低）
        ListNode<T>* insertAsFirst ( T const& e ){  _size++; return header->insertAsSucc ( e );  } //将e当作首节点插入
        ListNode<T>* insertAsLast ( T const& e ){  _size++; return trailer->insertAsPred ( e );  } //将e当作末节点插入
        ListNode<T>* insertA ( ListNode<T>* p, T const& e ){  _size++; return p->insertAsSucc ( e );  } //将e当作p的后继插入（After）
        ListNode<T>* insertB ( ListNode<T>* p, T const& e ){  _size++; return p->insertAsPred ( e );  } //将e当作p的前驱插入（Before）
        int insert( int r, T const& e ) { insertB( rank(r), e ); return r; }
        T remove ( ListNode<T>* p ); //删除合法位置p处的节点,返回被删除节点
        T remove ( int r ) { remove( rank(r) ); }
        void merge ( List<T>& L ) { merge ( first(), size, L, L.first(), L._size ); } //全列表归并
        void sort ( ListNode<T>* p, int n ); //列表区间排序
        void sort() { sort ( first(), _size ); } //列表整体排序
        int deduplicate(); //无序去重
        int uniquify(); //有序去重
        void reverse(); //前后倒置
        // 遍历
        void traverse ( void (* ) ( T& ) ); //遍历，依次实施visit操作（函数指针，只读或局部性修改）
        template <typename VST> void traverse ( VST& ); //遍历，依次实施visit操作（函数对象，可全局性修改）
};

template <typename T>
void List<T>::init() { //列表初始化，在创建列表对象时统一调用
    header = new ListNode<T>; //创建头哨兵节点
    trailer = new ListNode<T>; //创建尾哨兵节点
    header->succ = trailer; header->pred = NULL;
    trailer->pred = header; trailer->succ = NULL;
    _size = 0; //记录规模
}

template <typename T>
int List<T>::clear() { //清空列表
    int oldSize = _size;
    while ( 0 < _size ) remove ( header->succ ); //反复删除首节点，直至列表变空
    return oldSize;
}

template <typename T> //列表内部方法：复制列表中自位置p起的n项
void List<T>::copyNodes ( ListNode<T>* p, int n ) { //p合法，且至少有n-1个真后继节点
    init(); //创建头、尾哨兵节点并做初始化
    while ( n-- ){
        insertAsLast ( p->data );
        p = p->succ; //将起自p的n项依次作为末节点插入
    }
}

template <typename T> //有序列表的归并：当前列表中自p起的n个元素，与列表L中自q起的m个元素归并
void List<T>::merge ( ListNode<T>* & p, int n, List<T>& L, ListNode<T>* q, int m ) {
// 注意：在归并排序之类的场合，有可能 this == L && int(p) + n = int(q)
    ListNode<T>* pp = p->pred; //借助前驱（可能是header），以便返回前 ...
    while ( 0 < m ) //在q尚未移出区间之前
        if ( ( 0 < n ) && ( p->data <= q->data ) ){ //若p仍在区间内且v(p) <= v(q)，则
            if ( q == ( p = p->succ ) ) break; //p归入合并的列表，并替换为其直接后继
            n--;
        }
        else{ //若p已超出右界或v(q) < v(p)，则
            insertB ( p, L.remove ( ( q = q->succ )->pred ) ); //将q转移至p之前
            m--;
        }
    p = pp->succ; //确定归并后区间的（新）起点
}

template <typename T> //列表的归并排序算法：对起始于位置p的n个元素排序
void List<T>::mergeSort ( ListNode<T>* & p, int n ) { //valid(p) && int(p) + n <= size
    if ( n < 2 ) return; //若待排序范围已足够小，则直接返回；否则...
    int m = n >> 1; //以中点为界
    ListNode<T>* q = p;
    for ( int i = 0; i < m; i++ ) q = q->succ; //均分列表
    mergeSort ( p, m ); mergeSort ( q, n - m ); //对前、后子列表分别排序
    merge ( p, m, *this, q, n - m ); //归并
} //注意：排序后，p依然指向归并后区间的（新）起点

template <typename T> //列表的选择排序算法：对起始于位置p的n个元素排序
void List<T>::selectionSort ( ListNode<T>* p, int n ) { //valid(p) && int(p) + n <= size
    ListNode<T>* head = p->pred;
    ListNode<T>* tail = p;
    for ( int i = 0; i < n; i++ ) tail = tail->succ; //待排序区间为(head, tail)
    while ( 1 < n ) { //在至少还剩两个节点之前，在待排序区间内
        ListNode<T>* max = selectMax ( head->succ, n ); //找出最大者（歧义时后者优先）
        insertB ( tail, remove ( max ) ); //将其移至无序区间末尾（作为有序区间新的首元素）
        tail = tail->pred; n--;
    }
}

template <typename T> //列表的插入排序算法：对起始于位置p的n个元素排序
void List<T>::insertionSort ( ListNode<T>* p, int n ) { //valid(p) && int(p) + n <= size
    for ( int r = 0; r < n; r++ ) { //逐一为各节点
        insertA ( search ( p->data, r, p ), p->data ); //查找适当的位置并插入
        p = p->succ;
        remove ( p->pred ); //转向下一节点
    }
}

template <typename T> //复制L中自第r项起的n项（assert: r+n <= L._size）
List<T>::List ( List<T> const& L, int r, int n ) {
    ListNode<T>* p = L.first();
    while ( 0 < r-- ) p = p->succ;
    copyNodes ( p, n );
}

template <typename T> //重载下标操作符，以通过秩直接访问列表节点（虽方便，效率低，需慎用）
T& List<T>::operator[] ( int r ) { //assert: 0 <= r < size
    return rank(r)->data; //目标节点，返回其中所存元素
}

template <typename T> //在无序列表内节点p（可能是trailer）的n个（真）前驱中，找到等于e的最后者
ListNode<T>* List<T>::find ( T const& e, int n, ListNode<T>* p ) const {
    while ( 0 < n-- ) //（0 <= n <= int(p) < _size）对于p的最近的n个前驱，从右向左
        if ( e == ( p = p->pred )->data ) return p; //逐个比对，直至命中或范围越界
    return NULL; //p越出左边界意味着区间内不含e，查找失败
} //失败时，返回NULL

template <typename T> //在有序列表内节点p（可能是trailer）的n个（真）前驱中，找到不大于e的最后者
ListNode<T>* List<T>::search ( T const& e, int n, ListNode<T>* p ) const {
    // assert: 0 <= n <= int(p) < _size
    do {
       p = p->pred; n--;  //从右向左
    } while ( ( -1 < n ) && ( e < p->data ) ); //逐个比较，直至命中或越界
    return p; //返回查找终止的位置
} //失败时，返回区间左边界的前驱（可能是header）——调用者可通过valid()判断成功与否

template <typename T> //从起始于位置p的n个元素中选出最大者
ListNode<T>* List<T>::selectMax ( ListNode<T>* p, int n ) {
    ListNode<T>* max = p; //最大者暂定为首节点p
    for ( ListNode<T>* cur = p; 1 < n; n-- ) //从首节点p出发，将后续节点逐一与max比较
        if ( ( cur = cur->succ )->data >= max->data ) //若当前元素不小于max，则
            max = cur; //更新最大元素位置记录
    return max; //返回最大节点位置
}

template <typename T>
T List<T>::remove ( ListNode<T>* p ) { //删除合法节点p，返回其数值
    T e = p->data; //备份待删除节点的数值（假定T类型可直接赋值）
    p->pred->succ = p->succ; //后继
    p->succ->pred = p->pred; //前驱
    delete p; //释放节点
    _size--; //更新规模
    return e; //返回备份的数值
}

template <typename T>
void List<T>::sort ( ListNode<T>* p, int n ) { //列表区间排序
    mergeSort ( p, n );
}

template <typename T>
int List<T>::deduplicate() {
    int oldSize = _size; ListNode<T>* p = first(); ListNode<T>* q = NULL;
    for ( int r = 0; p != trailer; p = p->succ, q = find ( p->data, r, p ) )
        q ? remove ( q ): r++; //r为无重前缀的长度
    return oldSize - _size; //即被删除元素总数
}

template <typename T>
int List<T>::uniquify() { //成批剔除重复元素，效率更高
    if ( _size < 2 ) return 0; //平凡列表自然无重复
    int oldSize = _size; //记录原规模
    ListNode<T>* p = first(); //p为各区段起点
    ListNode<T>* q; //q为其后继
    while ( trailer != ( q = p->succ ) ) //反复考查紧邻的节点对(p, q)
        if ( p->data != q->data ) p = q; //若互异，则转向下一区段
        else remove ( q ); //否则（雷同），删除后者
    return oldSize - _size; //列表规模变化量，即被删除元素总数
}

template <typename T>
void List<T>::reverse() { //前后倒置
    if ( _size < 2 ) return; //平凡情况
    for ( ListNode<T>* p = header; p; p = p->pred ) //自前向后，依次
        swap ( p->pred, p->succ ); //交换各节点的前驱、后继指针
    swap ( header, trailer ); //头、尾节点互换
}

template <typename T>
void List<T>::traverse ( void ( *visit ) ( T& ) ){ //借助函数指针机制遍历
    for ( ListNode<T>* p = header->succ; p != trailer; p = p->succ )
        visit ( p->data );
}

template <typename T> template <typename VST> //元素类型、操作器
void List<T>::traverse ( VST& visit ){ //借助函数对象机制遍历
    for ( ListNode<T>* p = header->succ; p != trailer; p = p->succ )
        visit ( p->data );
}

#endif