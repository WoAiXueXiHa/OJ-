// https://leetcode.cn/problems/first-unique-character-in-a-string/description/
/*
* 
* 思路1 ：
* class Solution {
public:
    int firstUniqChar(string s) {
        array<int,26> cnt = {0};
        queue<int> q;

        for(int i = 0; i < s.size(); i++){
            // 记录每个字母出现次数
            cnt[s[i] - 'a']++;

            // 当前索引入队列
            q.push(i);

            // 队列不为空才能出队
            // 不断弹出队首，直到：
            //   1) 队列空了（说明当前没有唯一字符），或者
            //   2) 队首对应字符的计数 == 1（它就是当前最靠左仍唯一的）
            while(!q.empty() && cnt[ s[q.front()] - 'a'] > 1){
                q.pop();
            }
        }
        // 循环体结束时：
        // 如果 q 非空，q.front() 就是到目前为止最左的唯一字符下标
        // 如果 q 为空，说明目前没有唯一字符

        return q.empty() ? -1 : q.front();
    }
};
*/