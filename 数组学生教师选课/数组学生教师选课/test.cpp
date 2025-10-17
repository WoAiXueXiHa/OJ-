#include "info.hpp"
using namespace Info;


int main() {
    vector<SATI> data = {
        {"s1","a",85,"物理","t1","王"},
        {"s1","a",80,"数学","t2","赵"},
        {"s2","b",85,"物理","t1","王"},
        {"s2","b",80,"数学","t1","王"},
        {"s3","b",85,"物理","t1","王"},
        {"s3","b",80,"数学","t2","赵"},
    };

    // 1. 学生平均分
    double avg = avergeOneStu(data, "s1");
    if (avg < 0) cout << "此学生学号不存在\n";
    else cout << "学生 s1 的平均分 = " << avg << "\n";

    // 2. 老师各科平均分
    avergeOneTeacher(data, "t1");

    // 3. 对学生 s1：其老师中谁教过的不同学生最多
    mostStudentsTeacherFor(data, "s1");


    return 0;
}
