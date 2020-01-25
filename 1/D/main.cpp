#include <fstream>
#include <vector>
#include <algorithm>

void increase_cycle(std::vector<size_t> &u, std::vector<std::vector<char> > &g) {
    size_t n = g.size();
    std::vector<char> used(n, false);
    std::for_each(u.begin(), u.end(), [&used](size_t v) { used[v] = true; });
    for (size_t i = 0; i < u.size(); i++) {
        size_t j = (i + 1 < u.size()) ? i + 1 : 0;
        for (size_t k = 0; k < n; k++) {
            if (!used[k] && g[u[i]][k] && g[k][u[j]]) {
                u.insert(u.begin() + i + 1, k);
                return;
            }
        }
    }
    std::vector<size_t> v1, v2;
    for (size_t k = 0; k < n; k++) {
        if (!used[k] && g[k][u[0]]) {
            v1.push_back(k);
        } else {
            v2.push_back(k);
        }
    }
    for (size_t w2 : v2) {
        for (size_t w1 : v1) {
            if (g[w2][w1]) {
                u.insert(u.begin() + 1, w2);
                u.insert(u.begin() + 2, w1);
                return;
            }
        }
    }
}

int main() {
    std::ifstream in("guyaury.in");
    std::ofstream out("guyaury.out");
    size_t n;
    in >> n;
    std::vector<std::vector<char> > g(n, std::vector<char>(n, false));
    for (size_t i = 1; i < n; i++) {
        std::string str;
        in >> str;
        for (size_t j = 0; j < i; j++) {
            if (str[j] == '1') {
                g[i][j] = true;
            } else {
                g[j][i] = true;
            }
        }
    }
    std::vector<size_t> u;
    for (size_t i = 1; i < n; i++) {
        bool exit = false;
        for (size_t j = 1; j < n; j++) {
            if (g[0][i] && g[i][j] && g[j][0]) {
                u.push_back(0);
                u.push_back(i);
                u.push_back(j);
                exit = true;
                break;
            }
        }
        if (exit) {
            break;
        }
    }
    while (u.size() < n) {
        increase_cycle(u, g);
    }
    std::for_each(u.begin(), u.end(), [&out](size_t v) { out << v + 1 << " "; });
    out << std::endl;
    in.close();
    out.close();
    return 0;
}
