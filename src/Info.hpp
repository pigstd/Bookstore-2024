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

#endif //INFO