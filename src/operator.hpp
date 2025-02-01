#ifndef OPERATOR
#define OPERATOR

#include "MemoryRiver.hpp"
#include "user.hpp"
#include "database.hpp"
#include "mystack.hpp"
#include "mystr.hpp"
#include "book.hpp"
#include "Info.hpp"
#include "Exception.hpp"
#include <string>
#include <vector>

using std::pair;
using std::string;
using std::vector;
using std::map;
using std::cout;
using std::cin;

struct User_login {
    int user, select;
    User_login(int _user, int _select = -1) : user(_user), select(_select) {}
};

#define Loginstack vector<User_login>

/*
主要操作：
init 初始化
split_string 将一整行按照空格分开
operation 进行操作
check_Privilege 检验权限是否满足
*/
// 初始化
void init();
//将一整行按照空格分开
vector<string> split_string(string s) {
    vector<string> res;
    string now = "";
    for (char ch : s) {
        if (ch == ' ') {
            if (now != "") res.push_back(now);
            now = "";
        }
        else now += ch;
    }
    if (now != "") res.push_back(now);
    return res;
}
// 进行操作
void operation(Loginstack &LoginStack, string &oprstr);

/*
# 帐户系统指令
su [UserID] ([Password])?
logout
register [UserID] [Password] [Username]
passwd [UserID] ([CurrentPassword])? [NewPassword]
useradd [UserID] [Password] [Privilege] [Username]
delete [UserID]
*/

// 找到 UserID 对应的 UserID_int，如果不存在就抛出异常
int findUser(userstr UserID) {
    block_list<userstr, int, 0> databaseuser("UserID_to_int");
    vector<int> all = databaseuser.find_with_vector(UserID);
    if (all.size() == 0) throw Invalid();
    // assert(all.size() == 1);//一个 UserID 只能对应一个 UserID_int
    return all[0];
}


// 找到 UserID_int 对应的 UserID，如果不存在就抛出异常
User findUser(int UserID_int) {
    User res;
    MemoryRiver<User, 0> fileuser("Users");
    fileuser.read(res, UserID_int);
    return res;
}

// 检验当前登录栈的权限是否 >= privilege，若不满足则抛出异常
void check_Privilege(Loginstack &LoginStack, usertype privilege) {
    if (findUser(LoginStack.back().user).gettype() < privilege)
        throw Invalid();
}

// su 登录 传入当前的登录栈以及指令
void su(Loginstack &LoginStack, vector<string> orders) {
    if (orders.size() < 2 || orders.size() > 3) throw Invalid();
    userstr UserID(orders[1], isvalidname);
    int UserID_int = findUser(UserID);
    User su_user = findUser(UserID_int);
    // cerr << "userFound\n";
    if (orders.size() == 3) {
        if (su_user.checkpassword(orders[2]) == false) throw Invalid();
    }
    else if (findUser(LoginStack.back().user).gettype() <= su_user.gettype()) throw Invalid();
    // 可以登录
    su_user.Loginupd(+1);
    LoginStack.push_back({su_user.getID_int(), -1});
}
// logout 登出 传入当前的用户以及指令
void logout(Loginstack &LoginStack, vector<string> orders) {
    check_Privilege(LoginStack, customer);
    if (orders.size() != 1) throw Invalid();
    if (LoginStack.size() == 1) throw Invalid();
    findUser(LoginStack.back().user).Loginupd(-1);
    LoginStack.pop_back();
}
// register 注册 传入当前的用户以及指令
void registeruser(Loginstack &LoginStack, vector<string> orders) {
    if (orders.size() != 4) throw Invalid();
    User newuser(orders[1], orders[2], orders[3]);
    newuser.useradd();
}
// passwd 修改密码 传入当前的用户以及指令
void passwd(Loginstack &LoginStack, vector<string> orders) {
    if (orders.size() < 3 || orders.size() > 4) throw Invalid();
    check_Privilege(LoginStack, customer);
    userstr UserID(orders[1], isvalidname);
    int UserID_int = findUser(UserID);
    User upduser = findUser(UserID_int);
    if (orders.size() == 3) {
        check_Privilege(LoginStack, owner);
        upduser.changepassword(orders[2]);
    }
    else
        upduser.changepassword(orders[2], orders[3]);
}
// useradd 添加用户 传入当前的用户以及指令
void useradd(Loginstack &LoginStack, vector<string> orders) {
    check_Privilege(LoginStack, employee);
    if (orders.size() != 5) throw Invalid();
    User newuser(orders[1], orders[2], orders[3], orders[4]);
    if (newuser.gettype() >= findUser(LoginStack.back().user).gettype()) throw Invalid();
    newuser.useradd();
}
// delete 删除用户 传入当前的用户以及指令
void deleteuser(Loginstack &LoginStack, vector<string> orders) {
    if (orders.size() != 2) throw Invalid();
    check_Privilege(LoginStack, owner);
    // 如果待删除账户已经登录则操作失败
    userstr UserID(orders[1], isvalidname);
    int UserID_int = findUser(UserID);
    User deluser = findUser(UserID_int);
    if (deluser.isLogin()) throw Invalid();
    MemoryRiver<User, 0> fileuser("Users");
    fileuser.Delete(UserID_int);
    block_list<userstr, int, 0> databaseuser("UserID_to_int");
    databaseuser.del(UserID, UserID_int);
}

/*
# 图书系统指令
show (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]")?
buy [ISBN] [Quantity]
select [ISBN]
modify (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+
import [Quantity] [TotalCost]
*/

// show 检索图书
void showbook(Loginstack &LoginStack, vector<string> &orders);
// buy 购买图书
void buy(Loginstack &LoginStack, vector<string> &orders);
// select 选择图书
void select(Loginstack &LoginStack, vector<string> &orders);
// modify 修改图书信息
void modify(Loginstack &LoginStack, vector<string> &orders);
// import 进货
void import(Loginstack &LoginStack, vector<string> &orders);

// 输出 BookID 对应的书本的信息
void show_with_ID(const int &BookID) {
    MemoryRiver<Book, 0> filebook("Books");
    Book book; filebook.read(book, BookID);
    book.show();
}
// 输出 BookISBN 对应的书本的信息，若不存在输出空行
void show_with_ISBN(const ISBNstr &BookISBN) {
    block_list<ISBNstr, int, 0> ISBN("bookISBN_to_ID");
    auto ID = ISBN.find_with_vector(BookISBN);
    if (ID.size() == 0) cout << '\n';
    else show_with_ID(ID[0]);
}
// 返回 BookID 对应的书，若 ID 是 -1 则抛出异常
Book find_with_BookID(const int &BookID) {
    if (BookID == -1) throw Invalid();
    MemoryRiver<Book, 0> filebook("Books");
    Book book; filebook.read(book, BookID);
    return book;
}
// 返回 BookISBN 对应的 BookID，若不存在返回 -1
int find_with_ISBN(const string &BookISBN) {
    block_list<ISBNstr, int, 0> ISBN("bookISBN_to_ID");
    auto ID = ISBN.find_with_vector(BookISBN);
    if (ID.size() == 0) return -1;
    else return ID[0];
}

void showbook(Loginstack &LoginStack, vector<string> &orders) {
    check_Privilege(LoginStack, customer);
    // show all
    if (orders.size() == 1) {
        // 键值数据库 bookISBN_to_ID
        block_list<ISBNstr, int, 0> ISBN("bookISBN_to_ID");
        if (ISBN.all_opt(show_with_ID) == 0)
            cout << '\n';// 无满足要求的图书时输出空行
        return;
    }
    if (orders.size() != 2) throw Invalid();
    string order = orders[1];
    if (order.size() <= 2) throw Invalid();
    if (order[1] == 'I') {
        // -ISBN=
        if (order.size() <= 6) throw Invalid();
        auto [sL, sISBN] = split_str_bynum(order, 6);
        ISBNstr ISBN(sISBN, isvalidISBN);
        show_with_ISBN(ISBN);
    }
    else if (order[1] == 'n') {
        bookstr name(Book().checkupdname(order));        
        // 键值数据库 bookName_to_ISBN
        block_list<bookstr, ISBNstr, 0> Name("bookName_to_ISBN");
        if (Name.find_with_opt(name, show_with_ISBN) == 0)
            cout << '\n';
    }
    else if (order[1] == 'a') {
        bookstr author(Book().checkupdauthor(order));
        // 键值数据库 bookAuthor_to_ISBN
        block_list<bookstr, ISBNstr, 0> Author("bookAuthor_to_ISBN");
        if (Author.find_with_opt(author, show_with_ISBN) == 0)
            cout << '\n';
    }
    else if (order[1] == 'k') {
        string keywords = Book().checkupdkey(order);
        auto keyvec = getkey(keywords);
        if (keyvec.size() != 1) throw Invalid();
        // 键值数据库 bookKey_to_ISBN
        block_list<bookstr, ISBNstr, 0> Key("bookKey_to_ISBN");
        if (Key.find_with_opt(keyvec[0], show_with_ISBN) == 0)
            cout << '\n';
    }
    else throw Invalid();
}

void buy(Loginstack &LoginStack, vector<string> &orders) {
    check_Privilege(LoginStack, customer);
    if (orders.size() != 3) throw Invalid();
    Book book = find_with_BookID(find_with_ISBN(orders[1]));
    int buynum = string_to_Zint(orders[2]);
    if (book.queryRemain() < buynum) throw Invalid();
    book.updremain(-buynum);
    double buysum = buynum * book.queryPrice();
    cerr << buynum << ' ' << book.queryPrice() << '\n';
    cout << std::fixed << std::setprecision(2) << buysum << '\n';
    // !!!
    // to be done:
    // 需要把价格的信息加入销售信息
    // 等待 Info.hpp 写完后完成

}

void select(Loginstack &LoginStack, vector<string> &orders) {
    check_Privilege(LoginStack, employee);
    if (orders.size() != 2) throw Invalid();
    int BookID = find_with_ISBN(orders[1]);
    if (BookID == -1) {
        Book newbook(orders[1]);
        newbook.bookadd();
        BookID = newbook.querybookID();
    }
    LoginStack.back().select = BookID;
}

void modify(Loginstack &LoginStack, vector<string> &orders) {
    check_Privilege(LoginStack, employee);
    int BookID = LoginStack.back().select;
    if (BookID == -1) throw Invalid();
    Book upd_book = find_with_BookID(BookID);
    enum modifyType {_ISBN = 0, _name, _author, _keyword, _price};
    int is_modified[] = {0, 0, 0, 0, 0};
    //first : 修改后的名字 second : 类型
    vector<pair<string, modifyType>> updinfo;
    for (int i = 1; i < (int)orders.size(); i++) {
        string order = orders[i];
        modifyType type;
        switch (order[1]) {
            case 'I':
                type = _ISBN;
                updinfo.push_back({upd_book.checkupdISBN(order), type});
                break;
            case 'n':
                type = _name;
                updinfo.push_back({upd_book.checkupdname(order), type});
                break;
            case 'a':
                type = _author;
                updinfo.push_back({upd_book.checkupdauthor(order), type});
                break;
            case 'k':
                type = _keyword;
                updinfo.push_back({upd_book.checkupdkey(order), type});
                break;
            case 'p':
                type = _price;
                updinfo.push_back({upd_book.checkupdprice(order), type});
                break;
            default: throw Invalid();
        }
        if (is_modified[type]) throw Invalid();
        is_modified[type] = 1;
    }
    // 可以修改
    // 删去原本在键值数据库的信息
    upd_book.bookadd_database(-1);
    for (auto [newinfo, type] : updinfo) {
        switch (type)
        {
            case _ISBN:
                upd_book.updISBN(newinfo);
                break;
            case _name:
                upd_book.updname(newinfo);
                break;
            case _author:
                upd_book.updauthor(newinfo);
                break;
            case _keyword:
                upd_book.updkey(newinfo);
                break;
            case _price:
                upd_book.updprice(newinfo);
                break;
        }
    }
    upd_book.bookadd_database(1);
    upd_book.bookupd();
}

void import(Loginstack &LoginStack, vector<string> &orders) {
    check_Privilege(LoginStack, employee);
    if (orders.size() != 3) throw Invalid();
    int BookID = LoginStack.back().select;
    if (BookID == -1) throw Invalid();
    Book book = find_with_BookID(BookID);
    int Quantity = string_to_Zint(orders[1]);
    double TotalCost = string_to_double(orders[2]);
    book.updremain(+Quantity);
    // !!!
    // to be done:
    // 需要把交易额度的信息加入销售信息
    // 等待 Info.hpp 写完后完成

}

//初始化
void init() {
    /*
    需要检查是否是第一次启动，通过检查文件是否存在即可
    到最后再加上这个功能吧，调试的时候可以去掉，毕竟比较麻烦
    */

    /*
    User 的初始化：Users, UserID_to_int
    */
    MemoryRiver<User, 0> fileuser;
    fileuser.initialise("Users", 0, 1);
    block_list<userstr, int, 1> databaseuser("UserID_to_int");
    // 注册超级管理员 root
    User root("root", "sjtu", "7", "superadmin");
    root.useradd();
    // book 的初始化
    MemoryRiver<Book, 0> filebook("Books");
    filebook.initialise("Books", 0, 1);
    block_list<ISBNstr, int, 1> ISBN("bookISBN_to_ID");
    block_list<bookstr, ISBNstr, 1> Author("bookAuthor_to_ISBN");
    block_list<bookstr, ISBNstr, 1> Key("bookKey_to_ISBN");
    block_list<bookstr, ISBNstr, 1> Name("bookName_to_ISBN");
}


// 使用名字前面带下划线来区分
enum operatorType {_Invalid, _quit,// 基础指令
_su, _logout, _register, _passwd, _useradd, _delete, // 账户系统指令
_showbook, _buy, _select, _modify, _import, // 图书系统指令
_showfinance, _log, _reportfinance, _reportemployee, // 日志系统指令
_do_noting // 只有空格的指令，无输出内容
};

operatorType get_opt_type(vector<string> &orders) {
    // 基础指令 _Invalid, _exit
    if (orders.size() == 0) return _do_noting;
    operatorType type = _Invalid;
    if (orders[0] == "exit" || orders[0] == "quit") type = _quit;
    // 账户系统指令 _su, _logout, _register, _passwd, _useradd, _delete
    if (orders[0] == "su") type = _su;
    if (orders[0] == "logout") type = _logout;
    if (orders[0] == "register") type = _register;
    if (orders[0] == "passwd") type = _passwd;
    if (orders[0] == "useradd") type = _useradd;
    if (orders[0] == "delete") type = _delete;
    // 图书系统指令 _showbook, _buy, _select, _modify, _import 
    if (orders[0] == "show") {
        if (orders.size() >= 2 && orders[1] == "finance") type = _showfinance;
        else type = _showbook;
    }
    if (orders[0] == "buy") type = _buy;
    if (orders[0] == "select") type = _select;
    if (orders[0] == "modify") type = _modify;
    if (orders[0] == "import") type = _import;
    // 日志系统指令 _showfinance, _log, _reportfinance, _reportemployee
    if (orders[0] == "log") type = _log;
    if (orders[0] == "report" && orders.size() >= 2) {
        if (orders[1] == "finance") type = _reportfinance;
        if (orders[1] == "employee") type = _reportemployee;
    }
    return type;
}

void quit(Loginstack &LoginStack, vector<string> &orders) {
    // cerr << "quit!\n";
    if (orders.size() != 1) throw Invalid();
    vector<string> logoutorders(1, "logout");
    while(LoginStack.size() != 1) logout(LoginStack, logoutorders);
    exit(0);
}

void operation(Loginstack &LoginStack, string &optstr) {
    vector<string> orders = split_string(optstr);
    operatorType type = get_opt_type(orders);
    switch (type) {
    case _do_noting:
        //do noting
        break;
    case _quit:
        quit(LoginStack, orders);
        break;
    case _su:
        su(LoginStack, orders);
        break;
    case _logout:
        logout(LoginStack, orders);
        break;
    case _register:
        registeruser(LoginStack, orders);
        break;
    case _passwd:
        passwd(LoginStack, orders);
        break;
    case _useradd:
        useradd(LoginStack, orders);
        break;
    case _delete:
        deleteuser(LoginStack, orders);
        break;
    case _showbook:
        showbook(LoginStack, orders);
        break;
    case _buy:
        buy(LoginStack, orders);
        break;
    case _select:
        select(LoginStack, orders);
        break;
    case _modify:
        modify(LoginStack, orders);
        break;
    case _import:
        import(LoginStack, orders);
        break;
    default:
        throw Invalid();
        break;
    }
}

#endif //OPERATOR