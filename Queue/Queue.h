#ifndef Queue_List_h
#define Queue_List_h
    #include "List\List.cpp" //以列表为基类，派生出栈模板类
    template <typename T>
    class Queue: public List<T> { //将列表的首/末端作为栈顶/底
    public: //size()、empty()以及其它开放接口，均可直接沿用
        void enqueue ( T const& e ) { this->insertAsLast ( e ); } //入队：尾部插入
        T dequeue() { return this->remove ( this->first() ); } //出队：首部删除
        T& front() { return this->first()->data; } //队首
    };
#endif
