#ifndef BinSearch_hpp
#define BinSearch_hpp

    template <typename T>
    static int binSearch ( T* S, T const& e, int lo, int hi ) {
        while ( lo < hi ) { //每步迭代仅需做一次比较判断，有两个分支
            int mi = ( lo + hi ) >> 1; //以中点为轴点（区间宽度的折半，等效于宽度之数值表示的右移）
            ( e < S[mi] ) ? hi = mi : lo = mi + 1; //经比较后确定深入[lo, mi)或(mi, hi)
        } //成功查找不能提前终止
        return lo - 1; //循环结束时，lo为大于e的元素的最小秩，故lo - 1即不大于e的元素的最大秩
    }

#endif