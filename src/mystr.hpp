#include "Exception.hpp"

template<int MAX_Len>
class str {
friend bool operator < (const str<MAX_Len> &a, const str<MAX_Len> &b);
friend bool operator == (const str<MAX_Len> &a, const str<MAX_Len> &b);
    char ch[MAX_Len + 1];
public:
    str(string s = "") {
        if (s.size() > MAX_Len) throw Invalid();
        for (int i = 0; i < s.size(); i++) ch[i] = s[i];
        for (int i = s.size(); i <= MAX_Len; i++) ch[i] = '\0';
    }
    str(const str<MAX_Len> &s) {
        for (int i = 0; i <= MAX_Len; i++) ch[i] = s.ch[i];
    }
    str& operator=(const str<MAX_Len> &s) {
        for (int i = 0; i <= MAX_Len; i++) ch[i] = s.ch[i];
        return *this;
    }
};

template<int MAX_len>
bool operator ==(const str<MAX_len> &a, const str<MAX_len> &b) {
    for (int i = 0; i <= MAX_len; i++)
        if (a.ch[i] != b.ch[i]) return a.ch[i] < b.ch[i];
    return false;
}

template<int MAX_len>
bool operator ==(const str<MAX_len> &a, const str<MAX_len> &b) {
    for (int i = 0; i <= MAX_len; i++)
        if (a.ch[i] != b.ch[i]) return false;
    return true;
}