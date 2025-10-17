#include <iostream>
#include <string>
#include <vector>
using namespace std;

namespace Info {

    // 每门课一条记录
    struct stuAndTeacherInfo {
        string _stuID;
        string _stuName;
        int    _score;        // 单科分数 每条记录只有一个分数
        string _subject;
        string _teacherID;
        string _teacherName;
    };
    typedef stuAndTeacherInfo SATI;

    // 在 string 数组里找 key 的下标 找不到返回 -1 
    int idxFind(const vector<string>& data, const string& key) {
        for (int i = 0; i < (int)data.size(); ++i) {
            if (data[i] == key) return i;
        }
        return -1;
    }

    // ================== 1. 某个学生（按学号）的平均分 ==================
    // 同一学生会有多条记录（不同科目）这里把该学生所有记录的_score求平均
    // 返回：平均分 如果没有记录 返回 -1.0
    double avergeOneStu(const vector<SATI>& data, const string& stuID) {
        int sum = 0, cnt = 0;              
        for (const auto& e : data) {
            if (e._stuID == stuID) {       // 找到该学生ID就累加
                sum += e._score;
                ++cnt;
            }
        }
        if (cnt == 0) return -1.0;         
        return sum * 1.0 / cnt;
    }

    // ================== 2. 某个老师教的每门课的平均分 ==================
    // 思路：自己维护三条并行数组：subjects  sums  cnts
    // 每遇到一门新科目就开一格 之后同科目累加
    void avergeOneTeacher(const vector<SATI>& data, const string& teacherID) {
        vector<string> subjects;   // 去重后的科目列表
        vector<int>    sums;       // 对应科目的分数总和
        vector<int>    cnts;       // 对应科目的出现次数

        for (const auto& e : data) {
            if (e._teacherID == teacherID) {
                int pos = idxFind(subjects, e._subject);   // 找该科目是否已出现
                if (pos == -1) {                            // 第一次见到此科目
                    subjects.push_back(e._subject);
                    sums.push_back(e._score);
                    cnts.push_back(1);
                }
                else {                                    //  科目已存在，累加
                    sums[pos] += e._score;
                    ++cnts[pos];
                }
            }
        }

        if (subjects.empty()) {
            cout << teacherID << " 老师没有任何记录\n";
        }
        else {
            cout << teacherID << " 老师的各科平均分：\n";
            for (int i = 0; i < (int)subjects.size(); ++i) {
                cout << "  " << subjects[i] << " -> " << (sums[i] * 1.0 / cnts[i]) << "\n";
            }
        }
    }

    // ================== 3. 对于某个学生：其老师中谁教过的不同学生数最多 ==================
    // 步骤：
    //  1) 先找出这个学生上过课的老师ID列表 要去重
    //  2) 对于这些候选老师 统计被该老师教过的不同学生数 
    void mostStudentsTeacherFor(const vector<SATI>& data, const string& stuID) {
        // 1) 该学生接触过的老师列表 用 vector + idxFind 去重
        vector<string> teacherIDs;
        vector<string> teacherNames;  
        for (const auto& e : data) {
            if (e._stuID == stuID) {
                int pos = idxFind(teacherIDs, e._teacherID);
                if (pos == -1) {
                    teacherIDs.push_back(e._teacherID);
                    teacherNames.push_back(e._teacherName);
                }
            }
        }
        if (teacherIDs.empty()) {
            cout << "学生 " << stuID << " 没有老师记录\n";
            return;
        }

        // b 对每个候选老师 统计其不同学生数
        int bestIdx = -1;
        int bestCnt = -1;
        for (int i = 0; i < (int)teacherIDs.size(); ++i) {
            const string& T = teacherIDs[i];

            vector<string> distinctStu;    // 去重：该老师下已统计过的学生ID
            for (const auto& e : data) {
                if (e._teacherID == T) {
                    if (idxFind(distinctStu, e._stuID) == -1) {
                        distinctStu.push_back(e._stuID);
                    }
                }
            }

            if ((int)distinctStu.size() > bestCnt) {
                bestCnt = (int)distinctStu.size();
                bestIdx = i;
            }
        }

        cout << "对学生 " << stuID << " 来说：教过学生最多的老师是 "
            << teacherIDs[bestIdx] << "（" << teacherNames[bestIdx]
            << "），不同学生数 = " << bestCnt << "\n";
    }

   
} 


