#ifndef DATABASE
#define DATABASE

#include "MemoryRiver.hpp"
#include <iostream>
#include <assert.h>
#include <functional>
using std::cin;
using std::cout;
using std::cerr;

const int MAX_BLOCK = 700;//一个块的大小

template<typename indexname, typename valuename, int is_cover = 0>
//index 的类型， value 的类型
//indexname 可比较（重载 < 和 ==）
class block_list {

class dataList;
class data {
private:
    indexname index;
    valuename value;
    friend class block_list;
    friend class dataList;
public:
    data() {
        index = indexname(), value = valuename();
    }
    data(indexname _index, valuename _val):index(_index), value(_val) {}
    bool operator <(const data &b) const {
        return index < b.index || (index == b.index && value < b.value);
    }
    bool operator == (const data &b) const {
        return index == b.index && value == b.value;
    }
    // void show() const {
    //     cerr << value << ' '; index.show();
    // }
};
class node {
private:
    int sz, index, next;
    data max_data;
    friend class block_list;
public:
    node(int sz = 0, int index = 0, int next = -1) : max_data() {
        this->sz = sz, this->index = index, this->next = next;
    }
};

class dataList {
private:
    data a[MAX_BLOCK];
    friend class block_list;
public:
    data& operator [] (int index) {
        return a[index];
    }
    dataList() {}
};

private:
    MemoryRiver<node, 1> file_head;
    MemoryRiver<dataList, 0> file_body;
public:
    block_list(string s = "") {
        file_head.initialise(s + "_head", -1, is_cover);
        file_body.initialise(s + "_body", 0 , is_cover);
    }
    void insert(const indexname &_index, const valuename &_val) {
        int head; file_head.get_info(head, 1);
        data new_data(_index, _val);
        node res;
        dataList List;
        int index = head;
        if (head != -1) {
            while(index != -1) {
                file_head.read(res, index);
                if (res.sz == 0 || new_data < res.max_data || res.next == -1) break;
                index = res.next;
            }
            file_body.read(List, res.index);
        }
        else {
            res.index = file_body.write(List);
            head = index = file_head.write(res);
        }
        res.sz++;
        int pos = 0;
        for (int i = res.sz - 2; i >= 0; i--)
            if (List.a[i] < new_data)
                {pos = i + 1; break;}
            else List.a[i + 1] = List.a[i];
        List.a[pos] = new_data;
        res.max_data = std::max(res.max_data, new_data);
        if (res.sz == MAX_BLOCK) {
            dataList new_List;
            node new_node(MAX_BLOCK - MAX_BLOCK / 2, 0, res.next);
            for (int i = MAX_BLOCK / 2; i < MAX_BLOCK; i++)
                new_node.max_data = std::max(new_node.max_data, List[i]),
                new_List[i - MAX_BLOCK / 2] = List[i], List[i] = data();
            res.max_data = List[0];
            for (int i = 1; i < MAX_BLOCK / 2; i++)
                res.max_data = std::max(res.max_data, List[i]);
            res.sz = MAX_BLOCK / 2;
            file_body.update(List, res.index);
            int newindex = file_body.write(new_List);
            new_node.index = newindex;
            res.next = file_head.write(new_node);
            file_head.update(res, index);
        }
        else {
            file_body.update(List, res.index);
            file_head.update(res, index);
        }
        file_head.write_info(head, 1);
    }
    void del(const indexname &_index, const valuename &_val) {
        int head; file_head.get_info(head, 1);
        data del_data(_index, _val);
        if (head == -1) return;
        node res; 
        int index = head, frontpoint = head;
        while(index != -1) {
            file_head.read(res, index);
            if (res.sz == 0 || !(res.max_data < del_data) || res.next == -1) break;
            frontpoint = index;
            index = res.next;
        }
        dataList List;
        file_body.read(List, res.index);
        int pos = -1;
        for (int i = 0; i < res.sz; i++)
            if (del_data == List.a[i])
                {pos = i; break;}
        if (pos == -1) return;
        if (res.sz == 1) {
            file_body.Delete(res.index);
            if (index == head) file_head.Delete(index), head = res.next;
            else {
                node front;
                file_head.read(front, frontpoint);
                front.next = res.next;
                file_head.update(front, frontpoint);
                file_head.Delete(index);
            }
            file_head.write_info(head, 1);
            return;
        }
        for (int i = pos; i < res.sz; i++)
            List.a[i] = List.a[i + 1];
        res.sz--;
        List.a[res.sz] = data();
        file_body.update(List, res.index);
        file_head.update(res, index);
        file_head.write_info(head, 1);
    }
    //找到所有索引为 index 的并且用 opt 函数操作，返回找到的元素个数
    int find_with_opt(const indexname &index, std::function<void(const valuename&)> opt) {
        int head; file_head.get_info(head, 1);
        int i = head; node res;
        int flag = 0, cnt = 0;
        while(i != -1) {
            file_head.read(res, i);
            if (flag == 1) {
                dataList blockdata;
                file_body.read(blockdata, res.index);
                for (int i = 0; i < res.sz; i++) {
                    if (blockdata[i].index == index) cnt++, opt(blockdata[i].value);
                    else {flag = -1; break;}
                }
            }
            else if (flag == 0) {
                if (res.max_data.index < index) {i = res.next; continue;}
                flag = 1;
                dataList blockdata;
                file_body.read(blockdata, res.index);
                for (int i = 0; i < res.sz; i++) {
                    if (blockdata[i].index == index) cnt++, opt(blockdata[i].value);
                    else if (index < blockdata[i].index) {flag = -1; break;}
                }
            }
            if (flag == -1) break;
            i = res.next;
        }
        return cnt;
    }
    //找到所有索引为 index，返回 vector
    std::vector<valuename> find_with_vector(const indexname &index) {
        int head; file_head.get_info(head, 1);
        std::vector<valuename> allvalue;
        int i = head; node res;
        int flag = 0, cnt = 0;
        while(i != -1) {
            file_head.read(res, i);
            if (flag == 1) {
                dataList blockdata;
                file_body.read(blockdata, res.index);
                for (int i = 0; i < res.sz; i++) {
                    if (blockdata[i].index == index) cnt++, allvalue.push_back(blockdata[i].value);
                    else {flag = -1; break;}
                }
            }
            else if (flag == 0) {
                if (res.max_data.index < index) {i = res.next; continue;}
                flag = 1;
                dataList blockdata;
                file_body.read(blockdata, res.index);
                for (int i = 0; i < res.sz; i++) {
                    if (blockdata[i].index == index) cnt++, allvalue.push_back(blockdata[i].value);
                    else if (index < blockdata[i].index) {flag = -1; break;}
                }
            }
            if (flag == -1) break;
            i = res.next;
        }
        return allvalue;
    }
};


#endif //DATABASE