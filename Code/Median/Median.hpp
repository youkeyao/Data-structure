#ifndef Median_hpp
#define Median_hpp

#include "Vector\Vector.hpp"

template <typename T> //子向量S1[lo1, lo1 + n1)和S2[lo2, lo2 + n2)分别有序，数据项可能重复
T trivialMedian ( Vector<T>& S1, int lo1, int n1, Vector<T>& S2, int lo2, int n2 ) {
    int hi1 = lo1 + n1, hi2 = lo2 + n2;
    Vector<T> S; //将两个有序子向量归并为一个有序向量
    while ( ( lo1 < hi1 ) && ( lo2 < hi2 ) ) {
        while ( ( lo1 < hi1 ) && S1[lo1] <= S2[lo2] ) S.insert ( S1[lo1 ++] );
        while ( ( lo2 < hi2 ) && S2[lo2] <= S1[lo1] ) S.insert ( S2[lo2 ++] );
    }
    while ( lo1 < hi1 ) S.insert ( S1[lo1 ++] );
    while ( lo2 < hi2 ) S.insert ( S1[lo2 ++] );
    return S[ ( n1 + n2 ) / 2]; //直接返回归并向量的中位数
}

template <typename T> //序列S1[lo1, lo1 + n)和S2[lo2, lo2 + n)分别有序，n > 0，数据项可能重复
T median ( Vector<T>& S1, int lo1, Vector<T>& S2, int lo2, int n ) { //中位数算法（高效版）
    if ( n < 3 ) return trivialMedian ( S1, lo1, n, S2, lo2, n ); //递归基
    int mi1 = lo1 + n / 2, mi2 = lo2 + ( n - 1 ) / 2; //长度（接近）减半
    if ( S1[mi1] < S2[mi2] )
        return median ( S1, mi1, S2, lo2, n + lo1 - mi1 ); //取S1右半、S2左半
    else if ( S1[mi1] > S2[mi2] )
        return median ( S1, lo1, S2, mi2, n + lo2 - mi2 ); //取S1左半、S2右半
    else
        return S1[mi1];
}

#endif
