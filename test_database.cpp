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

int main() {
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
            // std::vector<int> v = database.find_with_vector(s);
            // if (v.size() == 0) cout << "null\n";
            // else {
            //     for (int x : v) cout << x << ' ';
            //     cout << '\n';
            // }
            int cnt = database.find_with_opt(hash(s), print_value);
            if (cnt == 0) cout << "null\n";
            else cout << '\n';
        }
        else assert(0);
    }
    return 0;
}