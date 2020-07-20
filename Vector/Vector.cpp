#include "Vector.h"
#include "Fib.h"
#include <iostream>
#include <cstdlib>
using namespace std;

template <typename T>
void Vector<T>::copyFrom( T const* A, int lo, int hi ){ //以数组区间A[lo, hi)为蓝本复制向量
    _elem = new T[ _capacity = 2*(hi-lo) ]; //分配空间
    _size = 0; //规模清零
    while(lo<hi){ //A[lo, hi)内的元素逐一
        _elem[_size++] = A[lo++]; //复制至_elem[0, hi - lo)
    }
}

template <typename T>
void Vector<T>::expand(){ //向量空间不足时扩容
    if(_size < _capacity)return; //尚未满员时，不必扩容
    if(_capacity < DEFAULT_CAPACITY)_capacity = DEFAULT_CAPACITY; //不低于最小容量
    T* oldElem = _elem; //复制原向量内容
    _elem = new T[_capacity <<= 1]; //容量加倍
    for(int i = 0;i < _size;i++)_elem[i] = oldElem[i]; //复制原向量内容（T为基本类型，或已重载赋值操作符'='）
    delete [] oldElem; //释放原空间
}

template <typename T>
void Vector<T>::shrink(){ //装填因子过小时压缩向量所占空间
    if(_capacity < DEFAULT_CAPACITY << 1)return; //不致收缩到DEFAULT_CAPACITY以下
    if(_size << 2 > _capacity)return; //以25%为界
    T* oldElem = _elem; //复制原向量内容
    _elem = new T[_capacity >>= 1]; //容量减半
    for(int i = 0;i < _size;i++)_elem[i] = oldElem[i]; //复制原向量内容（T为基本类型，或已重载赋值操作符'='）
    delete [] oldElem; //释放原空间
}

template <typename T> //向量的起泡排序（跳跃版）
void Vector<T>::bubbleSort( int lo, int hi ){ //assert: 0 <= lo < hi <= size
    for( int last = --hi; lo < hi; hi = last )
        for( int i = last = lo; i < hi; i++ )
            if( _elem[i] > _elem[i + 1] ) //若逆序，则
                swap( _elem[ last = i ], _elem[ i + 1 ] ); //经交换使局部有序
}

template <typename T>
int Vector<T>::max( int lo, int hi ){ //在[lo, hi]内找出最大者
    int mx = hi;
    while(lo < hi--)mx = _elem[mx]>_elem[hi] ? mx:hi; //在max有多个时保证后者优先，进而保证selectionSort稳定
    return mx;
}

template <typename T> //向量选择排序
void Vector<T>::selectionSort ( int lo, int hi ) { //0 <= lo < hi <= size
    while ( lo < --hi )
        swap ( _elem[max ( lo, hi ) ], _elem[hi] ); //将[hi]与[lo, hi]中的最大者交换
}

template <typename T> //有序向量（区间）的归并
void Vector<T>::merge ( int lo, int mi, int hi ) { //各自有序的子向量[lo, mi)和[mi, hi)
    T* A = _elem + lo; //合并后的向量A[0, hi - lo) = _elem[lo, hi)
    int lb = mi - lo;
    T* B = new T[lb]; //前子向量B[0, lb) = _elem[lo, mi)
    for ( int i = 0; i < lb; i++ ) B[i] = A[i]; //复制前子向量
    int lc = hi - mi;
    T* C = _elem + mi; //后子向量C[0, lc) = _elem[mi, hi)
    for ( int i = 0, j = 0, k = 0; j < lb; ) //归并：反复从B和C首元素中取出更小者
        A[i++] = ( lc <= k || B[j] <= C[k] ) ? B[j++] : C[k++]; //将其归入A中
    delete [] B; //释放临时空间B
}

template <typename T> //向量归并排序
void Vector<T>::mergeSort ( int lo, int hi ) { //0 <= lo < hi <= size
    if ( hi - lo < 2 ) return; //单元素区间自然有序，否则...
    int mi = ( lo + hi ) >> 1; //以中点为界
    mergeSort ( lo, mi ); mergeSort ( mi, hi ); //分别排序
    merge ( lo, mi, hi ); //归并
}

template <typename T>
void Vector<T>::heapSort ( int lo, int hi ) { //0 <= lo < hi <= size
    T* A = _elem + lo;
    int n = hi - lo;
    heapify( A, n ); //将待排序区间建成一个完全二叉堆，O(n)
    while ( 0 < --n ){ //反复地摘除最大元并归入已排序的后缀，直至堆空
        swap( A[0], A[n] );
        percolateDown( A, n, 0 ); //堆顶与末元素对换，再下滤
    }
}

template <typename T> //轴点构造算法：通过调整元素位置构造区间[lo, hi)的轴点，并返回其秩
int Vector<T>::partition ( int lo, int hi ) { //版本C
    swap ( _elem[lo], _elem[ lo + rand() % ( hi - lo ) ] ); //任选一个元素与首元素交换
    T pivot = _elem[lo]; //以首元素为候选轴点——经以上交换，等效于随机选取
    int mi = lo;
    for ( int k = lo + 1; k < hi; k++ ) //自左向右扫描
        if ( _elem[k] < pivot ) //若当前元素_elem[k]小于pivot，则
            swap ( _elem[++mi], _elem[k] ); //将_elem[k]交换至原mi之后，使L子序列向右扩展
    swap ( _elem[lo], _elem[mi] ); //候选轴点归位
    return mi; //返回轴点的秩
}

template <typename T> //向量快速排序
void Vector<T>::quickSort ( int lo, int hi ) { //0 <= lo < hi <= size
    if ( hi - lo < 2 ) return; //单元素区间自然有序，否则...
    int mi = partition ( lo, hi ); //在[lo, hi)内构造轴点
    quickSort ( lo, mi ); //对前缀递归排序
    quickSort ( mi + 1, hi ); //对后缀递归排序
}

template <typename T> //向量希尔排序
void Vector<T>::shellSort ( int lo, int hi ) { //0 <= lo < hi <= size <= 2^30
    for ( int d = 0x3FFFFFFF; 0 < d; d >>= 1 ) //PS Sequence: { 1, 3, 7, 15, ..., 1073741823 }
        for ( int j = lo + d; j < hi; j++ ) { //for each j in [lo+d, hi)
            T x = _elem[j]; int i = j - d;
            while ( lo <= i && _elem[i] > x ){
                _elem[i + d] = _elem[i];
                i -= d;
            }
            _elem[i + d] = x; //insert [j] into its subsequence
       }
}

template <typename T> //无序向量的顺序查找：返回最后一个元素e的位置；失败时，返回lo - 1
int Vector<T>::find ( T const& e, int lo, int hi ) const { //assert: 0 <= lo < hi <= _size
    while ( ( lo < hi-- ) && ( e != _elem[hi] ) ); //从后向前，顺序查找
    return hi; //若hi < lo，则意味着失败；否则hi即命中元素的秩
}

template <typename T> //在有序向量的区间[lo, hi)内，确定不大于e的最后一个节点的秩
int Vector<T>::search ( T const& e, int lo, int hi ) const { //assert: 0 <= lo < hi <= _size
    return ( rand() % 2 ) ? //按各50%的概率随机使用二分查找或Fibonacci查找
           binSearch ( _elem, e, lo, hi ) : fibSearch ( _elem, e, lo, hi );
}

template <typename T>
static int binSearch ( T* S, T const& e, int lo, int hi ) {
    while ( lo < hi ) { //每步迭代仅需做一次比较判断，有两个分支
        int mi = ( lo + hi ) >> 1; //以中点为轴点（区间宽度的折半，等效于宽度之数值表示的右移）
        ( e < S[mi] ) ? hi = mi : lo = mi + 1; //经比较后确定深入[lo, mi)或(mi, hi)
    } //成功查找不能提前终止
    return lo - 1; //循环结束时，lo为大于e的元素的最小秩，故lo - 1即不大于e的元素的最大秩
}

template <typename T> //Fibonacci查找算法：在有序向量的区间[lo, hi)内查找元素e，0 <= lo <= hi <= _size
static int fibSearch ( T* S, T const& e, int lo, int hi ) {
    for( Fib fib ( hi - lo ); lo < hi;  ) { //Fib数列制表备查
       while( hi - lo < fib.get() ) fib.prev(); //自后向前顺序查找（分摊O(1)）
       int mi = lo + fib.get() - 1; //确定形如Fib(k) - 1的轴点
       ( e < S[mi] ) ? hi = mi : lo = mi + 1; //比较后确定深入前半段[lo, mi)或后半段(mi, hi)
    } //成功查找不能提前终止
    return --lo; //循环结束时，lo为大于e的元素的最小秩，故lo - 1即不大于e的元素的最大秩
} //有多个命中元素时，总能保证返回最秩最大者；查找失败时，能够返回失败的位置

template <typename T>
Vector<T>& Vector<T>::operator= ( Vector<T> const& V ) { //深复制
    if ( _elem ) delete [] _elem; //释放原有内容
    copyFrom ( V._elem, 0, V.size() ); //整体复制
    return *this; //返回当前对象的引用，以便链式赋值
}

template <typename T>
T Vector<T>::remove ( int r ) { //删除向量中秩为r的元素，0 <= r < size
    T e = _elem[r]; //备份被删除元素
    remove ( r, r + 1 ); //调用区间删除算法，等效于对区间[r, r + 1)的删除
    return e; //返回被删除元素
}

template <typename T>
int Vector<T>::remove ( int lo, int hi ) { //删除区间[lo, hi)
    if ( lo == hi ) return 0; //出于效率考虑，单独处理退化情况，比如remove(0, 0)
    while ( hi < _size ) _elem[lo++] = _elem[hi++]; //[hi, _size)顺次前移hi - lo个单元
    _size = lo; //更新规模，直接丢弃尾部[lo, _size = hi)区间
    shrink(); //若有必要，则缩容
    return hi - lo; //返回被删除元素的数目
}

template <typename T> //将e作为秩为r元素插入
int Vector<T>::insert ( int r, T const& e ) { //assert: 0 <= r <= size
    expand(); //若有必要，扩容
    for ( int i = _size; i > r; i-- ) _elem[i] = _elem[i-1]; //自后向前，后继元素顺次后移一个单元
    _elem[r] = e;//置入新元素
    _size++; //更新容量
    return r; //返回秩
}

template <typename T>
void Vector<T>::sort ( int lo, int hi ) { //向量区间[lo, hi)排序
    switch ( rand() % 6 ) {
        case 1:  bubbleSort ( lo, hi ); break; //起泡排序
        case 2:  selectionSort ( lo, hi ); break; //选择排序
        case 3:  mergeSort ( lo, hi ); break; //归并排序
        case 4:  heapSort ( lo, hi ); break; //堆排序
        case 5:  quickSort ( lo, hi ); break; //快速排序
        default: shellSort ( lo, hi ); break; //希尔排序
    } //随机选择算法以充分测试。实用时可视具体问题的特点灵活确定或扩充
}

template <typename T>
void Vector<T>::unsort ( int lo, int hi ) { //等概率随机置乱区间[lo, hi)
    T* V = _elem + lo; //将子向量_elem[lo, hi)视作另一向量V[0, hi - lo)
    for ( int i = hi - lo; i > 0; i-- ) //自后向前
        swap ( V[i - 1], V[rand() % i] ); //将V[i - 1]与V[0, i)中某一元素随机交换
}

template <typename T>
int Vector<T>::deduplicate() { //删除无序向量中重复元素（高效版）
    int oldSize = _size; //记录原规模
    int i = 1; //从_elem[1]开始
    while ( i < _size ) //自前向后逐一考查各元素_elem[i]
       ( find ( _elem[i], 0, i ) < 0 ) ? //在其前缀中寻找与之雷同者（至多一个）
       i++ : remove ( i ); //若无雷同则继续考查其后继，否则删除雷同者
    return oldSize - _size; //向量规模变化量，即被删除元素总数
}

template <typename T>
int Vector<T>::uniquify() { //有序向量重复元素剔除算法（高效版）
    int i = 0, j = 0; //各对互异“相邻”元素的秩
    while ( ++j < _size ) //逐一扫描，直至末元素
        if ( _elem[i] != _elem[j] ) //跳过雷同者
            _elem[++i] = _elem[j]; //发现不同元素时，向前移至紧邻于前者右侧
    _size = ++i;
    shrink(); //直接截除尾部多余元素
    return j - i; //向量规模变化量，即被删除元素总数
}

template <typename T>
void Vector<T>::traverse ( void ( *visit ) ( T& ) ){ //借助函数指针机制
    for ( int i = 0; i < _size; i++ ) visit ( _elem[i] ); //遍历向量
}
 
template <typename T> template <typename VST> //元素类型、操作器
void Vector<T>::traverse ( VST& visit ){ //借助函数对象机制
    for ( int i = 0; i < _size; i++ ) visit ( _elem[i] ); //遍历向量
}