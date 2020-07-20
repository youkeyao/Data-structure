#ifndef List_h
#define List_h
    #include "ListNode.h" //引入列表节点类
    
    template <typename T> class List { //列表模板类
    
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
            T& operator[] ( int r ) const; //重载，支持循秩访问（效率低）
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
            ListNode<T>* insertAsFirst ( T const& e ){  _size++; return header->insertAsSucc ( e );  } //将e当作首节点插入
            ListNode<T>* insertAsLast ( T const& e ){  _size++; return trailer->insertAsPred ( e );  } //将e当作末节点插入
            ListNode<T>* insertA ( ListNode<T>* p, T const& e ){  _size++; return p->insertAsSucc ( e );  } //将e当作p的后继插入（After）
            ListNode<T>* insertB ( ListNode<T>* p, T const& e ){  _size++; return p->insertAsPred ( e );  } //将e当作p的前驱插入（Before）
            T remove ( ListNode<T>* p ); //删除合法位置p处的节点,返回被删除节点
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
#endif