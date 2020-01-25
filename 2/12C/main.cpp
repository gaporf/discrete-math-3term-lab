#include <fstream>
#include <vector>
#include <algorithm>

bool dfs(size_t v, std::vector<char> &visit,
         std::vector<std::vector<size_t> > &g, std::vector<size_t> &match) {
    if (visit[v]) {
        return false;
    }
    visit[v] = true;
    for (size_t to : g[v]) {
        if (match[to] == g.size() || dfs(match[to], visit, g, match)) {
            match[to] = v;
            return true;
        }
    }
    return false;
}

int main() {
    std::ifstream in("matching.in");
    std::ofstream out("matching.out");
    size_t n;
    in >> n;
    std::vector<std::pair<int32_t, size_t> > w(n);
    for (size_t i = 0; i < n; i++) {
        in >> w[i].first;
        w[i].second = i;
    }
    std::sort(w.rbegin(), w.rend());
    std::vector<std::vector<size_t> > g(n);
    for (size_t i = 0; i < n; i++) {
        size_t k;
        in >> k;
        g[i].resize(k);
        for (size_t j = 0; j < k; j++) {
            in >> g[i][j];
            g[i][j]--;
        }
    }
    std::vector<size_t> match(n, n);
    for (size_t i = 0; i < n; i++) {
        std::vector<char> visit(n, false);
        dfs(w[i].second, visit, g, match);
    }
    std::vector<size_t> ans(n, 0);
    for (size_t i = 0; i < n; i++) {
        if (match[i] != n) {
            ans[match[i]] = i + 1;
        }
    }
    for (size_t v : ans) {
        out << v << " ";
    }
    out << std::endl;
    return 0;
}
