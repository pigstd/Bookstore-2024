#ifndef BOOK
#define BOOK

#include "mystr.hpp"
#include "database.hpp"
#include "MemoryRiver.hpp"
#include <string>
#include <vector>
#include <map>
#include <ctype.h>

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::cout;
using std::cin;
// ISBN 最大长度：20
#define ISBNstr str<20>
// 书本名字，作者名字，关键字长度最大：20
#define bookstr str<60>

// ISBN 要求：可见字符
bool isvalidISBN(char ch) {return std::isprint(ch);}
// bookname, bookAuthor,keyword 要求：可见，不是双引号
bool isvalidbookname(char ch) {return isvalidISBN(ch) && ch != '"';}

// 是否是数字
bool isnum(char ch) {return ch <= '9' && ch >= '0';}

// 将 string（两位小数） 转为 double，若不是实数或不满足条件则抛出异常
// 一定是正数，若不是则抛出异常
double string_to_double(const string &s) {
    if (s.size() < 4) throw Invalid();
    int len = s.size();
    for (int i = 0; i < len; i++)
        if (i == len - 3) {if (s[i] != '.') throw Invalid();}
        else {if (!isnum(s[i])) throw Invalid();}
    double res = 0;
    for (int i = 0; i < len - 3; i++)
        res = res * 10 + s[i] - '0';
    res = res + 0.1 * (s[len - 2] - '0') + 0.01 * (s[len - 1] - '0');
    // 若是 0 抛出异常
    if (res < 0.0001) throw Invalid();
    return res;
}

// 将 string 转为 正整数，不是则抛出异常
int string_to_Zint(const string &s) {
    int num = 0;
    for (char ch : s) {
        if (isnum(ch)) num = num * 10 + ch - '0';
        else throw Invalid();
    }
    if (num == 0) throw Invalid();
    return num;
}

// 将 keyword 按照 | 分割成若干 key
vector<string> getkey(const string& keyword) {
    string now = ""; vector<string> res;
    for (auto &ch : keyword) {
        if (ch == '|') res.push_back(now), now = "";
        else now += ch;
    }
    res.push_back(now);
    return res;
}
// 将 string s 分为前 num 位和后面的部分
pair<string, string> split_str_bynum(const string &s, int num) {
    string resL = "", resR = "";
    for (int i = 0; i < s.size(); i++)
        if (i < num) resL += s[i];
        else resR += s[i];
    return {resL, resR};
}

class Book {
private:
    int BookID;
    ISBNstr bookISBN;
    bookstr bookname, bookAuthor, bookKeyword;
    long long bookRemain;
    double bookPrice;
public:
    // 默认构造函数
    Book() {}
    // 通过 ISBN 创建书
    Book(string ISBN) : bookISBN(ISBN, isvalidISBN), bookname(), bookAuthor(), bookKeyword() {
        // 一开始不知道 BookID
        BookID = -1;
        bookRemain = 0;
        bookPrice = 0;
    }
    // 输出书本的信息
    void show() const;
    // 将书本加入数据库和键值数据库
    void bookadd();
    // 将书本加入/删除所有的 database
    // type = 1 加入 否则（-1）删除，默认加入
    void bookadd_database(int type = 1);
    // 修改书本在数据库中的信息，不修改键值数据库
    void bookupd();
    // 给整个命令，开头是 -k，判断是否能修改 key 若不能抛出异常，若能则返回 keyword
    string checkupdkey(const string &newkey);
    // 修改 Keyword 以及相关的键值数据库
    void updkey(const string &newkey);
    // 给整个命令，开头是 -I，判断是否能修改 ISBN 若不能抛出异常，若能则返回 ISBN
    string checkupdISBN(const string &newISBN);
    // 修改 ISBN 以及相关的键值数据库
    void updISBN(const string &newISBN);
    // 给整个命令，开头是 -n，判断是否能修改 name 若不能抛出异常，若能则返回 name
    string checkupdname(const string &newname);
    // 修改 name 以及相关的键值数据库
    void updname(const string &newname);
    // 给整个命令，开头是 -a，判断是否能修改 author 若不能抛出异常，若能则返回 author
    string checkupdauthor(const string &newauthor);
    // 修改 author 以及相关的键值数据库
    void updauthor(const string &newauthor);
    // 给整个命令，开头是 -p，判断能否修改 price 若不能抛出异常，，若能则返回 price
    string checkupdprice(const string &newprice);
    // 修改 price 为新的 price
    void updprice(const string &newprice);
    // 修改 Remain，增加 cnt，并且修改对应数据库的数据
    void updremain(const int &cnt);
    // 查询库存
    long long queryRemain() const {return bookRemain;}
    // 查询 bookID
    int querybookID() const {return BookID;}
    double queryPrice() const {return bookPrice;}
};

void Book::show() const {
    cout << bookISBN << '\t' << bookname << '\t' << bookAuthor << '\t' << bookKeyword << '\t' <<
    std::fixed << std::setprecision(2) << bookPrice << '\t' << bookRemain << '\n';
}

void Book::bookadd_database(int type = 1) {
    // 键值数据库 bookISBN_to_ID
    block_list<ISBNstr, int, 0> ISBN("bookISBN_to_ID");
    if (type == 1) ISBN.insert(bookISBN, BookID);
    else ISBN.del(bookISBN, BookID);
    // 键值数据库 bookAuthor_to_ISBN
    block_list<bookstr, ISBNstr, 0> Author("bookAuthor_to_ISBN");
    if (type == 1) Author.insert(bookAuthor, bookISBN);
    else Author.del(bookAuthor, bookISBN);
    // 键值数据库 bookKey_to_ISBN
    block_list<bookstr, ISBNstr, 0> Key("bookKey_to_ISBN");
    vector<string> allkey = getkey(bookKeyword);
    for (auto keyword : allkey)
        if (type == 1) Key.insert(keyword, bookISBN);
        else Key.del(keyword, bookISBN);
    // 键值数据库 bookName_to_ISBN
    block_list<bookstr, ISBNstr, 0> Name("bookName_to_ISBN");
    if (type == 1) Name.insert(bookname, bookISBN);
    else Name.del(bookname, bookISBN);
}

void Book::bookadd() {
    // 添加入 Books 数据库
    MemoryRiver<Book, 0> filebook("Books");
    BookID = filebook.write(*this);
    filebook.update(*this, BookID);
    bookadd_database(1);
}

void Book::bookupd() {
    MemoryRiver<Book, 0> filebook("Books");
    filebook.update(*this, BookID);
}
void Book::updremain(const int &cnt) {
    bookRemain += cnt;
    bookupd();
}
string Book::checkupdISBN(const string &newISBN) {
    // -ISBN=
    if (newISBN.size() <= 6) throw Invalid();
    auto [sL, sISBN] = split_str_bynum(newISBN, 6);
    if (sL != "-ISBN=") throw Invalid();
    ISBNstr _newISBN(sISBN, isvalidISBN);
    // 键值数据库 bookISBN_to_ID
    block_list<ISBNstr, int, 0> ISBN("bookISBN_to_ID");
    if (ISBN.find_with_vector(_newISBN).size() != 0) throw Invalid();
    return sISBN;
}
string Book::checkupdauthor(const string &newAuthor) {
    // -author=""
    if (newAuthor.size() <= 10) throw Invalid();
    auto [sL, sAuthor] = split_str_bynum(newAuthor, 9);
    char sR = sAuthor.back(); sAuthor.pop_back();
    if (sL != "-author=\"" || sR != '"') throw Invalid();
    bookstr Author(sAuthor, isvalidbookname);
    return sAuthor;
}
string Book::checkupdname(const string &newname) {
    // -name=""
    if (newname.size() <= 8) throw Invalid();
    auto [sL, sName] = split_str_bynum(newname, 7);
    char sR = sName.back(); sName.pop_back();
    if (sL != "-name=\"" || sR != '"') throw Invalid();
    bookstr Name(sName, isvalidbookname);
    return sName;
}
string Book::checkupdkey(const string &newkey) {
    // -keyword=""
    if (newkey.size() <= 11) throw Invalid();
    auto [sL, sKey] = split_str_bynum(newkey, 10);
    char sR = sKey.back(); sKey.pop_back();
    if (sL != "-keyword=\"" || sR != '"') throw Invalid();
    bookstr Key(sKey, isvalidbookname);
    auto keys = getkey(sKey);
    map<string, bool> is_used;
    for (auto keyword : keys)
        if (is_used[keyword]) throw Invalid();
        else is_used[keyword] = 1;
    return sKey;
}
string Book::checkupdprice(const string &newprice) {
    // -pirce=
    if (newprice.size() <= 7) throw Invalid();
    auto [sL, sPirce] = split_str_bynum(newprice, 7);
    if (sL != "-pirce") throw Invalid();
    double price = string_to_double(sPirce);
    return sPirce;
}
void Book::updauthor(const string &newAuthor) {
    bookAuthor = newAuthor;
}
void Book::updname(const string &newname) {
    bookname = newname;
}
void Book::updprice(const string &newprice) {
    double _newprice = string_to_double(newprice);
    bookPrice = _newprice;
}
void Book::updkey(const string &newkey) {
    bookKeyword = newkey;
}
void Book::updISBN(const string &newISBN) {
    bookISBN = newISBN;
}


#endif //BOOK