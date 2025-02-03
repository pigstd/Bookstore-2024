#ifndef INFO
#define INFO

#include "mystack.hpp"
#include "user.hpp"
#include "book.hpp"
#include <iostream>
#include <iomanip>

using std::cout;
using std::fixed;
using std::setprecision;


// saleinfo 部分
enum sale_type {_sale = 0, _buyin = 1};

class SaleInfo {
private:
    int SaleID, userID, bookID;
    sale_type SaleType;
    int SaleNum;
    ld SaleMoney;
public:
    // 空的构造函数
    SaleInfo();
    // 构造函数
    SaleInfo(int _userID, int _bookID, sale_type _Saletype, int _SaleNum, ld _SaleMoney);
    // 加入数据库 saleInfo
    void addInfo();
    // 输出销售信息 谁买了/卖了什么花了多少钱
    void printInfo() const;
    // 查询金额
    ld queryMoney() const;
    // 查询类型
    sale_type querytype() const;
};

void SaleInfo::addInfo() {
    mystack<SaleInfo> filesaleInfo("saleInfo");
    SaleID = filesaleInfo.insert(*this);
    filesaleInfo.pop();
    filesaleInfo.insert(*this);
}

void SaleInfo::printInfo() const {
    User user = findUser(userID);
    Book book = find_with_BookID(bookID);
    if (SaleType == _buyin)
        cout << fixed << setprecision(2) << "员工 " <<
        user.getname() << " 进货了 " << SaleNum << " 本名字为 " << book.queryname() << " 的书，花费了 " << SaleMoney << "元\n";
    else
        cout << fixed << setprecision(2) << "顾客 " <<
        user.getname() << " 买了 " << SaleNum << " 本名字为 " << book.queryname() << " 的书，赚了 " << SaleMoney << "元\n";
}

ld SaleInfo::queryMoney() const {return SaleMoney;}
sale_type SaleInfo::querytype() const {return SaleType;}

SaleInfo::SaleInfo() {};
SaleInfo::SaleInfo(int _userID, int _bookID, sale_type _Saletype, int _SaleNum, ld _SaleMoney) :
    SaleID(-1), userID(_userID), bookID(_bookID), SaleType(_Saletype), SaleNum(_SaleNum), SaleMoney(_SaleMoney) {}

// operatorinfo 部分

// operator 类型：使用名字前面带下划线来区分
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

#endif //INFO