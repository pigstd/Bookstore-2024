#include "src/database.hpp"
#include "src/myhash.hpp"
#include <iostream>
#include <functional>
using std::cin;
using std::cout;
using std::cerr;

void print_value(const int &x) {
    cout << x << ' ';
}

class mystr {
    int len;
    char ch[65];
public:
    mystr() {
        len = 0;
        ch[0] = '\0';
        for (int i = 1; i < 65; i++) ch[i] = 0;
    }
    mystr(string s) {
        len = s.size();
        for (int i = 0; i < len; i++) ch[i] = s[i];
        ch[len] = '\0';
        for (int i = len + 1; i < 65; i++) ch[i] = 0;
    }
    bool operator <(const mystr& b) const {
        for (int i = 0; i < std::min(len, b.len); i++)
            if (ch[i] != b.ch[i]) return ch[i] < b.ch[i];
        return len < b.len;
    }
    bool operator ==(const mystr& b) const {
        if (len != b.len) return 0;
        for (int i = 0; i < len; i++)
            if (ch[i] != b.ch[i]) return 0;
        return 1;
    }
    // mystr& operator =(const mystr& b) {
    //     len = b.len;
    //     for (int i = 0; i < 65; i++)
    //         ch[i] = b.ch[i];
    //     return *this;
    // }
    void print() const {
        cout << len << ' ' << ch << '\n';
    }
};

int main() {
    block_list<mystr, int, 0> database("test");
    // string s = "233";
    // database.insert(s, 1);
    // cout << database.find_with_opt(s, print_value) << '\n';
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
            // std::vector<int> v = database.find_with_vector(s);
            // if (v.size() == 0) cout << "null\n";
            // else {
            //     for (int x : v) cout << x << ' ';
            //     cout << '\n';
            // }
            int cnt = database.find_with_opt(s, print_value);
            if (cnt == 0) cout << "null\n";
            else cout << '\n';
        }
        else assert(0);
    }
    return 0;
}