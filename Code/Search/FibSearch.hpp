#ifndef FibSearch_hpp
#define FibSearch_hpp

    #include "Fib.h"

    template <typename T> //Fibonacci查找算法：在有序向量的区间[lo, hi)内查找元素e，0 <= lo <= hi <= _size
    static int fibSearch ( T* S, T const& e, int lo, int hi ) {
        for( Fib fib ( hi - lo ); lo < hi;  ) { //Fib数列制表备查
        while( hi - lo < fib.get() ) fib.prev(); //自后向前顺序查找（分摊O(1)）
        int mi = lo + fib.get() - 1; //确定形如Fib(k) - 1的轴点
        ( e < S[mi] ) ? hi = mi : lo = mi + 1; //比较后确定深入前半段[lo, mi)或后半段(mi, hi)
        } //成功查找不能提前终止
        return --lo; //循环结束时，lo为大于e的元素的最小秩，故lo - 1即不大于e的元素的最大秩
    } //有多个命中元素时，总能保证返回最秩最大者；查找失败时，能够返回失败的位置

#endif