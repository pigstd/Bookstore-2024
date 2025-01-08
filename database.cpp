#include "src/MemoryRiver.hpp"
#include <iostream>
#include <assert.h>
using std::cin;
using std::cout;
using std::cerr;

const int MAX_BLOCK = 500;

class hash {
private:
    int hs1, hs2;
    static int base1, base2, mod1, mod2;
public:
    hash(const string &s = "") {
        hs1 = hs2 = 0;
        for (auto c : s)
            hs1 = (1ll * hs1 * base1 + (long long)(c) + 2ll) % mod1,
            hs2 = (1ll * hs2 * base2 + (long long)(c) + 2ll) % mod2;
    }
    bool operator < (const hash &b) const {return hs1 < b.hs1 || (hs1 == b.hs1 && hs2 < b.hs2);};
    bool operator == (const hash &b) const {return hs1 == b.hs1 && hs2 == b.hs2;};
    void show() const {
        cerr << hs1 << ' ' << hs2 << '\n';
    }
};
int hash::base1 = 131;
int hash::base2 = 137;
int hash::mod1 = 1e9 + 7;
int hash::mod2 = 998244353;

template<typename indexname, typename valuename>
//index 的类型， value 的类型
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
    node(int sz = 0, int index = 0, int next = -1) {
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
    int head;
    MemoryRiver<node, 1> file_head;
    MemoryRiver<dataList, 0> file_body;
public:
    block_list(string s = "") {
        file_head.initialise(s + "_head", -1);
        file_body.initialise(s + "_body");
        file_head.get_info(head, 1);
    }
    void insert(const indexname &_index, const valuename &_val) {
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
    }
    void del(const indexname &_index, const valuename &_val) {
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
            return;
        }
        for (int i = pos; i < res.sz; i++)
            List.a[i] = List.a[i + 1];
        res.sz--;
        List.a[res.sz] = data();
        file_body.update(List, res.index);
        file_head.update(res, index);
    }
    void find(const indexname &index) {
        int i = head; node res;
        int flag = 0, cnt = 0;
        while(i != -1) {
            file_head.read(res, i);
            if (flag == 1) {
                dataList blockdata;
                file_body.read(blockdata, res.index);
                for (int i = 0; i < res.sz; i++) {
                    if (blockdata[i].index == index) cnt++, cout << blockdata[i].value << ' ';
                    else {flag = -1; break;}
                }
            }
            else if (flag == 0) {
                if (res.max_data.index < index) {i = res.next; continue;}
                flag = 1;
                dataList blockdata;
                file_body.read(blockdata, res.index);
                for (int i = 0; i < res.sz; i++) {
                    if (blockdata[i].index == index) cnt++, cout << blockdata[i].value << ' ';
                    else if (index < blockdata[i].index) {flag = -1; break;}
                }
            }
            if (flag == -1) break;
            i = res.next;
        }
        if (cnt == 0) cout << "null\n";
        else cout << '\n';
    }
    ~block_list() {
        file_head.write_info(head, 1);
    }
};

int main() {
    // std::cerr << "new test2!\n";
    block_list<hash, int> database("test");
    int n; cin >> n;
    while(n--) {
        string s; cin >> s;
        if (s == "insert") {
            string s; int value;
            cin >> s >> value;
            database.insert(s, value);
        }
        else if (s == "delete") {
            string s; int value;
            cin >> s >> value;
            database.del(s, value);
        }
        else if (s == "find") {
            string s; cin >> s;
            database.find(hash(s));
        }
        else assert(0);
    }
    return 0;
}