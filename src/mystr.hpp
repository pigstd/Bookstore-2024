#ifndef MYSTR
#define MYSTR
#include "Exception.hpp"
#include <string>
#include <iostream>
#include <functional>
using std::ostream;

// 长度最长为 MAX_Len 的字符串
//支持比较，判等
template<int MAX_Len>
class str {
template<int MAX__Len>
    friend bool operator < (const str<MAX__Len> &a, const str<MAX__Len> &b);
template<int MAX__Len>
    friend bool operator == (const str<MAX__Len> &a, const str<MAX__Len> &b);
template<int MAX__Len>
    friend ostream& operator << (ostream &os, const str<MAX__Len> &b);
private:
    char ch[MAX_Len + 1];
public:
    // 构造函数（传入 std::string）
    str(std::string s = "") {
        if (s.size() > MAX_Len) throw Invalid();
        for (int i = 0; i < (int)s.size(); i++) ch[i] = s[i];
        for (int i = (int)s.size(); i <= MAX_Len; i++) ch[i] = '\0';
    }
    str(const str<MAX_Len> &s) {
        for (int i = 0; i <= MAX_Len; i++) ch[i] = s.ch[i];
    }
    str& operator=(const str<MAX_Len> &s) {
        for (int i = 0; i <= MAX_Len; i++) ch[i] = s.ch[i];
        return *this;
    }
    //判断这个字符串的所有字符是否都满足 checkfun 函数的限制
    bool check(std::function<bool(char)> checkfun) {
        for (int i = 0; i <= MAX_Len; i++)
            if (ch[i] == '\0') return true;
            else if (checkfun(ch[i]) == false)
                return false;
        return true;
    }
};

template<int MAX__Len>
bool operator <(const str<MAX__Len> &a, const str<MAX__Len> &b) {
    for (int i = 0; i <= MAX__Len; i++)
        if (a.ch[i] != b.ch[i]) return a.ch[i] < b.ch[i];
    return false;
}

template<int MAX__Len>
bool operator ==(const str<MAX__Len> &a, const str<MAX__Len> &b) {
    for (int i = 0; i <= MAX__Len; i++)
        if (a.ch[i] != b.ch[i]) return false;
    return true;
}

template<int MAX__Len>
ostream& operator << (ostream &os, const str<MAX__Len> &b) {
    for (int i = 0; i <= MAX__Len; i++)
        if (b.ch[i] == '\0') break;
        else os << b.ch[i];
    return os;
}

#endif //MYSTR