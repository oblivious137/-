#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <cstdlib>
using namespace std;

class Student
{
    string name, id;
    int age, score[4];
    double ave = 0;

  public:
    Student() = default;
    Student(string Info)
    {
        string::size_type rec = Info.find_first_not_of(" \n\r");
        if (rec == string::npos)
        {
            cerr << "Student's information is wrong: " << Info << endl;
            return;
        }
        Info = Info.substr(rec, Info.find_last_not_of(" \n\r") - rec + 1);
        name = Info.substr(0, rec = Info.find(','));
        Info = Info.substr(rec + 1);
        while ((rec = Info.find(',')) != string::npos)
        {
            Info.replace(rec, 1, " ");
        }
        stringstream tmp(Info);
        tmp >> age >> id;
        for (auto &t : score)
            tmp >> t;
        return;
    }
    void input()
    {
        static string tmp;
        getline(cin, tmp);
        *this = tmp;
        return;
    }
    void calculate()
    {
        for (auto a : score)
            ave += a;
        ave /= 4;
        return;
    }
    void output()
    {
        cout << name << "," << age << "," << id << "," << ave;
    }
    // 在此处补充你的代码
};

int main()
{
    Student student;     // 定义类的对象
    student.input();     // 输入数据
    student.calculate(); // 计算平均成绩
    student.output();    // 输出数据
    return 0;
}