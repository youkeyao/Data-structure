#ifndef PQ_h
#define PQ_h

    template <typename T>
    struct PQ { //优先级队列PQ模板类
        virtual void insert ( T ) = 0; //按照比较器确定的优先级次序插入词条
        virtual T getMax() = 0; //取出优先级最高的词条
        virtual T delMax() = 0; //删除优先级最高的词条
    };

#endif