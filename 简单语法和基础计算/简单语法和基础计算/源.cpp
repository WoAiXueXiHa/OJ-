// https://www.nowcoder.com/practice/ccb7383c76fc48d2bbc27a2a6319631c?tpId=62&&tqId=29468&rp=1&ru=
// KY111 日期差值
/*
#include <iostream>
#include <array>
using namespace std;

bool isLeapYear(int year){
    if((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        return true;
    else
        return false;
}

bool isMax(int year1, int year2, int month1, int month2, int day1, int day2){
    if(year1 != year2) return year1 > year2;
    if(month1 != month2) return month1 > month2;
    return day1 > day2;
}

int getMonthDay(int year, int month){
    array<int,13> arrDay = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if(month == 2 && isLeapYear(year)) return 29;
    else return arrDay[month];
}
int main(){
    int y1,m1,d1, y2,m2,d2;
    scanf("%4d%2d%2d",&y1,&m1,&d1);
    scanf("%4d%2d%2d",&y2,&m2,&d2);

    // 若第一组不是较大的日期，就交换（用 std::swap 或正确的 swap 实现）
    if (!isMax(y1,y2,m1,m2,d1,d2)) {
        swap(y1,y2);
        swap(m1,m2);
        swap(d1,d2);
    }

    int count = 1; // 若想计算包含端点的天数差
    while (y1!=y2 || m1!=m2 || d1!=d2) {
        ++count;
        ++d2;
        if (d2 > getMonthDay(y2,m2)) {
            d2 = 1;
            if (++m2 > 12) { m2 = 1; ++y2; }
        }
    }
    cout << count << endl;
    return 0;
}
*/

// https://www.nowcoder.com/practice/769d45d455fe40b385ba32f97e7bcded?tpId=37&&tqId=21296&rp=1&ru=/activity/oj&qru=/ta/huawei/question-ranking
// HJ73 日期到天数转换
/*
#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

int main() {
    vector<int> arrDay = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int year, month, day;
    // 累加天数
    int sum = 0;
    while(cin>>year>>month>>day){
        assert(year >= 1900 && year <= 2200);
        // 3: 0 + 31 + 28
        for(int i = 0; i < month - 1; ++i){
            sum += arrDay[i];
        }
    }
    // 判断闰年 过了2月这个闰月 都要+1
    if((month > 2) && ((year % 400 == 0)
        || (year % 100 != 0 && year % 4 == 0))){
            sum = sum + 1 + day;
    }else{
        sum += day;
    }
    cout<<sum<<endl;
    return 0;
}
// 64 位输出请用 printf("%lld")
*/

// https://www.nowcoder.com/practice/7a0da8fc483247ff8800059e12d7caf1?tpId=13&tqId=11200&tPage=3&rp=3&ru=/ta/coding-interviews&qru=/ta/coding-interviews/question-ranking
// JZ64 不用循环求 1+2+...+n
/*class Solution {
public:
class Sum{
    public:
    Sum(){
        _sum += _idx;
        ++_idx;
    }
};
    int Sum_Solution(int n) {
        _idx = 1;
        _sum = 0;
       std::vector<Sum> arr(n);   // 触发n次构造
        return _sum;
    }
    // 全局变量 类内声明
    static size_t _idx;
    static size_t _sum;
};
// 全局变量 类外定义
size_t Solution::_idx = 1;
size_t Solution::_sum = 0;*/

// https://www.nowcoder.com/practice/b1f7a77416194fd3abd63737cdfcf82b?tpId=69&&tqId=29669&rp=1&ru=/activity/oj&qru=/ta/hust-kaoyan/question-ranking
// KY222 打印日期
/*#include <iostream>
#include <array>
using namespace std;

int getMonthDay(int year, int month){
    array<int,13> arrDay = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if((month == 2) && ((year % 400 == 0) || (year % 100 != 0 && year % 4 == 0)))
        return 29;
    else
        return arrDay[month];
}
void printDate(int year, int days){
    int month = 1;
    while(days > getMonthDay(year,month)){
        // 从一月开始减
        days -= getMonthDay(year, month);
        ++month;
    }
    // 剩余的天数就是当月天数
    printf("%4d-%02d-%02d\n",year,month,days);
}
int main() {
    int year, days;
    while (cin >> year >> days) { 
        printDate(year, days);
    }
    return 0;
}
// 64 位输出请用 printf("%lld")*/