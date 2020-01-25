#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

struct task {
    int64_t w;
    size_t d;

    bool operator<(task const &rhs) {
        return d < rhs.d;
    }
};

int main() {
    freopen("schedule.in", "r", stdin);
    freopen("schedule.out", "w", stdout);
    std::ios_base::sync_with_stdio(false);
    size_t n;
    std::cin >> n;
    std::vector<task> tasks(n);
    for (auto &task : tasks) {
        std::cin >> task.d >> task.w;
    }
    sort(tasks.begin(), tasks.end());
    int64_t ans = 0;
    std::multiset<int64_t> st;
    for (size_t i = 1; i <= n; i++) {
        if (st.size() + 1 > tasks[i - 1].d) {
            if (!st.empty() && *st.begin() < tasks[i - 1].w) {
                ans += *st.begin();
                st.erase(st.begin());
                st.insert(tasks[i - 1].w);
            } else {
                ans += tasks[i - 1].w;
            }
        } else {
            st.insert(tasks[i - 1].w);
        }
    }
    std::cout << ans << std::endl;
    return 0;
}
