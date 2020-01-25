#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <queue>
#include <algorithm>

int main() {
    std::ifstream in("fullham.in");
    std::ofstream out("fullham.out");
    size_t n;
    in >> n;
    std::vector<std::vector<char> > g(n, std::vector<char>(n, false));
    for (size_t i = 1; i < n; i++) {
        std::string str;
        in >> str;
        for (size_t j = 0; j < i; j++) {
            if (str[j] == '1') {
                g[i][j] = g[j][i] = true;
            }
        }
    }
    std::deque<size_t> ans;
    for (size_t i = 0; i < n; i++) {
        ans.push_back(i);
    }
    for (size_t k = 0; k < n * (n - 1); k++) {
        size_t head = ans.front();
        ans.pop_front();
        if (!g[head][ans.front()]) {
            std::queue<size_t> que;
            size_t next_head = ans.front();
            ans.pop_front();
            que.push(next_head);
            while (true) {
                size_t i = ans.front();
                que.push(i);
                ans.pop_front();
                if (g[head][i] && g[next_head][ans.front()]) {
                    break;
                }
            }
            while (!que.empty()) {
                ans.push_front(que.front());
                que.pop();
            }
        }
        ans.push_back(head);
    }
    while (!ans.empty()) {
        out << ans.front() + 1 << " ";
        ans.pop_front();
    }
    in.close();
    out.close();
    return 0;
}
