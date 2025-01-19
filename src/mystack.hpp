#ifndef MYSTACK
#define MYSTACK

#include "MemoryRiver.hpp"
#include "Exception.hpp"
#include <string>
#include <functional>
#include <assert.h>

using std::string;
using std::function;

// mystack : stack with memoryriver
template<typename T>
class mystack {
private:
    string filename;
    // 运用 MemoryRiver<T, 1> 在不使用 delete 的时候，index 分别是 8, 8 + sizeof(T), ... 的特性
    // 查询从栈底开始第 k 个数的下标（1 下标）
    int queryindex(int k) const {
        if (k <= 0) throw Invalid();
        return 8 + sizeof(T) * (k - 1);
    }
    // 修改 size
    void updsize(int sz) {
        if (sz < 0) throw Invalid();
        MemoryRiver<T, 1> file(filename);
        file.write_info(sz, 1);
    }
public:
    mystack(string _filename = "", int is_cover = 0) : filename(_filename) {
        if (is_cover == 0) return;
        MemoryRiver<T, 1> file;
        file.initialise(filename, 0, 1);
    }
    // 返回 stack 的 size
    int size() const;
    // 插入元素
    void insert(const T &);
    // 弹出最后一个元素
    void pop();
    // 从底开始遍历所有元素，并且使用 func 函数
    void func_with_all(function<void(const T&)>) const;
    // 从下开始遍历栈顶的 k 个元素，并且使用 func 函数
    void func_with_topk(int, function<void(const T&)>) const;
    // 栈顶元素
    T top() const;
};

template<typename T>
int mystack<T>::size() const {
    MemoryRiver<T, 1> file(filename);
    int sz;
    file.get_info(sz, 1);
    return sz;
}
template<typename T>
void mystack<T>::insert(const T &data) {
    T _data = const_cast<T &>(data);
    int sz = size(); sz++;
    MemoryRiver<T, 1> file(filename);
    file.update(_data, queryindex(sz));
    updsize(sz);
}
template<typename T>
void mystack<T>::pop() {
    int sz = size(); sz--;
    updsize(sz);
}
template<typename T>
void mystack<T>::func_with_all(function<void(const T&)> func) const {
    int sz = size();
    MemoryRiver<T, 1> file(filename);
    for (int i = 1; i <= sz; i++) {
        T data; file.read(data, queryindex(i));
        func(data);
    }
}
template<typename T>
void mystack<T>::func_with_topk(int k, function<void(const T&)> func) const {
    int sz = size();
    MemoryRiver<T, 1> file(filename);
    for (int i = sz - k + 1; i <= sz; i++) {
        T data; file.read(data, queryindex(i));
        func(data);
    }
}
template<typename T>
T mystack<T>::top() const {
    int sz = size();
    MemoryRiver<T, 1> file(filename);
    T data; file.read(data, queryindex(sz));
    return data;
}

#endif //MYSTACK