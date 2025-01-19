#include<bits/stdc++.h>
#include "../src/mystack.hpp"
using namespace std;

struct node {
    int x, y;
    node(int x = 0, int y = 0) {this->x = x, this->y = y;}
    void print() const {
        cout << x << ' ' << y << '\n';
    }
};

void print(const node &x) {
    x.print();
}

int main() {
    mystack<node> test("test", 1);
    test.insert(node(3, 4));
    test.insert(node(5, 6));
    cout << test.size() << '\n';
    test.func_with_all(print);
    test.pop();
    try {
        test.func_with_topk(1, print);
        test.func_with_topk(1, print);
        test.func_with_topk(4, print);
    }
    catch (exception &err) {
        cout << err.what() << '\n';
    }
    try {
        test.pop();
        cout << "pop!\n";
        test.pop();
        cout << "pop!\n";
        test.pop();
        cout << "pop!\n";
    }
    catch (exception &err) {
        cout << err.what() << '\n';
    }
    test.insert(node(3, 3));
    test.insert(node(4, 4));
    test.top().print();
    mystack<node> test2("test");
    test2.top().print();
    test2.func_with_all(print);
    return 0;
}