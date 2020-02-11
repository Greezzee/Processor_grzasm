#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include "../GLib/error_msg.h"
template <typename T>
class Stack
{
public:
    Stack();
    ~Stack();

    void Push(T val);
    T Pop();
    T Top();

    void PrintStack();

private:
    T* _data;
    size_t _size;
    size_t _capacity;
};

template <typename T>
Stack<T>::Stack()
{
    _data = nullptr;
    _size = 0;
    _capacity = 0;
}

template <typename T>
Stack<T>::~Stack()
{
    if (_data != nullptr)
        free(_data);
    _size = 0;
    _capacity = 0;
}

template <typename T>
void Stack<T>::Push(T val)
{
    if (_size + 1 >= _capacity) {
        _data = (T*)realloc(_data, (_capacity * 2 + 1) * sizeof(T));
        if (_data == nullptr)
            Error::RuntimeErr("Stack overflow");
        _capacity = _capacity * 2 + 1;
    }
    _data[_size] = val;
    _size++;
}

template<typename T>
T Stack<T>::Pop()
{
    if (_size <= 0)
        Error::RuntimeErr("Empty Stack");
    T out = _data[_size - 1];
    _size--;
    return out;
}

template<typename T>
T Stack<T>::Top()
{
    if (_size <= 0)
        Error::RuntimeErr("Empty Stack");
    T out = _data[_size - 1];
    return out;
}

template<typename T>
void Stack<T>::PrintStack()
{
    for (int i = 0; i < _size; i++)
        printf("%Ld ", _data[i]);
}

#endif