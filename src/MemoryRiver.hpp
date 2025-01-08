#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP

#include <fstream>
#include <filesystem>
#include <iostream>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

const string delname = "_del";
template<class T, int info_len = 2>
class MemoryRiver {
private:
    /* your code here */
    fstream file;
    fstream file_del;
    string file_name;
    int sizeofT = sizeof(T);
public:
    MemoryRiver() = default;

    MemoryRiver(const string& file_name) : file_name(file_name) {}

    void initialise(string FN = "", int num = 0, int is_cover = 1) {
        if (FN != "") file_name = FN;
        // 如果存在文件，就不修改，视情况选择是否强制覆盖
        if (is_cover && std::filesystem::exists(file_name)) return;
        file.open(file_name, std::ios::out);
        file_del.open(file_name + delname, std::ios::out);
        int tmp = 0, tmp_1 = num;
        for (int i = 0; i < info_len; ++i)
            file.write(reinterpret_cast<char *>(&tmp_1), sizeof(int));
        file_del.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    //读出第n个int的值赋给tmp，1_base
    void get_info(int &tmp, int n) {
        if (n > info_len) return;
        file.open(file_name, std::ios::in);
        file.seekg((n - 1) * sizeof(int), std::ios::beg);
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    //将tmp写入第n个int的位置，1_base
    void write_info(int tmp, int n) {
        if (n > info_len) return;
        file.open(file_name, std::ios::out | std::ios::in);
        file.seekp((n - 1) * sizeof(int), std::ios::beg);
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置
    int write(T &t) {
        file_del.open(file_name + delname, std::ios::in);
        file_del.seekg(0, std::ios::beg);
        int sz, index;
        file_del.read(reinterpret_cast<char *>(&sz), sizeof(int));
        file.open(file_name, std::ios::out | std::ios::in);
        // std::cerr << "!! " << sz << '\n';
        if (sz != 0) {
            file_del.seekg(sz * sizeof(int), std::ios::beg);
            file_del.read(reinterpret_cast<char *>(&index), sizeof(int));
            file_del.seekg(0, std::ios::beg);
            sz--;
            file_del.write(reinterpret_cast<char *>(&sz), sizeof(int));
            file.seekp(index, std::ios::beg);
        }
        else {            
            file.seekp(0,std::ios::end);
            index = file.tellp();
        }
        file.write(reinterpret_cast<char *>(&t), sizeof(T));
        file.close();
        file_del.close();
        return index;
    }
    //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
    void update(T &t, const int index) {
        file.open(file_name, std::ios::out | std::ios::in);
        file.seekp(index,std::ios::beg);
        file.write(reinterpret_cast<char *>(&t), sizeof(T));
        file.close();
    }

    //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
    void read(T &t, const int index) {
        file.open(file_name, std::ios::in);
        file.seekg(index,std::ios::beg);
        file.read(reinterpret_cast<char *>(&t), sizeof(T));
        file.close();
    }

    //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
    void Delete(int index) {
        file_del.open(file_name + delname, std::ios::out | std::ios::in);
        file_del.seekg(0, std::ios::beg);
        int sz;
        file_del.read(reinterpret_cast<char *>(&sz), sizeof(int));
        sz++;
        file_del.seekp(0, std::ios::beg);
        file_del.write(reinterpret_cast<char *>(&sz), sizeof(int));
        file_del.seekp(sz * sizeof(int), std::ios::beg);
        file_del.write(reinterpret_cast<char *>(&index), sizeof(int));
        file_del.close();
    }
};


#endif //BPT_MEMORYRIVER_HPP