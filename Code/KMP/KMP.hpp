#ifndef KMP_hpp
#define KMP_hpp

#include <cstring>

int* buildNext ( char* P ) { //构造模式串P的next表（改进版本）
    size_t m = strlen ( P ), j = 0; //“主”串指针
    int* N = new int[m]; //next表
    int t = N[0] = -1; //模式串指针
    while ( j < m - 1 )
        if ( 0 > t || P[j] == P[t] ) { //匹配
            j++;t++;
            N[j] = ( P[j] != P[t] ? t : N[t] ); //注意此句与未改进之前的区别
        } else //失配
            t = N[t];
    return N;
}

int match ( char* P, char* T ) {  //KMP算法
    int* next = buildNext ( P ); //构造next表
    int n = ( int ) strlen ( T ), i = 0; //文本串指针
    int m = ( int ) strlen ( P ), j = 0; //模式串指针
    while ( j < m  && i < n ) //自左向右逐个比对字符
        if ( 0 > j || T[i] == P[j] ) //若匹配，或P已移出最左侧（两个判断的次序不可交换）
            { i ++;  j ++; } //则转到下一字符
        else //否则
            j = next[j]; //模式串右移（注意：文本串不用回退）
    delete [] next; //释放next表
    return i - j;
}

#endif